#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mpi.h"
#include "static.h"

#define HEAVY 100000
#define SHORT 1
#define LONG 10

int main(int argc, char *argv[]) {
	double start = MPI_Wtime();
	MPI_Status status;
	int myRank, size;
	int N = 20;
	int numOfTasks = N * N;

	prepareMPI(argc, argv, &size, &myRank, N);

	if (myRank == 0) {
		masterDistribute(N, size - 1);
		// note! --> the masterCollect method returns double (final answer), that's why it's an argument of printAnswer method.
		printAnswer(start, masterCollect(size, &status));

	} else if (myRank <= numOfTasks)
		slaveCompute(N, &status);

	/* shut down MPI */
	MPI_Finalize();

	return 0;
}

// This function performs heavy computations,
// its run time depends on x and y values
double heavy(int x, int y) {

	int i, loop = SHORT;
	double sum = 0;

// Super heavy tasks
	if (x < 3 || y < 3)
		loop = LONG;
// Heavy calculations
	for (i = 0; i < loop * HEAVY; i++)
		sum += cos(exp(sin((double) i / HEAVY)));

	return sum;
}

// This function distributes the range of tasks for slaves **size is number of slaves**
void masterDistribute(int N, int size) {

	int range[4] = { 0 }, slave_number = 1;
	int outer_range_jump = N * N / size / N; // constant jump in *outer* loop for slave compute range of heavy calculation
	int inner_range_jump = N * N / size % N; // constant jump in *inner* loop for slave compute range of heavy calculation
	int reminder = (N * N) % size;

	range[1] = N == size ? 0 : outer_range_jump;

	range[3] = inner_range_jump - 1 < 0 ? N - 1 : inner_range_jump - 1;

	while (1) {
		if (reminder > 0) {
			reminder--;
			if (range[3]++ > N - 1) {
				range[3] = 0;
				range[1]++;
			}
		}
		MPI_Send((int*) range, 4, MPI_INT, slave_number, 0, MPI_COMM_WORLD);
		slave_number++;
		if (range[1] >= N - 1 && range[3] >= N - 1)
			break;
		range[0] = range[1];
		if ((range[2] = range[3] + 1) > N - 1) {
			range[2] = 0;
			range[0]++;
		}
		if ((range[3] += inner_range_jump) > N - 1) {
			range[3] %= N;
			range[1]++;
		}
		if ((range[1] += outer_range_jump) > N - 1)
			range[1] = range[3] = N - 1;
	}
}

// This function called by the master (rank 0), compute all the slaves results and return final answer
double masterCollect(int size, MPI_Status *status) {
	int i;
	double slaveResult, answer;
	for (i = 1; i < size; ++i) {
		MPI_Recv(&slaveResult, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0,
		MPI_COMM_WORLD, status);
		answer += slaveResult;

	}
	return answer;

}

//This function called by the slaves for receiving range of tasks from master , calculate and send it back
void slaveCompute(int N, MPI_Status *status) {

	int taskRange[4] = { 0 }, x, y;
	double answer;

	MPI_Recv((int*) taskRange, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, status);
	for (x = taskRange[0]; x <= taskRange[1]; x++) {
		if (x > taskRange[0])
			taskRange[2] = 0;
		for (y = taskRange[2]; y < N; y++) {
			answer += heavy(x, y);
			if (x == taskRange[1] && y == taskRange[3])
				break;
		}
	}

	MPI_Send(&answer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}

//Called by master (rank 0), print the final result
void printAnswer(double start, double answer) {
	double time_spent = MPI_Wtime() - start;
	printf("\nanswer = %e\n\nExecute Time: %lf seconds\n\n", answer,
			time_spent);
}

//preparing for parallel computation
void prepareMPI(int argc, char *argv[], int *size, int *myRank, int N) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, size);
	MPI_Comm_rank(MPI_COMM_WORLD, myRank);

	if (*size < 2)
		MPI_Abort(MPI_COMM_WORLD, __LINE__);

	//no need in more processes then tasks! in main every rank higher then size of tasks finish main without any task
	if (*size > N * N + 1)
		*size = N * N + 1;

}

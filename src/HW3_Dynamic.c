#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mpi.h"
#include "dynamic.h"

#define HEAVY 100000
#define SHORT 1
#define LONG 10

int main(int argc, char *argv[]) {
	double start = MPI_Wtime();
	MPI_Status status;
	int myRank, size;
	int N = 20;

	prepareMPI(argc, argv, &size, &myRank, N);

	// note! --> the masterDistribute method returns double(final answer), that's why it's an argument of printAnswer method.
	if (myRank == 0)
		printAnswer(start, masterDistribute(size, N, &status));

	else
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

// This function distributes tasks for slaves when no task left to do --> last goodbye hand_shake with tag = 1
double masterDistribute(int size, int N, MPI_Status *status) {
	int i = 0, tag = 0, taskRange[2] = { 0 };
	int numOfTasks = N * N;
	double result, answer;

	while (++i < size) {

		MPI_Send(taskRange, 2, MPI_INT, i, 0, MPI_COMM_WORLD);

		if (++taskRange[1] > N - 1) {
			taskRange[1] = 0;
			taskRange[0]++;
		}
	}

	while (size > 1) {
		MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
		MPI_COMM_WORLD, status);

		if (++i > numOfTasks)
			tag = 1;

		if (status->MPI_TAG == 0) {
			answer += result;
			MPI_Send(taskRange, 2, MPI_INT, status->MPI_SOURCE, tag,
			MPI_COMM_WORLD);
			if (++taskRange[1] > N - 1) {
				taskRange[1] = 0;
				taskRange[0]++;
			}

		} else
			size--;
	}
	return answer;
}

//This function called by the slaves for receiving tasks from master , calculate, send it back and wait for next task until tag = 1
void slaveCompute(int N, MPI_Status *status) {

	int taskRange[2] = { 0 };
	double result = 0;

	MPI_Recv(taskRange, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, status);

	result = heavy(taskRange[0], taskRange[1]);

	while (1) {
		MPI_Send(&result, 1, MPI_DOUBLE, 0, status->MPI_TAG, MPI_COMM_WORLD);
		if (status->MPI_TAG)
			break;

		MPI_Recv(taskRange, 3, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, status);

		result = !status->MPI_TAG ? heavy(taskRange[0], taskRange[1]) : 0;

	}

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

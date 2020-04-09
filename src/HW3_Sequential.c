#include <stdio.h>
#include <math.h>
#include <time.h>

#define HEAVY 100000
#define SHORT 1
#define LONG 10

void calc_Heavy(int N, clock_t begin);
double heavy(int x, int y);

int main(int argc, char *argv[]) {
	clock_t begin = clock();
	int N = 20;

	calc_Heavy(N, begin);
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
// This function Calculate the heavy function for N
void calc_Heavy(int N, clock_t begin) {
	int x, y;

	double answer = 0;

	for (x = 0; x < N; x++)
		for (y = 0; y < N; y++)
			answer += heavy(x, y);

	double time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
	printf("answer = %e\n\nExecute Time: %lf seconds\n\n", answer, time_spent);
}

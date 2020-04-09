//all method's belong to dynamic task pool solution (HW3_Dynamic.c file)
double heavy(int x, int y);
double masterDistribute(int size, int N, MPI_Status *status);
void slaveCompute(int N, MPI_Status *status);
void prepareMPI(int argc, char *argv[], int *size, int *myRank, int N);
void printAnswer(double start, double answer);

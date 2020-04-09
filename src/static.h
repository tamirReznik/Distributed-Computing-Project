//all method's belong to static task pool solution (HW3_Static.c file)
double heavy(int x, int y);
void prepareMPI(int argc, char *argv[], int *size, int *myRank, int N);
void masterDistribute(int N, int size);
double masterCollect(int size, MPI_Status *status);
void slaveCompute(int N, MPI_Status *status);
void printAnswer(double begin, double answer);

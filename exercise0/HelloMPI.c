#include <cstdio>
#include <cstdlib>
#include <mpi.h>

int main(void) {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name(name, &len);

    if (rank == 0)
        printf("Hello World! From rank %d of %d on machine %s\n", rank, size, name);
    else
        printf("Just a normal process from rank %d of %d on machine %s\n", rank, size, name);

    MPI_Finalize();
    return 0;
}

// Basic repeated point-to-point MPI communication.
// Rank 0 sends three integers to rank 1, reusing the same variable.
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) std::cerr << "Run this program with at least 2 processes.\n";
        MPI_Finalize();
        return 1;
    }

    int number = 0;
    for (int i = 0; i < 3; ++i) {
        if (rank == 0) {
            number = i * 10;
            MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << number << std::endl;
        } else if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}

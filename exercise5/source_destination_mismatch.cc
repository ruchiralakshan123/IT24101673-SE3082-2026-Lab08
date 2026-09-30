#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        if (rank == 0) std::cerr << "Run with at least 3 processes to demonstrate the mismatch.\n";
        MPI_Finalize();
        return 1;
    }

    int value = 99;
    if (rank == 0) {
        std::cout << "Rank 0 sends value 99 to rank 1 (tag 0)." << std::endl;
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Rank 0 completed MPI_Send." << std::endl;
    } else if (rank == 1) {
        std::cout << "Rank 1 incorrectly waits for a message from rank 2." << std::endl;
        MPI_Recv(&value, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "This line is normally not reached: " << value << std::endl;
    }

    MPI_Finalize();
    return 0;
}

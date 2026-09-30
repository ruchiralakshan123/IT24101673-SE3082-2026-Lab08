// Exercise 5.2: Rewrite message2.cc using MPI_Bsend.
// The same send variable is reused after each buffered send.
#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) std::cerr << "Run with at least 2 processes.\n";
        MPI_Finalize();
        return 1;
    }

    int number = 0;
    std::vector<char> buffer;

    if (rank == 0) {
        int packed_size = 0;
        MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &packed_size);
        const int buffer_size = 3 * (packed_size + MPI_BSEND_OVERHEAD);
        buffer.resize(buffer_size);
        MPI_Buffer_attach(buffer.data(), buffer_size);
    }

    for (int i = 0; i < 3; ++i) {
        if (rank == 0) {
            number = i * 10;
            MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 buffered-send " << number << std::endl;
        } else if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << std::endl;
        }
    }

    if (rank == 0) {
        void* detached = nullptr;
        int detached_size = 0;
        MPI_Buffer_detach(&detached, &detached_size);
    }

    MPI_Finalize();
    return 0;
}

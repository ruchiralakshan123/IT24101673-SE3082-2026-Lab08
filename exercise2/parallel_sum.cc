// Exercise 2: Sum the integers from 1 to 10,000,000 using MPI.
#include <mpi.h>
#include <algorithm>
#include <cstdint>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 10000000LL;
    const long long base = N / size;
    const long long remainder = N % size;

    const long long local_count = base + (rank < remainder ? 1 : 0);
    const long long start = rank * base + std::min<long long>(rank, remainder) + 1;
    const long long end = start + local_count - 1;

    MPI_Barrier(MPI_COMM_WORLD);
    const double begin = MPI_Wtime();

    long long local_sum = 0;
    for (long long i = start; i <= end; ++i) {
        local_sum += i;
    }

    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    const double elapsed = MPI_Wtime() - begin;
    double max_elapsed = 0.0;
    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        const long long expected = N * (N + 1) / 2;
        std::cout << "Processes: " << size << '\n';
        std::cout << "Sum 1.." << N << " = " << global_sum << '\n';
        std::cout << "Expected = " << expected << '\n';
        std::cout << "Correct: " << (global_sum == expected ? "YES" : "NO") << '\n';
        std::cout << "TIME_SECONDS=" << max_elapsed << '\n';
    }

    MPI_Finalize();
    return 0;
}

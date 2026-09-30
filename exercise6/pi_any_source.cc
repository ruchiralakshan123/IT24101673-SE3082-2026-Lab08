// Exercise 6: Monte Carlo Pi with the root receiving worker results using MPI_ANY_SOURCE.
#include <mpi.h>
#include <iomanip>
#include <iostream>
#include <random>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long TOTAL_SAMPLES = 10000000LL;
    const long long base = TOTAL_SAMPLES / size;
    const long long remainder = TOTAL_SAMPLES % size;
    const long long local_samples = base + (rank < remainder ? 1 : 0);

    std::mt19937_64 generator(20210906ULL + static_cast<unsigned long long>(rank) * 104729ULL);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    MPI_Barrier(MPI_COMM_WORLD);
    const double begin = MPI_Wtime();

    long long local_inside = 0;
    for (long long i = 0; i < local_samples; ++i) {
        const double x = dist(generator);
        const double y = dist(generator);
        if (x * x + y * y <= 1.0) ++local_inside;
    }

    long long total_inside = 0;
    if (rank == 0) {
        total_inside = local_inside;
        for (int i = 1; i < size; ++i) {
            long long worker_inside = 0;
            MPI_Status status;
            MPI_Recv(&worker_inside, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 100,
                     MPI_COMM_WORLD, &status);
            total_inside += worker_inside;
            std::cout << "Received result from rank " << status.MPI_SOURCE << '\n';
        }
    } else {
        MPI_Send(&local_inside, 1, MPI_LONG_LONG, 0, 100, MPI_COMM_WORLD);
    }

    const double elapsed = MPI_Wtime() - begin;
    double max_elapsed = 0.0;
    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        const double pi = 4.0 * static_cast<double>(total_inside) / TOTAL_SAMPLES;
        std::cout << std::setprecision(12);
        std::cout << "Estimated Pi = " << pi << '\n';
        std::cout << "Processes = " << size << '\n';
        std::cout << "TIME_SECONDS=" << max_elapsed << '\n';
    }

    MPI_Finalize();
    return 0;
}

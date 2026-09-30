# SE4060 Parallel Computing — Lab Sheet 06 (MPI)

This repository contains the implementation for the MPI exercises in **SE4060 – Parallel Computing, Lab Sheet 6**.

## Lab Objective

Write MPI programs and practise parallel computation, point-to-point communication, `MPI_ANY_SOURCE`, buffered sends, and basic performance analysis.

## Repository Structure

```text
exercise0/  MPI lecture starter activity (HelloMPI)
exercise1/  Basic message-passing programs
exercise2/  Parallel sum from 1 to 10,000,000
exercise3/  Monte Carlo Pi with 10,000,000 samples
exercise4/  Performance-analysis instructions
exercise5/  Source/destination mismatch + MPI_Bsend version of message2
exercise6/  Monte Carlo Pi receiving from MPI_ANY_SOURCE
exercise7/  Exercise 6 using MPI_Bsend
scripts/    Benchmark and graph-generation scripts
results/    Generated timings and graphs
```

## Requirements

Install an MPI implementation such as Open MPI or MPICH, plus a C/C++ compiler.

Typical commands available after installation:

```bash
mpicxx --version
mpirun --version
```

For performance graphs, Python 3 and Matplotlib are also required:

```bash
python3 -m pip install matplotlib
```

## Compile Everything

From the repository root:

```bash
make
```

Compiled executables are created in `bin/`.

---

## Exercise 0 — Lecture MPI Activities

The starter `HelloMPI` program prints the rank and processor name.

Run:

```bash
mpirun -np 4 ./bin/hello_mpi
```

---

## Exercise 1 — GitHub Repository and Basic MPI Programs

The lab asks students to maintain the solutions in a GitHub repository. Two basic communication examples are included:

```bash
mpirun -np 2 ./bin/message1
mpirun -np 2 ./bin/message2
```

`message1.cc` sends one integer from rank 0 to rank 1.

`message2.cc` sends three integer values while reusing the same variable.

---

## Exercise 2 — Parallel Sum from 1 to 10,000,000

File:

`exercise2/parallel_sum.cc`

The range is divided among all MPI processes. Each process computes its own local sum, and `MPI_Reduce` combines the partial results at rank 0.

Run:

```bash
mpirun -np 1 ./bin/parallel_sum
mpirun -np 2 ./bin/parallel_sum
mpirun -np 4 ./bin/parallel_sum
```

The correct mathematical result is:

```text
50000005000000
```

The program checks the computed answer automatically.

---

## Exercise 3 — Parallel Monte Carlo Calculation of Pi

File:

`exercise3/monte_carlo_pi.cc`

The program performs a total of **10,000,000 Monte Carlo samples**. Samples are divided among MPI processes. Each process counts points that fall inside the unit quarter-circle, and the counts are combined to estimate Pi.

Run:

```bash
mpirun -np 1 ./bin/monte_carlo_pi
mpirun -np 2 ./bin/monte_carlo_pi
mpirun -np 4 ./bin/monte_carlo_pi
```

Because this is a Monte Carlo method, the result is an approximation of Pi.

---

## Exercise 4 — Time vs Processors and Speedup

The lab sheet requests graphs for execution time and speedup. Real timing values must come from an actual run; they should not be fabricated.

This repository includes an automated benchmark:

```bash
make
python3 -m pip install matplotlib
bash scripts/benchmark.sh
```

It runs the computational MPI programs with 1, 2, 4, and 8 processes and creates CSV files and PNG graphs inside `results/`.

Speedup is calculated as:

```text
Speedup(p) = T1 / Tp
```

where `T1` is the execution time with one process and `Tp` is the execution time with `p` processes.

> Note: the wording of the supplied lab sheet says “For both Exercise 1 and Exercise 2.” The repository provides performance measurement for the two computational exercises (parallel sum and Monte Carlo Pi), because these are the programs for which processor scaling is meaningful.

---

## Exercise 5.1 — Source/Destination Mismatch

File:

`exercise5/source_destination_mismatch.cc`

This program deliberately creates a mismatch:

- rank 0 sends to rank 1;
- rank 1 waits for a message from rank 2.

Compile with `make`, then demonstrate it using at least three processes:

```bash
mpirun -np 3 ./bin/mismatch
```

### Observation

The receive operation does not match the message sent by rank 0. Rank 1 waits for a message from rank 2 that never arrives, so the program can block/hang and may not reach `MPI_Finalize`.

Stop the intentionally blocked test with `Ctrl+C` after observing the behaviour.

---

## Exercise 5.2 — Buffered Send (`MPI_Bsend`)

File:

`exercise5/message2_bsend.cc`

The program attaches a user buffer using `MPI_Buffer_attach`, sends values with `MPI_Bsend`, and detaches the buffer using `MPI_Buffer_detach`.

Run:

```bash
mpirun -np 2 ./bin/message2_bsend
```

The same send variable is reused for multiple values.

---

## Exercise 6 — `MPI_ANY_SOURCE`

File:

`exercise6/pi_any_source.cc`

Each worker calculates its local Monte Carlo result and sends it to rank 0. Rank 0 uses:

```cpp
MPI_Recv(&worker_inside, 1, MPI_LONG_LONG,
         MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
```

This allows rank 0 to receive the next available worker result regardless of which worker finishes first.

Run:

```bash
mpirun -np 4 ./bin/pi_any_source
```

### Comparison with Exercise 3

- Exercise 3 uses `MPI_Reduce` to combine all partial counts.
- Exercise 6 explicitly receives worker results.
- `MPI_ANY_SOURCE` allows results to arrive in any worker order.
- Both versions should produce a Pi estimate close to the real value, although Monte Carlo values can vary slightly.

---

## Exercise 7 — `MPI_ANY_SOURCE` with Buffered Send

File:

`exercise7/pi_any_source_bsend.cc`

This version keeps the `MPI_ANY_SOURCE` receive logic from Exercise 6 but changes worker sends to `MPI_Bsend`.

Run:

```bash
mpirun -np 4 ./bin/pi_any_source_bsend
```

---

## Suggested GitHub Submission Workflow

Create a repository using your student ID if required by the lecturer, then from this project folder run:

```bash
git init
git add .
git commit -m "Complete SE4060 MPI Lab 06"
git branch -M main
git remote add origin YOUR_GITHUB_REPOSITORY_URL
git push -u origin main
```

Replace `YOUR_GITHUB_REPOSITORY_URL` with your repository URL.

## Important Before Final Submission

1. Compile the repository with `make` in an environment where MPI is installed.
2. Run Exercises 2, 3, 5, 6, and 7.
3. Run `bash scripts/benchmark.sh` to generate your real performance CSV files and graphs.
4. Add the generated `results/` files to Git.
5. Commit and push the final results.


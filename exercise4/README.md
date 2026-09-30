# Exercise 4 — Performance Analysis

Use the supplied benchmark and plotting scripts from the repository root:

```bash
make
python3 -m pip install matplotlib
bash scripts/benchmark.sh
```

The benchmark records execution times for the parallel sum and Monte Carlo Pi programs with different process counts.

Generated outputs are placed in `results/`:

- Time vs Number of Processors graphs
- Speedup graphs
- Raw CSV timing data

Speedup formula:

```text
Speedup(p) = T1 / Tp
```

Actual timing data must be collected on the computer or cluster used for the lab.

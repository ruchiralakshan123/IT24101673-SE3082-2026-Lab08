#!/usr/bin/env bash
set -euo pipefail

# Run this from the repository root after compiling.
# Example: bash scripts/benchmark.sh

PROCS=(1 2 4 8)
mkdir -p results

echo "processors,time_seconds" > results/sum_times.csv
echo "processors,time_seconds" > results/pi_times.csv

extract_time() {
  awk -F= '/TIME_SECONDS=/{print $2}' | tail -n 1
}

for p in "${PROCS[@]}"; do
  echo "Running parallel sum with $p processes..."
  t=$(mpirun -np "$p" ./bin/parallel_sum | extract_time)
  echo "$p,$t" >> results/sum_times.csv

  echo "Running Monte Carlo Pi with $p processes..."
  t=$(mpirun -np "$p" ./bin/monte_carlo_pi | extract_time)
  echo "$p,$t" >> results/pi_times.csv
done

python3 scripts/plot_results.py

echo "Done. CSV files and graphs are in the results/ directory."

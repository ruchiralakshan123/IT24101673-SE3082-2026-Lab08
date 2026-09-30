import csv
from pathlib import Path
import matplotlib.pyplot as plt

RESULTS = Path("results")

def read_csv(path):
    processors, times = [], []
    with path.open(newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            processors.append(int(row["processors"]))
            times.append(float(row["time_seconds"]))
    return processors, times

def make_plots(csv_name, prefix, title):
    processors, times = read_csv(RESULTS / csv_name)
    if not processors:
        return

    baseline = times[0]
    speedups = [baseline / t for t in times]

    plt.figure()
    plt.plot(processors, times, marker="o")
    plt.xlabel("Number of Processors")
    plt.ylabel("Time (seconds)")
    plt.title(f"{title}: Time vs Number of Processors")
    plt.grid(True)
    plt.savefig(RESULTS / f"{prefix}_time_vs_processors.png", bbox_inches="tight")
    plt.close()

    plt.figure()
    plt.plot(processors, speedups, marker="o", label="Measured Speedup")
    plt.plot(processors, processors, linestyle="--", label="Ideal Speedup")
    plt.xlabel("Number of Processors")
    plt.ylabel("Speedup")
    plt.title(f"{title}: Speedup")
    plt.legend()
    plt.grid(True)
    plt.savefig(RESULTS / f"{prefix}_speedup.png", bbox_inches="tight")
    plt.close()

make_plots("sum_times.csv", "sum", "Parallel Sum")
make_plots("pi_times.csv", "pi", "Monte Carlo Pi")
print("Graphs generated in results/.")

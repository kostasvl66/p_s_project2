import subprocess
import re
import sys

# -----------------------------
# CONFIGURATION
# -----------------------------
Ns = [100000, 1000000, 10000000]
Ms = [1, 2, 4, 8, 16]
J = 4

EXECUTABLE = "./sort"
LOG_FILE = "sort_benchmark_log.txt"

# -----------------------------
# FLOAT EXTRACTION REGEX
# -----------------------------
float_regex = re.compile(r"[-+]?\d*\.\d+")

# -----------------------------
# MAIN BENCHMARK
# -----------------------------
with open(LOG_FILE, "w") as log:

    # ---------- HEADER ----------
    header = ["N", "serial"] + [str(m) + " S/P" for m in Ms]
    log.write(" ".join(header) + "\n")

    # ---------- ROWS ----------
    for N in Ns:
        row = [str(N)]

        # ---- SERIAL ----
        serial_sum = 0.0

        for _ in range(J):
            result = subprocess.run(
                [EXECUTABLE, str(N), "serial"],
                capture_output=True,
                text=True
            )

            if result.returncode != 0:
                print(f"Serial execution failed for N={N}")
                print(result.stderr)
                sys.exit(1)

            floats = float_regex.findall(result.stdout)

            if len(floats) != 1:
                print("ERROR: Expected exactly ONE float in serial output!")
                print(result.stdout)
                sys.exit(1)

            serial_sum += float(floats[0])

        avg_serial = serial_sum / J
        row.append(f"{avg_serial:.6f}")

        # ---- PARALLEL ----
        for M in Ms:
            parallel_sum = 0.0

            for _ in range(J):
                result = subprocess.run(
                    [EXECUTABLE, str(N), "parallel", str(M)],
                    capture_output=True,
                    text=True
                )

                if result.returncode != 0:
                    print(f"Parallel execution failed for N={N}, M={M}")
                    print(result.stderr)
                    sys.exit(1)

                floats = float_regex.findall(result.stdout)

                if len(floats) != 1:
                    print("ERROR: Expected exactly ONE float in parallel output!")
                    print(result.stdout)
                    sys.exit(1)

                parallel_sum += float(floats[0])

            avg_parallel = parallel_sum / J
            row.append(f"{avg_parallel:.6f} {(avg_serial / avg_parallel):.6f}")

        log.write(" ".join(row) + "\n")

print("Sort benchmark completed. Results written to:", LOG_FILE)

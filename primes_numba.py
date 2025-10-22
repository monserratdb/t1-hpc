import time
import math
from numba import njit, prange # Import njit and prange [cite: 6430]
import os # To read environment variable for threads

# Numba accelerated is_prime function
@njit # Use nopython mode for best performance [cite: 6386, 6402]
def is_prime_numba(n):
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    # Use math.isqrt for efficient integer square root if available,
    # otherwise simulate with int(sqrt())
    limit = int(math.sqrt(n)) + 1
    for i in range(3, limit, 2):
        if n % i == 0:
            return False
    return True

# Numba accelerated parallel prime counting function
@njit(parallel=True) # Enable parallel execution [cite: 6412]
def count_primes_parallel(limit):
    count = 0
    # Use prange for parallel loop [cite: 6416, 6434]
    # Numba handles the reduction on 'count' automatically here
    for n in prange(2, limit):
        if is_prime_numba(n):
            count += 1
    return count

if __name__ == "__main__":
    limit = 400000000 # [cite: 4823]

    # --- Optional: Run once for JIT compilation ---
    # Numba compiles on the first call, which takes extra time 
    print("Compiling function (first run)...")
    _ = count_primes_parallel(1000) # Run on a small input
    print("Compilation complete.")
    # --- End Optional Warmup ---

    # Get number of threads from environment variable
    # Numba respects NUMBA_NUM_THREADS
    num_threads = os.environ.get('NUMBA_NUM_THREADS', 'N/A')

    start_time = time.perf_counter() # Start timer
    prime_count = count_primes_parallel(limit)
    end_time = time.perf_counter() # Stop timer
    duration = end_time - start_time

    print("\n=== Versión Python + Numba ===")
    print(f"Rango: 2 a {limit}")
    print(f"Threads Usados: {num_threads}") # Optional
    print(f"Primos encontrados: {prime_count}")
    print(f"Tiempo: {duration:.6f} segundos")
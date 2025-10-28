import time
import math
from numba import njit, prange
import os
import sys  # Para leer argumentos de línea de comandos

# Función para verificar si un número es primo
@njit
def is_prime_numba(n):
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    limit = int(math.sqrt(n)) + 1
    for i in range(3, limit, 2):
        if n % i == 0:
            return False
    return True

# Función paralela para contar primos
@njit(parallel=True)
def count_primes_parallel(limit):
    count = 0
    for n in prange(2, limit):
        if is_prime_numba(n):
            count += 1
    return count

if __name__ == "__main__":
    # --- Lectura de argumento 'limit' ---
    if len(sys.argv) < 2:
        print(f"Uso: python {sys.argv[0]} <limit>")
        print("Ejemplo: python numba_primes.py 400000000")
        sys.exit(1)

    limit = int(sys.argv[1])
    if limit < 2:
        print("El límite debe ser un entero mayor que 1.")
        sys.exit(1)

    # --- Warmup para compilación JIT ---
    _ = count_primes_parallel(1000)

    # Obtener número de threads desde la variable de entorno
    num_threads = os.environ.get('NUMBA_NUM_THREADS', 'N/A')

    # --- Medición de tiempo ---
    start_time = time.perf_counter()
    prime_count = count_primes_parallel(limit)
    end_time = time.perf_counter()
    duration = end_time - start_time

    # --- Resultados ---
    print("\n=== Versión Python + Numba ===")
    print(f"Rango: 2 a {limit}")
    print(f"Threads Usados: {num_threads}")
    print(f"Primos encontrados: {prime_count}")
    print(f"Tiempo: {duration:.6f} segundos")

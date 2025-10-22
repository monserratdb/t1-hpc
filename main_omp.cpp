#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <omp.h>

bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    // Optimization: check up to sqrt(n)
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int limit = 400000000;
    int count = 0; // Initialize count to 0

    auto start = std::chrono::high_resolution_clock::now();

    // Use parallel for but update count atomically inside the loop
    #pragma omp parallel for schedule(runtime)
    for (int n = 2; n < limit; n++) {
        if (is_prime(n)) {
            #pragma omp atomic // Ensure this update is thread-safe
            count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    int threads_used = 0;
    #pragma omp parallel
    {
        #pragma omp master
        threads_used = omp_get_num_threads();
    }


    std::cout << "=== Versión OpenMP (Atomic) ===" << std::endl;
    std::cout << "Rango: 2 a " << limit << std::endl;
    std::cout << "Threads Usados: " << threads_used << std::endl;
    std::cout << "Primos encontrados: " << count << std::endl;
    std::cout << "Tiempo: " << duration.count() << " segundos" << std::endl;

    return 0;
}
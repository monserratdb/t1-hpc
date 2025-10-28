#include <iostream>
#include <chrono>
#include <cmath>
#include <omp.h>
#include <cstdlib> // para atoi()

/**
 * @brief Verifica si un número es primo. Implementación optimizada.
 */
bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    int limit = (int)std::sqrt(n);
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // --- Lectura del argumento 'limit' ---
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <limit>\n";
        std::cerr << "Ejemplo: " << argv[0] << " 400000000\n";
        return 1;
    }

    const int limit = atoi(argv[1]);
    if (limit < 2) {
        std::cerr << "El límite debe ser un número entero mayor que 1.\n";
        return 1;
    }

    long long count = 0;

    // Obtener número de hilos (desde OMP_NUM_THREADS o configuración por defecto)
    int threads_used = omp_get_max_threads();
    if (getenv("OMP_NUM_THREADS")) {
        threads_used = atoi(getenv("OMP_NUM_THREADS"));
    }

    auto start = std::chrono::high_resolution_clock::now();

    // --- Bucle paralelo con reducción ---
    #pragma omp parallel for reduction(+:count) schedule(runtime)
    for (int n = 2; n < limit; n++) {
        if (is_prime(n)) {
            count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // --- Resultados ---
    std::cout << "===============================================" << std::endl;
    std::cout << "Rango: 2 a " << limit << std::endl;
    std::cout << "Primos encontrados: " << count << std::endl;
    std::cout << "Threads usados: " << threads_used << std::endl;
    std::cout << "OMP_SCHEDULE: " << (getenv("OMP_SCHEDULE") ? getenv("OMP_SCHEDULE") : "static (Default)") << std::endl;
    std::cout << "Tiempo de ejecucion: " << duration.count() << " segundos" << std::endl;
    std::cout << "===============================================" << std::endl;

    return 0;
}

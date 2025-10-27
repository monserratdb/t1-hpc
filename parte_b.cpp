#include <iostream>
#include <chrono>
#include <cmath>
#include <omp.h>

/**
 * @brief Verifica si un número es primo. Implementación optimizada.
 * * @param n El número a verificar.
 * @return true si es primo, false en caso contrario.
 */
bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    // Solo necesitamos verificar divisores impares hasta la raíz cuadrada de n
    // La raíz cuadrada se calcula una vez.
    int limit = (int)std::sqrt(n);
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    // Definir el límite superior de conteo.
    // Usar un límite alto garantiza suficiente carga para ver las diferencias.
    const int limit = 400000000; 
    
    // Usamos 'long long' ya que la cantidad de primos puede ser muy grande.
    long long count = 0; 
    
    // --- Configuración OpenMP ---
    // Puedes fijar el número de hilos aquí si no usas la variable de entorno
    // omp_set_num_threads(4); 

    int threads_used = omp_get_max_threads();
    // Intenta obtener el valor de OMP_NUM_THREADS si fue fijado.
    if (getenv("OMP_NUM_THREADS")) {
        threads_used = atoi(getenv("OMP_NUM_THREADS"));
    }


    // --- Medición de Tiempo ---
    auto start = std::chrono::high_resolution_clock::now();

    // =========================================================================
    // PARALELIZACIÓN CRÍTICA
    // reduction(+:count) es ESENCIAL para un conteo eficiente y correcto.
    // schedule(runtime) permite configurar la estrategia con la variable de entorno OMP_SCHEDULE.
    // =========================================================================
    #pragma omp parallel for reduction(+:count) schedule(runtime)
    for (int n = 2; n < limit; n++) {
        if (is_prime(n)) {
            count++;
        }
    }

    // --- Fin de Medición de Tiempo ---
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // --- Impresión de Resultados ---
    std::cout << "===============================================" << std::endl;
    std::cout << "Rango: 2 a " << limit << std::endl;
    std::cout << "Primos encontrados: " << count << std::endl;
    std::cout << "Threads Usados: " << threads_used << std::endl;
    std::cout << "OMP_SCHEDULE: " << (getenv("OMP_SCHEDULE") ? getenv("OMP_SCHEDULE") : "static (Default)") << std::endl;
    std::cout << "Tiempo de Ejecución: " << duration.count() << " segundos" << std::endl;
    std::cout << "===============================================" << std::endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

const int MAX_ALTURA = 100; // Máxima altura considerada
std::vector<std::mutex> altura_locks(MAX_ALTURA / 10 + 1); // Un mutex por segmento de 10m

// Estructura para almacenar datos del drone
struct Drone {
    int id;
    int altura_deseada;
};

void despegar_drone(Drone drone, std::vector<Drone>& drones) {
    int segmento = drone.altura_deseada / 10;
    std::mutex& lock = altura_locks[segmento];

    // Verificar si hay un drone consecutivo activo
    bool puede_despegar = true;
    for (const auto& otro_drone : drones) {
        if (otro_drone.id == drone.id - 1 || otro_drone.id == drone.id + 1) { // Consecutivo por ID
            int otro_segmento = otro_drone.altura_deseada / 10;
            if (otro_segmento == segmento || std::abs(otro_segmento - segmento) <= 1) {
                puede_despegar = false;
                break;
            }
        }
    }

    if (puede_despegar) {
        std::lock_guard<std::mutex> guard(lock); // Bloquea el segmento
        std::cout << "Dron " << drone.id << " esperando para despegar...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simula despegue
        std::cout << "Dron " << drone.id << " despega hacia " << drone.altura_deseada << "m\n";
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simula ascenso
        std::cout << "Dron " << drone.id << " alcanzó " << drone.altura_deseada << "m\n";
    } else {
        std::cout << "Dron " << drone.id << " espera, conflicto con drone consecutivo\n";
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Espera y reintenta
        despegar_drone(drone, drones); // Reintento (simplificado)
    }
}

int main() {
    // Lista de drones (ID, altura deseada)
    std::vector<Drone> drones = {
        {0, 0}, {1, 0}, {2, 0}, {3, 40}, {4, 0}
    };

    std::vector<std::thread> threads;
    for (const auto& drone : drones) {
        threads.emplace_back(despegar_drone, drone, std::ref(drones));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
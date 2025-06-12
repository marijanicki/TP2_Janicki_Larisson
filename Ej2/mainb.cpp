#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
void tarea_con_polling(const std::string& name);
std::mutex m1, m2;

void tarea_con_polling(const std::string& name) {
while (true) {
// Intenta adquirir ambos mutex sin bloquear
if (std::try_lock(m1, m2) == -1) {
std::cout << name
<< " obtuvo ambos mutex, trabajando…\n";
std::this_thread::sleep_for(
std::chrono::milliseconds(500));
m1.unlock(); m2.unlock();
break;
} else {
// No se pudo adquirir, se hace algo más
std::cout << name
<< " no pudo obtener los mutex, sigue intentando...\n";
std::this_thread::sleep_for(
std::chrono::milliseconds(100));
}
}
}

int main() {
std::thread t1(tarea_con_polling, "Hilo 1");
std::thread t2(tarea_con_polling, "Hilo 2");
t1.join(); t2.join();
std::cout << "Ambos hilos terminaron\n";
return 0;
}
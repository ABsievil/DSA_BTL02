#include "main.h"
#include "restaurant.cpp"
#include <chrono>

int main(int argc, char* argv[]) {
    string fileName = "output_random.txt";
    auto start = std::chrono::high_resolution_clock::now();
    simulate(fileName);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time run: " << duration.count() << "(s)\n";
    cin.get();
    system("pause");
    return 0;
}

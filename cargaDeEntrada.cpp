#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> // std::shuffle
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono::system_clock

// Este programa solicita al usuario una cantidad de números, los almacena en un vector,
// los mezcla aleatoriamente y luego los escribe en un archivo CSV.
// El archivo CSV tendrá una sola fila con los números separados por comas.
// El programa utiliza la biblioteca estándar de C++ para manejar la entrada/salida y la aleatoriedad.
// El archivo CSV se llamará "numeros_aleatorios.csv".
// el programa se compila con la llamada a g++ -std=c++17 cargaDeEntrada.cpp -o cargaDeEntrada
// El programa se ejecuta con ./cargaDeEntrada



int main() {
    int n;
    std::cout << "Ingrese la cantidad de números: ";
    std::cin >> n;

    std::vector<int> numeros;

    // Generador de números aleatorios en el rango [0, n]
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generador(seed);
    std::uniform_int_distribution<int> distribucion(0, n);

    for (int i = 0; i < n; ++i) {
        numeros.push_back(distribucion(generador));
    }

    // Mezclar aleatoriamente
    std::shuffle(numeros.begin(), numeros.end(), generador);

    // Escribir en archivo CSV (modo append)
    std::ofstream archivo("numeros_aleatorios.csv", std::ios::app);
    if (!archivo) {
        std::cerr << "Error al abrir el archivo para escritura.\n";
        return 1;
    }

    for (size_t i = 0; i < numeros.size(); ++i) {
        archivo << numeros[i];
        if (i != numeros.size() - 1) {
            archivo << ",";
        }
    }

    archivo << std::endl;
    archivo.close();

    std::cout << "Números aleatorios agregados a 'numeros_aleatorios.csv'.\n";

    return 0;
}
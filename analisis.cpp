#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mpi.h>

#include "secuential/quickSort.cpp" 
#include "utils/fileReader.cpp"
#include "utils/checkSorted.cpp"
#include "parallel/quicksort.cpp"

// The code is compiled with:
// mpic++ analisis.cpp -o analisis -std=c++17
// Code to run:
// mpirun -np 4 ./analisis


using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    string filePath = "numeros_aleatorios.csv";
    string outputPath = "resultados.csv";

    vector<vector<int>> data = readFile(filePath);
    if (data.empty()) {
        if (rank == 0) cerr << "No data found in the file." << endl;
        MPI_Finalize();
        return 1;
    }

    ofstream outputFile;
    if (rank == 0) {
        outputFile.open(outputPath);
        if (!outputFile.is_open()) {
            cerr << "Failed to open output file." << endl;
            MPI_Finalize();
            return 1;
        }
        // Escribir encabezado
        outputFile << "Index,Size,Secuencial_ms,Paralelo_ms,Speedup,Eficiencia" << endl;
    }

    for (size_t i = 0; i < data.size(); i++) {
        vector<int> original = data[i];

        // Secuential sort
        vector<int> seqVec = original;
        auto startSeq = chrono::high_resolution_clock::now();
        quickSort(seqVec);
        auto endSeq = chrono::high_resolution_clock::now();
        double timeSeq = chrono::duration<double, milli>(endSeq - startSeq).count();

        // Parallel sort
        vector<int> parVec = original;
        auto startPar = chrono::high_resolution_clock::now();
        parallelQuickSort(parVec);
        auto endPar = chrono::high_resolution_clock::now();
        double timePar = chrono::duration<double, milli>(endPar - startPar).count();

        if (rank == 0) {
            if (!checkSorted(seqVec)) {
                cerr << "Secuential sort failed on row " << i << endl;
                continue;
            }
            if (!checkSorted(parVec)) {
                cerr << "Parallel sort failed on row " << i << endl;
                continue;
            }

            double speedup = timePar > 0.0 ? timeSeq / timePar : 0.0;
            double efficiency = speedup / size;

            outputFile << i << "," 
                       << original.size() << "," 
                       << fixed << setprecision(2) << timeSeq << "," 
                       << fixed << setprecision(2) << timePar << "," 
                       << fixed << setprecision(2) << speedup << "," 
                       << fixed << setprecision(2) << efficiency
                       << endl;
        }
    }

    if (rank == 0) outputFile.close();

    MPI_Finalize();
    return 0;
}
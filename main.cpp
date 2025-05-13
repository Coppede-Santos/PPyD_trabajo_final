#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "secuential/quickSort.cpp" 
#include "utils/fileReader.cpp"
#include "utils/checkSorted.cpp"

using namespace std;

int main() {
    string filePath = "numeros_aleatorios.csv"; // Path to the CSV file

    vector<vector<int>> data = readFile(filePath);
    if (data.empty()) {
        cerr << "No data found in the file." << endl;
        return 1;
    }

   
    
    // Sort each row of the data
    for (auto& row : data) {
        // Start timer 
        auto start = chrono::high_resolution_clock::now();

        quickSort(row);
   
        // End timer
        auto end = chrono::high_resolution_clock::now();
        double duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Check if the row is sorted
        if (!checkSorted(row)) {
            cerr << "Row is not sorted." << endl;
            return 1;
        }

        // Print the duration
        cout << "Row of size " << row.size() << " sorted in " << duration << " ms." << endl;    
    }
    
    return 0;
}
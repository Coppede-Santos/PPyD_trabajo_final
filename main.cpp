#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "secuential/quickSort.cpp" 
#include "utils/fileReader.cpp"

using namespace std;

int main() {
    string filePath = "data.csv"; // Path to the CSV file

    vector<vector<int>> data = readFile(filePath);
    if (data.empty()) {
        cerr << "No data found in the file." << endl;
        return 1;
    }

    // Start timer 
    auto start = chrono::high_resolution_clock::now();
    
    // Sort each row of the data
    for (auto& row : data) {
        quickSort(row);
    }

    // End timer
    auto end = chrono::high_resolution_clock::now();
    int duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    int avgDuration = duration / data.size();

    // Check if the data is sorted
    for (const auto& row : data) {
        for (size_t i = 1; i < row.size(); ++i) {
            if (row[i] < row[i - 1]) {
                cerr << "Data is not sorted in row" << i << endl;
                return 1;
            }
        }
    }

    cout << "Data is sorted successfully in " << duration << " ms." << endl;
    
    return 0;
}
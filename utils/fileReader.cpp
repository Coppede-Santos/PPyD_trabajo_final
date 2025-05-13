// Read a csv and get the vectors of data (int)
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<int>> readFile(string filePath) {
    // Open the file
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return {};
    }

    // Read each line from the file
    vector<vector<int>> data;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<int> row;

        // Read each value in the line
        while (getline(ss, value, ',')) {
            try {
                // Convert the value to an integer and add it to the row
                row.push_back(stoi(value));
            } catch (const invalid_argument& e) {
                cerr << "Invalid value: " << value << endl;
            }
        }

        // Add the row to the data
        data.push_back(row);
    }

    // Close the file
    file.close();

    return data;
}
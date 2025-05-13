#include <iostream>
#include <vector>

using namespace std;
bool checkSorted(vector<int> v) {
    for (int i = 1; i < v.size(); i++) {
        if (v[i] < v[i - 1]) {
            return false; // Not sorted
        }
    }

    return true; // Sorted
}
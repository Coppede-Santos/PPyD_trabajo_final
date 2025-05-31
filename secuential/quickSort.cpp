#include <iostream>
#include <vector>

namespace secuential {
    int quickSort(std::vector<int>& arr);
}

int quickSortRecursive(std::vector<int>& arr, int left, int right) {
    // If the array has 1 or 0 elements, it is already sorted
    if (left >= right) return 0; 

    // Choose a pivot element (middle one)
    int pivot = arr[left + (right - left) / 2];

    // Partition the array into two halves
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        // Swap elements if they are in the wrong order
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    // Recursively sort the two halves
    quickSortRecursive(arr, left, j);
    quickSortRecursive(arr, i, right);

    return 0;
}

int quickSort(std::vector<int>& arr) {
    // Injecto code to spend time
    int size = arr.size() / 500;
    int x;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            x = i * j; // This line is just to inject some computation time
        }
    }
    
    return quickSortRecursive(arr, 0, arr.size() - 1);
}



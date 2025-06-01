#include <iostream>
#include <vector>

#define LOAD_FACTOR 800


namespace secuential {
    int quickSort(std::vector<int>& arr);
}

int quickSortRecursive(std::vector<int>& arr, int left, int right) {
    if (left >= right) return 0;

    int pivot = arr[left + (right - left) / 2];
    int i = left, j = right;

    // 💡 Carga artificial proporcional al tamaño del subarreglo
    int size = right - left + 1;
    int dummy = 0;
    for (int a = 0; a < size * LOAD_FACTOR; ++a) {
    dummy += (a ^ 1234567);
}


    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

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



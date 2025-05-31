
#include <iostream>
#include <vector>
#include <mpi.h>


using namespace std;

int parallelQuickSort(std::vector<int>& arr) {
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Master process 
    vector<int> sorted_array; // Define sorted_array here to make it accessible later
    if (rank == 0) {
        // Calculate the sum of all elements
        long long sum = 0;
        for (int i = 0; i < arr.size(); ++i) {
            if (arr[i] < 0) {
                cerr << "Negative values are not allowed in the array." << endl;
                return -1; // Return an error code
            }

            sum += arr[i];
        }

        if (sum == 0) {
            // If the sum is zero, all elements are zero, so we can return the original array
            sorted_array = arr;
            return 0;
        }


        // The number of pivots is equal to the number of processes minus one
        // Get the pivot values to distribute between processes
        int average_value = sum / arr.size();
        int pivot_gap = average_value * 2 / size;

        vector<int> pivots(size - 1);
        int current_pivot =  0;
        for (int i = 0; i < size - 1; ++i) {
            current_pivot += pivot_gap;
            pivots[i] = current_pivot;
        }

        vector<vector<int>> chunks(size);
        for (int i = 0; i < arr.size(); ++i) {
            bool assigned = false;
            for (int j = 0; j < size - 1; ++j) {
                // Check if the element is less than the pivot
                if (arr[i] < pivots[j]) {
                    chunks[j].push_back(arr[i]);
                    assigned = true;
                    break;
                }
            }

            // If the element is not assigned to any chunk, add it to the last chunk
            if (!assigned) {
                chunks[size - 1].push_back(arr[i]);
            }
        }

        // Log the chunks size 
        for (int i = 0; i < size; ++i) {
            cout << "Chunk " << i << " size: " << chunks[i].size() << endl;
        }

        // Send the chunks to all processes
        vector<int> local_chunk;
        int chunk_size;
        for (int i = 1; i < size; ++i) {
            chunk_size = chunks[i].size();
            MPI_Send(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            if (chunk_size > 0) {
                MPI_Send(chunks[i].data(), chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        // Sort the local chunk
        local_chunk = chunks[0]; // Master process sorts its own chunk
        quickSort(local_chunk);
        sorted_array = local_chunk; // Initialize sorted_array with the master's local chunk


        for (int i = 1; i < size; ++i) {
            MPI_Recv(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (chunk_size > 0) {
                local_chunk.resize(chunk_size);
                MPI_Recv(local_chunk.data(), chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sorted_array.insert(sorted_array.end(), local_chunk.begin(), local_chunk.end());
            }
        }

        // Return the sorted array to the master process
        arr = sorted_array;

    } else {
        // Other processes receive their chunks
        int chunk_size;
        MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> local_chunk(chunk_size);
        if (chunk_size > 0) {
            MPI_Recv(local_chunk.data(), chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Sort the local chunk
        quickSort(local_chunk);

        // Send the sorted local chunk back to the master process
        MPI_Send(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if (chunk_size > 0) {
            // Send the sorted local chunk back to the master process
            MPI_Send(local_chunk.data(), chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    

    return 0;
}

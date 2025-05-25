
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
        int sum = 0;
        for (int i = 0; i < arr.size(); ++i) {
            sum += arr[i];
        }

        // The number of pivots is equal to the number of processes minus one
        // Get the pivot values to distribute between processes
        int average_value = sum / arr.size();
        int pivot_gap = average_value * 2 / size;

        vector<int> pivots(size - 1);
        int current_pivot =  0;
        for (int i = 0; i < size - 1; ++i) {
            pivots[i] = current_pivot;
            current_pivot += pivot_gap;
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

        // Send the chunks to all processes
        vector<int> local_chunk;
        int chunk_size;
        for (int i = 0; i < size; ++i) {
            chunk_size = chunks[i].size();
            MPI_Send(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            if (chunk_size > 0) {
                MPI_Send(chunks[i].data(), chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        // Sort the local chunk
        local_chunk = chunks[0]; // Master process sorts its own chunk
        std::qsort(local_chunk.begin(), local_chunk.end());

        // Collect the sorted chunks from all processes
        for (int i = 0; i < size; ++i) {
            MPI_Recv(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (chunk_size > 0) {
                local_chunk.resize(chunk_size);
                MPI_Recv(local_chunk.data(), chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sorted_array.insert(sorted_array.end(), local_chunk.begin(), local_chunk.end());
            }
        }

    } else {
        // Other processes receive their chunks
        int chunk_size;
        MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        vector<int> local_chunk(chunk_size);
        if (chunk_size > 0) {
            MPI_Recv(local_chunk.data(), chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Sort the local chunk
        std::qsort(local_chunk.begin(), local_chunk.end());

        // Send the sorted local chunk back to the master process
        MPI_Send(local_chunk.data(), chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }

    // Finalize MPI
    MPI_Finalize();
    // If this is the master process, print the sorted array
    if (rank == 0) {
        cout << "Sorted array: ";
        for (const auto& val : sorted_array) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}

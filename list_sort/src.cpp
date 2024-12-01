#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>


void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void iterativeMergeSort(std::vector<int>& arr, int n) {
    for (int size = 1; size < n; size *= 2) {
#pragma omp parallel for
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = std::min(left + size - 1, n - 1);
            int right = std::min(left + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

    #pragma optimize( "", off )
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int global_size = 1e+10; // Размер общего массива
    if (global_size % size != 0) {
        if (rank == 0) {
            std::cerr << "Error: global_size must be divisible by the number of processes." << std::endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int local_size = global_size / size;
    std::vector<int> data;
    if (rank == 0) {
        data.resize(global_size);
        std::srand(42); // Для воспроизводимости
        for (int i = 0; i < global_size; i++) {
            data[i] = std::rand() % 1000;
        }
    }

    std::vector<int> local_data(local_size);
    MPI_Scatter(data.data(), local_size, MPI_INT,
        local_data.data(), local_size, MPI_INT,
        0, MPI_COMM_WORLD);

    int num_threads = omp_get_max_threads();
    num_threads = 24;
    omp_set_num_threads(num_threads);

    auto start_time = std::chrono::high_resolution_clock::now();
    iterativeMergeSort(local_data, local_size);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    //#pragma optimize( "", on )

    MPI_Gather(local_data.data(), local_size, MPI_INT,
        data.data(), local_size, MPI_INT,
        0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int step = local_size; step < global_size; step *= 2) {
            for (int i = 0; i < global_size; i += 2 * step) {
                int mid = std::min(i + step - 1, global_size - 1);
                int right = std::min(i + 2 * step - 1, global_size - 1);
                merge(data, i, mid, right);
            }
        }

        std::cout << "Отсортированный массив (первые 10 элементов): ";
        for (int i = 0; i < std::min(10, global_size); i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    bool suc = true;

    for (int i = 0; i < global_size; ++i) {
        //std::cout << i << " " << data[i] << std::endl;
        if (data[i] > data[std::min(i + 1, global_size - 1)])
        {
            std::cout << "False" << std::endl;
            std::cout << data[i] << " " << data[std::min(i + 1, global_size - 1)] << std::endl;
            std::cout << i << " " << std::min(i + 1, global_size - 1) << std::endl;
            suc = false;
        }


    }std::cout << "suc: " << suc << "  t: " << elapsed.count() << std::endl;

    MPI_Finalize();
    return 0;
}




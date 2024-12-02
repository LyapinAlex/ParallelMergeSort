#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <chrono>


void merge(std::vector<int>& arr, long long left, long long mid, long long right) {
    long long n1 = mid - left + 1;
    long long n2 = right - mid;

    std::vector<long long> L(n1), R(n2);

    for (long long i = 0; i < n1; i++) L[i] = arr[left + i];
    for (long long i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    long long i = 0, j = 0, k = left;

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

void iterativeMergeSort(std::vector<int>& arr, long long n) {
    for (long long size = 1; size < n; size *= 2) {
#pragma omp parallel for
        for (long long left = 0; left < n - 1; left += 2 * size) {
            long long mid = std::min(left + size - 1, n - 1);
            long long right = std::min(left + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

int main(int argc, char** argv) {



    std::map<std::string, long long> myEnvIntArgs;
    myEnvIntArgs["NUM_THREADS="] = 2;
    myEnvIntArgs["SIZE_LIST="] = 1e+5;
    myEnvIntArgs["RANDOM_LIST="] = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        for (const auto& pair : myEnvIntArgs) {
            if (arg.rfind(pair.first, 0) == 0)
                myEnvIntArgs[pair.first] = std::stoi(arg.substr(pair.first.size()));
        }

    }

    omp_set_num_threads(myEnvIntArgs["NUM_THREADS="]);
    long long global_size = myEnvIntArgs["SIZE_LIST="];
    std::vector<int> data;

    if (myEnvIntArgs["RANDOM_LIST="] == 1){
        data.resize(global_size);
        std::srand(42); // Для воспроизводимости
        for (int i = 0; i < global_size; i++) {
            data[i] = std::rand() % 1000;
        }

    }
    else
    {   
        std::cout << "Введите элементы массива через пробел и нажмите Enter: ";
        std::string input_line;
        std::getline(std::cin, input_line);
        std::istringstream input_stream(input_line);

        int number;
        while (input_stream >> number) {
            data.push_back(number);
        }
        global_size = data.size();
    }


    auto start_time = std::chrono::high_resolution_clock::now();
    iterativeMergeSort(data, global_size);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;


    bool suc = true;

    for (long long i = 0; i < global_size; ++i)
        if (data[i] > data[std::min(i + 1, global_size - 1)])
            suc = false;

    std::cout << "Время работы алгоритма: " << elapsed.count() << std::endl;

    std::ofstream output("output.txt", std::ios::app);
    output << std::scientific;
    output << "(" << myEnvIntArgs["NUM_THREADS="] << ", " << global_size << ", " << elapsed.count() << ")\n";
    output.close();


    return 0;
}




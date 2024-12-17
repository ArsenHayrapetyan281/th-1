#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

void sum_array_portion(const std::vector<int>& arr, size_t start, size_t end, long long& partial_sum) {
    partial_sum = 0;
    for (size_t i = start; i < end; ++i) {
        partial_sum += arr[i];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <array_size> <thread_count>\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    size_t M = std::stoull(argv[2]);
    std::vector<int> arr(N);
    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(M, 0);

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < N; ++i) {
        arr[i] = dist(gen);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    long long total_sum = 0;
    for (size_t i = 0; i < N; ++i) {
        total_sum += arr[i];
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_single = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    start_time = std::chrono::high_resolution_clock::now();
    size_t chunk_size = N / M;
    for (size_t i = 0; i < M; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == M - 1) ? N : start + chunk_size;
        threads.emplace_back(sum_array_portion, std::cref(arr), start, end, std::ref(partial_sums[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }
    total_sum = 0;
    for (const auto& partial_sum : partial_sums) {
        total_sum += partial_sum;
    }
    end_time = std::chrono::high_resolution_clock::now();
    auto duration_threads = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Time spent without threads: " << duration_single << " ms\n";
    std::cout << "Time spent with " << M << " threads: " << duration_threads << " ms\n";

    return 0;
}

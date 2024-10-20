#include <iostream>
#include <vector>
#include <numeric>
#include <thread>

int main()
{
    const size_t data_size = 1'000'000;
    std::vector<int> data(data_size);

    std::iota(data.begin(), data.end(), 1);

    const unsigned int number_of_threads = std::thread::hardware_concurrency();
    const size_t chunk_size = data_size / number_of_threads;

    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(number_of_threads, 0);

    auto worker = [&data](int start_index, int end_index, long long& sum) {
        sum = std::accumulate(data.begin() + start_index, data.begin() + end_index, 0LL);
    };

    for (unsigned int i = 0; i < number_of_threads; i++) {
        size_t start_index = i * chunk_size;
        size_t end_index = (i == number_of_threads - 1) ? data_size : start_index + chunk_size;

        threads.emplace_back(worker, start_index, end_index, std::ref(partial_sums[i]));
    }

    for (auto& thread: threads) {
        thread.join();
    }

    long long total_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);

    std::cout << "Total sum: " << total_sum << std::endl;

    return 0;
}

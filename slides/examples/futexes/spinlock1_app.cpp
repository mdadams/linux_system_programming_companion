#include <format>
#include <iostream>
#include <thread>
#include <vector>
#include "spinlock1.hpp"

spinlock m;
unsigned long long count = 0;

void worker() {
	for (int i = 0; i < 10'000; ++i) {
		m.lock();
		++count;
		m.unlock();
	}
}

int main() {
	std::vector<std::jthread> threads;
	for (int i = 0; i < 4; ++i) {
		threads.emplace_back(worker);
	}
	for (auto&& t : threads) {t.join();}
	std::cout << std::format("{}\n", count);
}

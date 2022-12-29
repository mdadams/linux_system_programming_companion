#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include "barrier.hpp"

void worker(int instance, barrier& b) {
	for (int i = 0; i < 10; ++i) {
		b.wait();
		std::cout << "1";
		b.wait();
		std::cout << "2";
		b.wait();
		if (!instance) {std::cout << '\n';}
	}
}

int main() {
	int num_threads = 16;
	std::vector<std::jthread> threads;
	barrier b(num_threads);
	for (int i = 0; i < num_threads; ++i) {
		threads.emplace_back(worker, i, std::ref(b));
	}
}

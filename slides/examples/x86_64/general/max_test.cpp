#include <cassert>
#include <vector>
#include <tuple>
#include <iostream>

int max(int, int);

int main() {
	const std::vector<std::pair<std::pair<int, int>, int>> tests{
		{{1, 2}, 2},
		{{-1, 2}, 2},
		{{1, -2}, 1},
		{{-1, -2}, -1},
		{{0, 0}, 0},
		{{0, 1}, 1},
		{{1, 0}, 1},
		{{1, 1}, 1},
	};

	std::cout << "sizeof(int) " << sizeof(int) << '\n';

	for (auto&& [input, output] : tests) {
		if (max(input.first, input.second) != output) {
			std::cerr << input.first << ' ' << input.second << ' ' <<
			  output << '\n';
		}
	}
}

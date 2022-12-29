#include <cassert>
#include <iostream>
#include <vector>

unsigned int hamming_weight(unsigned int);

int main() {
	const std::vector<
	  std::pair<unsigned int, unsigned int>> tests{
		{0, 0},
		{0b1, 1},
		{0b1001, 2},
		{0b01010, 2},
		{0b1111, 4},
		{0b1000000, 1},
		{1u << (8 * sizeof(unsigned int) - 1), 1},
		{(1u << (8 * sizeof(unsigned int))) - 1, 8 * sizeof(unsigned int)},
	};
	for (auto [input, output] : tests) {
		if (hamming_weight(input) != output) {
			std::cerr << "failed " << input << '\n';
		}
	}
}

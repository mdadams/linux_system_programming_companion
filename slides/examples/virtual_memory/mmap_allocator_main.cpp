#include <boost/align/is_aligned.hpp>
#include <cassert>
#include <format>
#include <iostream>
#include <vector>
#include "mmap_allocator.hpp"

int main() {
	long page_size = sysconf(_SC_PAGE_SIZE);
	std::vector<int, mmap_allocator<int>> v{1, 2, 3};
	std::vector<int> w{1, 2, 3};
	bool v_aligned = boost::alignment::is_aligned(page_size, v.data());
	assert(v_aligned);
	bool w_aligned = boost::alignment::is_aligned(page_size, w.data());
	std::cout << std::format("{} {}\n", v_aligned,
	  static_cast<void*>(v.data()));
	std::cout << std::format("{} {}\n", w_aligned,
	  static_cast<void*>(w.data()));
}

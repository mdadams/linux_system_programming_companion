#include <format>
#include <iostream>
#include <iomanip>
#include <sys/mman.h>
#include <unistd.h>

int main() {
	char buf[256];
	sprintf(buf, "cat /proc/%d/maps", getpid());
	std::cout << "The memory mappings for this process are as follows:\n";
	if (system(buf)) {std::cerr << "cat failed\n"; return 1;}
	long page_size = sysconf(_SC_PAGE_SIZE);
	size_t length = page_size;
	std::cout << "WARNING: This program is likely going to crash very soon.\n";
	for (size_t length = page_size; length; length <<= 1) {
		std::cout << std::format("Deleting all memory mappings up to "
		  "(but not including) address {:#8x}.\n", length);
		if (munmap(0, length)) {
			std::cerr << "munmap failed\n";
			return 1;
		}
	}
}

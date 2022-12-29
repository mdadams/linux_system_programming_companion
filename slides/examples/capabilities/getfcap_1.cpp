#include <format>
#include <iostream>
#include <sys/capability.h>
#include <sys/types.h>
#include "unique_cap.hpp"

int64_t getcapmask(cap_t cap, cap_flag_t set) {
	uint64_t mask = 0;
	for (int i = 0; i <= CAP_LAST_CAP; ++i) {
		cap_flag_value_t value;
		if (cap_get_flag(cap, i, set, &value)) {return -1;}
		mask = (mask << 1) | value;
	}
	return mask;
}

int main(int argc, char** argv) {
	if (argc < 2) {std::cerr << "bad usage\n"; return 1;}
	unique_cap cap(cap_get_file(argv[1]));
	if (!cap) {
		if (errno == ENODATA) {std::cout << "no capabilities\n"; return 0;}
		else {std::cerr << "cap_get_proc failed\n"; return 1;}
	}
	int64_t pmask = getcapmask(cap.get(), CAP_PERMITTED);
	int64_t emask = getcapmask(cap.get(), CAP_EFFECTIVE);
	int64_t imask = getcapmask(cap.get(), CAP_INHERITABLE);
	if (emask < 0 || imask < 0 || pmask < 0)
		{std::cerr << "cannot get capabilities\n"; return 1;}
	std::cout << std::format(
	  "  permitted set: {:#016x}\n"
	  "  effective bit: {:#01x}\n"
	  "inheritable set: {:#016x}\n",
	  pmask, emask, imask);
}

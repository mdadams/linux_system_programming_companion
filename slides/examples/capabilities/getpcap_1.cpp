#include <format>
#include <iostream>
#include <sys/capability.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include "unique_cap.hpp"

int64_t getcapmask_cap(cap_t cap, cap_flag_t set) {
	int64_t mask = 0;
	for (int i = 0; i <= CAP_LAST_CAP; ++i) {
		cap_flag_value_t value;
		if (cap_get_flag(cap, i, set, &value)) {return -1;}
		mask = (mask << 1) | value;
	}
	return mask;
}

int64_t getcapmask_prctl(int type) {
	int64_t mask = 0;
	for (int i = 0; i <= CAP_LAST_CAP; ++i) {
		int value = -1;
		if (type == PR_CAPBSET_READ) {value = prctl(PR_CAPBSET_READ, i);}
		else if (type == PR_CAP_AMBIENT)
			{value = prctl(PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, i, 0, 0);}
		if (value < 0) {return -1;}
		mask = (mask << 1) | value;
	}
	return mask;
}

int main() {
	unique_cap cap(cap_get_proc());
	if (!cap) {std::cerr << "cap_get_proc failed\n";}
	int64_t all = (static_cast<int64_t>(1) << (CAP_LAST_CAP) + 1) - 1;
	int64_t emask = getcapmask_cap(cap.get(), CAP_EFFECTIVE);
	int64_t imask = getcapmask_cap(cap.get(), CAP_INHERITABLE);
	int64_t pmask = getcapmask_cap(cap.get(), CAP_PERMITTED);
	int64_t bmask = getcapmask_prctl(PR_CAPBSET_READ);
	int64_t amask = getcapmask_prctl(PR_CAP_AMBIENT);
	int sbits = prctl(PR_GET_SECUREBITS);
	if (emask < 0 || imask < 0 || pmask < 0 || bmask < 0 || amask < 0 ||
	 sbits < 0)
		{std::cerr << "cannot get capability information\n"; return 1;}
	std::cout << std::format(
      "all capabilities: {:#016x}\n"
	  "   permitted set: {:#016x}\n"
	  "   effective set: {:#016x}\n"
	  " inheritable set: {:#016x}\n"
	  "    bounding set: {:#016x}\n"
	  "     ambient set: {:#016x}\n"
	  "     secure bits: {:#02x}\n",
	  all, pmask, emask, imask, bmask, amask, sbits);
}

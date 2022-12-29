#include <format>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>

int main() {
	std::cout << std::format(
	  "real UID: {}\n"
	  "real GID: {}\n"
	  "effective UID: {}\n"
	  "effective GID: {}\n",
	  getuid(), getgid(), geteuid(), getegid());
	auto max_groups = sysconf(_SC_NGROUPS_MAX);
	std::vector<gid_t> groups(max_groups);
	if (int num_groups = getgroups(groups.size(), groups.data());
	  num_groups > 0) {
		groups.resize(num_groups);
		std::cout << "supplementary GIDs:";
		for (auto gid : groups) {std::cout << std::format(" {}", gid);}
		std::cout << '\n';
	}
}

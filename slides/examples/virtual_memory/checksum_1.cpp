#include <format>
#include <iostream>
#include <numeric>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned int sysv_checksum(unsigned char* buf, size_t len) {
	unsigned long sum = 0;
	for (; len; ++buf, --len) {sum += *buf;}
	unsigned long x = (sum & 0xffff) + ((sum & 0xffffffff) >> 16);
	return (x & 0xffff) + (x >> 16);
}

int main(int argc, char** argv) {
	if (argc < 2) {return 1;}
	int fd;
	if ((fd = open(argv[1], O_RDONLY)) < 0) {return 1;}
	struct stat stat_buf;
	if (fstat(fd, &stat_buf) < 0) {return 1;}
	void* addr;
	if ((addr = mmap(nullptr, stat_buf.st_size, PROT_READ, MAP_PRIVATE,
	  fd, 0)) == MAP_FAILED) {return 1;}
	if (close(fd) < 0) {return 1;}
	std::cout << std::format("{}\n", sysv_checksum(
	  static_cast<unsigned char*>(addr), stat_buf.st_size));
}

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc < 2) {std::cerr << "bad usage\n"; return 1;}
	const std::string hello("Hello, World!\n");
	int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {std::cerr << "open failed\n"; return 1;}
	if (ftruncate(fd, hello.size()) < 0)
	  {std::cerr << "ftruncate failed\n"; return 1;}
	void* ptr = mmap(nullptr, hello.size(),
	  PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {std::cerr << "mmap failed\n"; return 1;}
	if (close(fd) < 0) {std::cerr << "close failed\n"; return 1;}
	char* cptr = static_cast<char*>(ptr);
	cptr = std::copy(hello.begin(), hello.end(), cptr);
	if (msync(ptr, hello.size(), MS_SYNC))
	  {std::cerr << "msync failed"; return 1;}
	if (munmap(ptr, hello.size())) {std::cerr << "munmap failed\n"; return 1;}
}

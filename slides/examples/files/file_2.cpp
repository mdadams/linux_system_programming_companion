#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "unique_fd.hpp"

int main() {
	std::string buf{"Hello, World!\n"};
	unique_fd fd(open("/tmp/foo", O_CREAT | O_TRUNC |
	  O_WRONLY, S_IRWXU));
	if (!fd) {std::cout << "open failed\n";}
	if (lseek(fd.get(), 1, SEEK_SET) < 0) {std::cerr << "lseek failed\n";}
	if (write(fd.get(), &buf[1], buf.size() - 1) != buf.size() - 1)
	  {std::cerr << "write failed\n";}
	if (lseek(fd.get(), 0, SEEK_SET) < 0) {std::cerr << "lseek failed\n";}
	if (write(fd.get(), &buf[0], 1) != 1) {std::cerr << "write failed\n";};
}

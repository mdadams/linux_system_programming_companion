#include <iostream>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "unique_fd.hpp"

ssize_t read_with_retry(int fd, void* buf, ssize_t count) {
	ssize_t n;
	while ((n = read(fd, buf, count)) < 0 && errno == EINTR) {}
	return n;
}

ssize_t write_with_retry(int fd, const void* buf, ssize_t count) {
	ssize_t n;
	while ((n = write(fd, buf, count)) < 0 && errno == EINTR) {}
	return n;
}

ssize_t write_all(int fd, const void* buf, ssize_t count) {
	const char* start = static_cast<const char*>(buf);
	ssize_t remaining = count;
	do {
		ssize_t n = write_with_retry(fd, start, remaining);
		if (n <= 0) {return -1;}
		remaining -= n;
		start += n;
	} while (remaining > 0);
	return count;
}

int copy(int source_fd, int destination_fd) {
	std::vector<char> buffer(64 * 1024);
	for (;;) {
		ssize_t n = read_with_retry(source_fd, buffer.data(), buffer.size());
		if (n < 0) {return -1;}
		else if (!n) {break;}
		if (write_all(destination_fd, buffer.data(), n) != n) {return -1;}
	}
	return 0;
}

int main(int argc, char** argv) {
	if (argc < 3) {std::cerr << "invalid usage\n";}
	unique_fd source_fd(open(argv[1], O_RDONLY));
	if (!source_fd)
		{std::cerr << "cannot open source file\n"; return 1;}
	unique_fd destination_fd(open(argv[2], O_CREAT | O_TRUNC | O_WRONLY,
	  S_IRWXU));
	if (!destination_fd)
		{std::cerr << "cannot open destination file\n"; return 1;}
	if (copy(source_fd.get(), destination_fd.get()))
		{std::cerr << "copy failed\n"; return 1;}
}

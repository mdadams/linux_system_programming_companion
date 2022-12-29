#include <array>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

ssize_t writev_all(int fd, iovec* iov, size_t count) {
	ssize_t written = 0;
	for (;;) {
		ssize_t n;
		while ((n = writev(fd, iov, count)) < 0 && errno == EINTR) {}
		if (n < 0) {return -1;}
		else if (!n) {return written;}
		written += n;
		for (; count > 0 && n >= iov->iov_len; ++iov, --count) {n -= iov->iov_len;}
		if (!count) {return written;}
		iov->iov_base = static_cast<char*>(iov->iov_base) + n;
		iov->iov_len -= n;
	}
}

int main() {
	std::string hello("Hello!");
	char bonjour[] = "Bonjour!";
	char newline = '\n';
	std::array<iovec, 4> iov{{
		{hello.data(), hello.size()}, {&newline, 1},
		{bonjour, std::strlen(bonjour)}, {&newline, 1},
	}};
	if (writev_all(1, &iov[0], iov.size()) < 0) {std::cerr << "write failed\n";}
}

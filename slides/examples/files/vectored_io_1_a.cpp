#include <array>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main() {
	std::string hello("Hello!");
	char bonjour[] = "Bonjour!";
	char newline = '\n';
	std::array<iovec, 4> iov{{
		{hello.data(), hello.size()}, {&newline, 1},
		{bonjour, std::strlen(bonjour)}, {&newline, 1},
	}};
	/* note: successfull write operation may not necessarily
	  write all of data */
	if (writev(1, &iov[0], iov.size()) < 0)
		{std::cerr << "write failed\n";}
}

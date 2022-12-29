#include <format>
#include <iostream>
#include <unistd.h>

int main() {
	std::cout << std::format(
	  "POSIX version: {}\n"
	  "maximum login name length: {}\n"
	  "maximum host name length: {}\n"
	  "maximum length of arguments to exec: {}\n"
	  "maximum number of open files: {}\n"
	  "page size in bytes: {}\n",
	  sysconf(_SC_VERSION),
	  sysconf(_SC_LOGIN_NAME_MAX),
	  sysconf(_SC_HOST_NAME_MAX),
	  sysconf(_SC_ARG_MAX),
	  sysconf(_SC_OPEN_MAX),
	  sysconf(_SC_PAGESIZE));
}

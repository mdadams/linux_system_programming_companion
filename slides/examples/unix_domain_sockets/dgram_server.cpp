#include <cstring>
#include <format>
#include <iostream>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char** argv) {
	std::string pathname("/tmp/socket_demo");
	if (argc >= 2) {pathname = argv[1];}
	int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
	if (bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
	  {std::cerr << "bind failed\n"; return 1;}
	constexpr int bufsize = 256;
	char buf[bufsize];
	for (;;) {
		int ret;
		socklen_t addr_len = sizeof(sockaddr_un);
		if ((ret = recvfrom(fd, buf, bufsize, 0, (struct sockaddr*) &addr,
		  &addr_len)) < 0) {std::cerr << "recvfrom failed\n"; return 1;}
		buf[ret] = '\0';
		if (ret == 0) {break;}
		if (!strcmp(buf, "end")) {break;}
		std::cout << std::format("{} {} {}\n", addr_len, ret, buf);
	}
	if (unlink(pathname.c_str()) == -1 && errno != ENOENT)
	  {std::cerr << "unlink failed\n"; return 1;}
}

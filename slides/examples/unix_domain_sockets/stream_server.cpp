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
	int sfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
	  {std::cerr << "bind failed\n"; return 1;}
	constexpr int bufsize = 256;
	char buf[bufsize];
	if (listen(sfd, 1)) {std::cerr << "listen failed\n"; return 1;}
	socklen_t addr_len = sizeof(sockaddr_un);
	int fd = accept(sfd, (struct sockaddr*) &addr, &addr_len);
	for (;;) {
		int ret;
		if ((ret = recv(fd, buf, bufsize, 0)) < 0)
		  {std::cerr << "recv failed\n"; return 1;}
		buf[ret] = '\0';
		if (ret == 0) {break;}
		std::cout << std::format("{} {} {}\n", addr_len, ret, buf);
	}
	close(fd);
	if (unlink(pathname.c_str()) == -1 && errno != ENOENT)
	  {std::cerr << "unlink failed\n"; return 1;}
}

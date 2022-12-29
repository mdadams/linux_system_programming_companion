#include <iostream>
#include <string>
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char** argv) {
	std::string pathname("/tmp/socket_demo");
	if (argc >= 2) {pathname = argv[1];}
	int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (fd < 0) {std::cerr << "socket failed\n"; return 1;}
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
	if (connect(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un)) < 0)
	  {std::cerr << "connect failed\n"; return 1;}
	std::string message;
	while (std::cin >> message) {
		int ret;
		if ((ret = send(fd, message.c_str(), message.size(), 0)) < 0 ||
		  ret != message.size())
		  {std::cerr << "send failed\n"; return 1;}
	}
	close(fd);
}

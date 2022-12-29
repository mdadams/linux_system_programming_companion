#include <cstring>
#include <iostream>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

ssize_t send_message(int fd, const void* buf, size_t count) {
	struct iovec iov = {.iov_base = const_cast<void*>(buf), .iov_len = count};
	struct msghdr hdr;
	hdr.msg_name = nullptr;
	hdr.msg_namelen = 0;
	hdr.msg_iov = &iov;
	hdr.msg_iovlen = 1;
	hdr.msg_control = nullptr;
	hdr.msg_controllen = 0;
	//hdr.msg_flags = MSG_EOR;
	hdr.msg_flags = 0;
	return sendmsg(fd, &hdr, 0);
}

int main(int argc, char** argv) {
	std::string pathname("/tmp/demo");
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
		if ((ret = send_message(fd, message.c_str(), message.size())) <= 0)
		  {std::cerr << "send failed\n"; return 1;}
	}
	close(fd);
}

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int copy(int source_fd, int destination_fd) {
	char buf[512];
	for (;;) {
		int n = read(source_fd, buf, sizeof(buf));
		if (n < 0) {return -1;}
		else if (n == 0) {break;}
		if (write(destination_fd, buf, n) != n) {return -1;}
	}
	return 0;
}

int send_fd(int sd, int fd) {
	struct msghdr msg = {0};
	char cbuf[CMSG_SPACE(sizeof(int))]{};
	char dbuf[1]{};
	struct iovec iov;
	iov.iov_base = dbuf;
	iov.iov_len = sizeof(dbuf);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cbuf;
	msg.msg_controllen = sizeof(cbuf);
	msg.msg_name = nullptr;
	msg.msg_namelen = 0;
	struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	memcpy(CMSG_DATA(cmsg), &fd, sizeof(int));
	return (sendmsg(sd, &msg, 0) < 0) ? -1 : 0;
}

int receive_fd(int sd) {
	struct msghdr msg = {0};
	char dbuf[1];
	struct iovec iov;
	iov.iov_base = dbuf;
	iov.iov_len = sizeof(dbuf);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	char cbuf[256];
	msg.msg_control = cbuf;
	msg.msg_controllen = sizeof(cbuf);
	msg.msg_name = nullptr;
	msg.msg_namelen = 0;
	ssize_t n = recvmsg(sd, &msg, 0);
	if (n < 0) {return -1;}
	struct cmsghdr* cmptr = CMSG_FIRSTHDR(&msg);
	if (!cmptr) {return -2;}
	if (cmptr->cmsg_len != CMSG_LEN(sizeof(int))) {return -10;}
	if (cmptr->cmsg_level != SOL_SOCKET) {return -3;}
	if (cmptr->cmsg_type != SCM_RIGHTS) {return -4;}
	int fd;
	memcpy(&fd, CMSG_DATA(cmptr), sizeof(int));
	return fd;
}

void child(int sd) {
	int fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0) {throw std::runtime_error("[child] open failed");}
	if (send_fd(sd, fd))
	  {throw std::runtime_error("[child] sending FD failed");}
}

void parent(int sd) {
	int fd = receive_fd(sd);
	if (fd < 0) {throw std::runtime_error("recv_fd failed");}
	if (copy(fd, 1)) {throw std::runtime_error("copy failed");}
}

int main(int argc, char** argv) try {
	int sock_fds[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_fds)) {
		throw std::runtime_error("socketpair failed");}
	pid_t pid = fork();
	if (pid > 0) {
		close(sock_fds[1]);
		parent(sock_fds[0]);
	} else if (pid == 0) {
		close(sock_fds[0]);
		child(sock_fds[1]);
	} else {throw std::runtime_error("fork failed");}
} catch (std::exception& e) {std::cerr << e.what() << '\n';}

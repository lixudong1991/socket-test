#include"unp.h"
#include <syslog.h>
void sig_chld(int s)
{
	int stat;
	while (waitpid(-1, &stat, WNOHANG)>0);
}
void str_echo(int fd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while ((n = read(fd, buf, MAXLINE))>0)
	{
		buf[n] = 0;
		printf("--> %s\n", buf);
		Writen(fd, buf, n);
	}
	if (n<0 && errno == EINTR)
		goto again;
	else if (n<0)
		err_sys("str_echo: read error");
	else if (n == 0)
		printf("client close\n");
}
extern int daemon_proc;
int daemon_openlog_init(char *name, int fac)
{
	pid_t pid;
	if ((pid = Fork()) < 0)
		return -1;
	else if (pid >0)
		_exit(0);
	if (setsid() < 0)
		return -1;
	Signal(SIGCHLD, SIG_IGN);
	if ((pid = Fork()) < 0)
		return -1;
	else if (pid > 0)
		_exit(0);
	for (int i = 0; i < 64; i++)
		close(i);

	daemon_proc = 1;
	chdir("/");
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);
	openlog(name, LOG_PID, fac);

	return 0;
}
int main(int argc, char **argv)
{
	if (argc!=2)
	{
		return 0;
	}
	daemon_openlog_init(argv[0],0);
	int nready, tcpfd, udpfd, confd,tem=1,n;
	struct sockaddr_in tcpaddr, udpaddr,cliaddr;
	socklen_t len = sizeof(cliaddr);
	fd_set rset;
	FD_ZERO(&rset);
	char recvbuf[MAXLINE];
	Signal(SIGCHLD, sig_chld);
	bzero(&tcpaddr, sizeof(tcpaddr));
	bzero(&udpaddr, sizeof(udpaddr));
	bzero(&cliaddr, sizeof(cliaddr));

	tcpaddr.sin_family = udpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = udpaddr.sin_port = htons(atoi(argv[1]));
	tcpaddr.sin_addr.s_addr = udpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	tcpfd = Socket(AF_INET, SOCK_STREAM, 0);
	udpfd = Socket(AF_INET, SOCK_DGRAM, 0);

	Setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &tem, sizeof(tem));
	Bind(tcpfd, (SA*)&tcpaddr, sizeof(tcpaddr));
	Bind(udpfd, (SA*)&udpaddr, sizeof(udpaddr));
	Listen(tcpfd, LISTENQ);
	for (;;)
	{
		FD_SET(tcpfd, &rset);
		FD_SET(udpfd, &rset);
		if ((nready = select(max(tcpfd, udpfd) + 1, &rset, NULL, NULL, NULL)) < 0)
		{
			if (errno = EINTR)
				continue;
			else
				err_sys("select error");
		}
		if (FD_ISSET(tcpfd, &rset))
		{
			confd = Accept(tcpfd, NULL, NULL);
			if (Fork() == 0)
			{
				close(tcpfd);
				str_echo(confd);
				exit(0);
			}
			else {
				close(confd);
			}
		}
		if (FD_ISSET(udpfd, &rset))
		{
			n=Recvfrom(udpfd, recvbuf, MAXLINE, 0, (SA*)&cliaddr, &len);
			Sendto(udpfd, recvbuf, n, 0, (SA*)&cliaddr, len);
		}
	}

}
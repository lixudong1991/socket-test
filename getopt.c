#include"unp.h"
#include<netinet/tcp.h>	

void getSopt(int sockfd)
{
	int val = 0, len = sizeof(int);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1)
	{
		err_ret("getscokopt SO_RCVBUF error");
	}
	printf("SO_RCVBUF : %d\n", val);
	val = 0;
	if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &val, &len) == -1)
	{
		err_ret("getscokopt SO_SNDBUF error");
	}
	printf("SO_SNDBUF : %d\n", val);
	val = 0;
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &val, &len) == -1)
	{
		err_ret("getscokopt TCP_MAXSEG error");
	}
	printf("TCP_MAXSEG : %d\n", val);
}
int main(int argc,char **argv)
{
	if (argc !=3)
	{
		return 0;
	}
	int sock = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	Inet_pton(AF_INET, argv[1], &addr.sin_addr);
	getSopt(sock);
	Connect(sock, (SA*)&addr, sizeof(addr));
	getSopt(sock);
	sleep(1);
	close(sock);
	return 0;
}
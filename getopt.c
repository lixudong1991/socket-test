#include"unp.h"


int main()
{
	int val = 0,len=sizeof(int);
	int sockfd = Socket(AF_INET, SOCK_STREAM, 0);
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

}
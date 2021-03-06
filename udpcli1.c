#include"unp.h"
void dg_cli(FILE *fp, int sockfd, const SA* pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE];
	Connect(sockfd, pservaddr, servlen);
	while (Fgets(sendline, MAXLINE, fp) != NULL)
	{
		Write(sockfd, sendline, strlen(sendline));
		n = Read(sockfd, recvline, MAXLINE);
		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
}
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		return 0;
	}
	struct sockaddr_in  addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	Inet_pton(AF_INET, argv[1], &addr.sin_addr);
	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	dg_cli(stdin, sockfd, (SA*)&addr, sizeof(addr));
	close(sockfd);

}
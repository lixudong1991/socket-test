#include"unp.h"
void dg_echo(int sockfd, struct sockaddr* addr, socklen_t cli)
{
	int n;
	socklen_t len=cli;
	char mesg[MAXLINE] = { 0 };
	char cliip[22] = { 0 };
	for (;;)
	{
		len = cli;
		bzero(cliip, 22);
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, addr, &len);
		mesg[n] = 0;
		printf("recvfrom : %s port: %d\n", Inet_ntop(AF_INET, &((struct sockaddr_in*)addr)->sin_addr, cliip, sizeof(cliip)), ntohs(((struct sockaddr_in*)addr)->sin_port));
		Sendto(sockfd, mesg, n, 0, addr, len);
	}
}
int main(int argc, char **argv)
{
	struct sockaddr_in addr,cliaddr;
	bzero(&addr, sizeof(addr));
	bzero(&cliaddr, sizeof(cliaddr));
	addr.sin_family = AF_INET;
	addr.sin_port =htons(9878);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	Bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

	dg_echo(sockfd, (SA*)&cliaddr, sizeof(cliaddr)); 
	

}
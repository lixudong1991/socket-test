#include"unp.h"
int main(int argc,char **argv)
{
	int listenfd, connfd, maxi, maxfd, sockfd, nready,i;
	int client[FD_SETSIZE] = { 0 };
	ssize_t n;
	fd_set rset, allset;
	struct sockaddr_in servaddr,clientaddr;
	socklen_t clientlen=sizeof(clientaddr);
	char buff[MAXLINE];
	char clientip[22]={0};
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9877);
	Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	maxfd = listenfd;
	maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
	{
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	for (;;)
	{
		rset = allset;
		nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(listenfd, &rset))
		{
			connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
			printf("%s  %d\n", Inet_ntop(AF_INET, &clientaddr.sin_addr, clientip, sizeof(clientip)), ntohs(clientaddr.sin_port));
			for (i = 0; i < FD_SETSIZE; i++)
			{
				if (client[i] < 0) 
				{
					client[i] = connfd;
					break;
				}			
			}
			if (i == FD_SETSIZE)
				err_quit("too many clients");
			if (maxfd < connfd)
				maxfd = connfd;
			FD_SET(connfd, &allset);
			if (i > maxi)
				maxi = i;
			if (--nready <= 0)
				continue;
		}
		for (i = 0; i <= maxi; i++)
		{
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset))
			{
				bzero(buff, MAXLINE);
				if ((n = Read(sockfd, buff, MAXLINE)) == 0)
				{
					client[i] = -1;
					FD_CLR(sockfd, &allset);
					Close(sockfd);
				}else{
					Writen(sockfd, buff, n);
				}
				if (--nready <= 0)
					break;
			}
		}
	}

}

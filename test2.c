#include"unp.h"
void sig(int signal)
{
        printf("SIGPIPE\n");
}
void str_cli(FILE *fp, int sockfd)
{
        char sendline[MAXLINE], recvline[MAXLINE];
		int maxfd;
		fd_set rset;
		FD_ZERO(&rset);
		for(;;)
		{
			FD_SET(fileno(fp),&rset);
			FD_SET(sockfd,&rset);
			maxfd=max(fileno(fp),sockfd)+1;
			Select(maxfd,&rset,NULL,NULL,NULL);
			if(FD_ISSET(sockfd,&rset))
			{
				if(Readline(sockfd,recvline,MAXLINE)==0)
				{
					err_quit("str_cli:server terminated prematurely");
				}
				Fputs(recvline,stdout);
		
			}
			if(FD_ISSET(fileno(fp),&rset))
			{
				if(Fgets(sendline,MAXLINE,fp)==NULL)
				return;
				Writen(sockfd,sendline,strlen(sendline));
			}
		}
}
int main(int argc, char **argv)
{
        int sockfd, n;
        char recvline[MAXLINE + 1];
        struct sockaddr_in servaddr;
        if (argc != 3)
                err_quit("usage: a.out <IPaddress> <port>");
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                err_sys("socket error");
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
        if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
                err_quit("inet_pton error for &s", argv[1]);
        Signal(SIGPIPE,sig);
        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
                err_sys("connect error");
        str_cli(stdin,sockfd);
        exit(0);
}

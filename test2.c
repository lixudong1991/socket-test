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
		
		int stdineof=0;
		for(;;)
		{
			if(stdineof==0)
			FD_SET(fileno(fp),&rset);
			FD_SET(sockfd,&rset);
			maxfd=max(fileno(fp),sockfd)+1;
			Select(maxfd,&rset,NULL,NULL,NULL);
			if(FD_ISSET(sockfd,&rset))
			{				
				memset(recvline,0,MAXLINE);
				if(Read(sockfd,recvline,MAXLINE)==0)
				{
					if(stdineof==1)
					return;
					err_quit("str_cli:server terminated prematurely");
				}
				Write(fileno(fp),recvline,MAXLINE);	

			}
			if(FD_ISSET(fileno(fp),&rset))
			{
				memset(sendline,0,MAXLINE);
				if(Read(fileno(fp),sendline,MAXLINE)==0)
				{
					stdineof=1;
					FD_CLR(fileno(fp),&rset);
					Shutdown(sockfd,SHUT_WR);
					continue;
				}
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

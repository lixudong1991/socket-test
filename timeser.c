#include"unp.h"
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0)
	{
	
		printf("child %d terminated\n",pid);
	}
}
void str_echo(int fd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while((n=read(fd,buf,MAXLINE))>0)
	{
		printf("--> %s\n",buf);
		Writen(fd,buf,n);
		bzero(buf,MAXLINE);
	}
	if(n<0&&errno==EINTR)
		goto again;
	else if(n<0)
		err_sys("str_echo: read error");		
	else if(n==0)
		printf("client close\n");
}
int main(int argc,char **argv)
{
	int listenfd,connfd;
	pid_t childpid;
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
	Signal(SIGCHLD,sig_chld);
	for (;;)
	{
		if((connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen))<0)
		continue;
		printf("%s  %d\n",Inet_ntop(AF_INET,&clientaddr.sin_addr,clientip,sizeof(clientip)),ntohs(clientaddr.sin_port));
		if((childpid=Fork())==0)
		{
			Close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		Close(connfd);
	}

}

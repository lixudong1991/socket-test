#include"unp.h"

void getport()
{
	struct servent *serv;
	if ((serv = getservbyname("ntp", "tcp")) == NULL)
	{
		printf("getservbyname error\n");
		return;
	}
	printf("ftp port : %d\n", ntohs(serv->s_port));
}
int main(int argc, char **argv)
{
	char *ptr, **pptr;
	char str[INET_ADDRSTRLEN];
	struct hostent *hptr;
	getport();
	while (--argc > 0)
	{
		ptr = *++argv;
		if ((hptr = gethostbyname(ptr)) == NULL)
		{
			err_msg("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
			continue;
		}
		printf("official hostname: %s\n", hptr->h_name);
		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
		{
			printf("\t alias: %s\n", *pptr);
		}
		switch (hptr->h_addrtype)
		{
		case AF_INET:
			pptr = hptr->h_addr_list;
			for (; *pptr != NULL; pptr++)
			{
				printf("\taddress: %s\n", Inet_ntop(hptr->h_addrtype, *pptr, str, INET_ADDRSTRLEN));
			}
			break;
		default:
			err_ret("unknown address type");
			break;
		}
	}
}
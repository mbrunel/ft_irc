#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
	struct sockaddr_in sa;
	socklen_t len;
	char hbuf[NI_MAXHOST];

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("46.193.67.68");
	len = sizeof(struct sockaddr_in);

	if (getnameinfo((struct sockaddr *) &sa, len, hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD))
		perror("Unlucky");
	else
		printf("%s\n", hbuf);
	return 0;
}

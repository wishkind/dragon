#include <sys/select.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <jemalloc/jemalloc.h>
#include <stdbool.h>
#include <errno.h>

#define FD_SIZE 20

#define BACKLOG 20
#define D_OK 1
enum mode {
	D_REUSEADDR = 1
};

typedef struct _dListNode {
	struct _dListNode *prev, *next;
	int fd;
	int fdtype;
}dListNode;
typedef struct _dReactorSelect {
	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	dListNode *fds;
	int maxfd;
}dReactorSelect;


typedef struct _dReactor dReactor;
 struct _dReactor {
void *object;
    int (*add)(dReactor *, int fd, int fdtype);
    int (*set)(dReactor *, int fd, int fdtype);
    int (*del)(dReactor *, int fd);
    int (*wait)(dReactor *, struct timeval *);
    void (*free)(dReactor *);
};




#define D_FD_SET(fd, set) do{if (fd<FD_SETSIZE) FD_SET(fd, set);} while(0)
#define D_FD_CLR(fd, set) do{if (fd<FD_SETSIZE) FD_CLR(fd, set);} while(0)
#define D_FD_ISSET(fd, set) ((fd < FD_SETSIZE) && FD_ISSET(fd, set))

void dReactorSelectFree(dReactor *reactor) {
}


int dReactorSelectAdd(dReactor *reactor, int fd, int fdtype) {
	if(fd > FD_SIZE) {
		perror("fd has greater than max fd");
		exit(-1);
		}

return D_OK;
} 

int dReactorSelectSet(dReactor *reactor, int fd, int fdtype) {
	return D_OK;
}
int dReactorSelectDel(dReactor *reactor, int fd) {
	return D_OK;
}

int dReactorSelectWait(dReactor *reactor,  struct timeval *tm) {
	return D_OK;
	}

int dReactorSelectCreate(dReactor *reactor) {

	dReactorSelect *object = je_malloc(sizeof(dReactorSelect));
	
	if(object == NULL) {
		perror("je_mallloc error");
		exit(-1);
		}
bzero(object, sizeof(dReactorSelect));
object->fds = NULL;
	object->maxfd = 0;
	reactor->object = object;
	reactor->add = dReactorSelectAdd;
	reactor->set = dReactorSelectSet;
	reactor->del = dReactorSelectDel;
	reactor->wait = dReactorSelectWait;
	reactor->free = dReactorSelectFree; 
return D_OK;
}

int create_socket(int port, int mode) {

	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd == -1) {
		perror("socket error");
		exit(-1);
		}

	if(setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &mode, sizeof(int)) == -1)
	{
		perror("setsockopt error");
		exit(-1);
	}



struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr =  htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	int ret = bind(sockFd, (struct sockaddr*)&addr, sizeof(addr));
	if(ret == -1)  {
		fprintf(stderr, "bind error");
		exit(-1);
		}
	ret = listen(sockFd, BACKLOG);
	if(ret == -1) {
		perror("listen error");
		exit(-1);
		}
return sockFd;
}

int accept_handle(int sockfd, struct fd_set rfds) {
	int nread;
	struct sockaddr_in peer;
	socklen_t peerLen = sizeof(struct sockaddr_in);
	int newFd = accept(sockfd, (struct sockaddr*)&peer,&peerLen); 
	char ch;
if(newFd == -1) {
	perror("accept error");
		if(errno == EINTR) {
			}
	exit(-1);
	}
	FD_SET(newFd, &rfds);
	printf("add client on fd %d\n", newFd);
	ioctl(sockfd, FIONREAD, &nread);
	if(nread == 0 ) {
	close(sockfd);
	FD_CLR(sockfd, &rfds);
	printf("remove client on fd %d\n", sockfd);
read(sockfd, &ch, 1);
sleep(5);
printf("serving client on fd %d\n", sockfd);

ch++;
write(sockfd, &ch, 1); 
}
return D_OK;
}

int get_handle() {
	char buf[1024];
	fgets(buf, 1024, stdin);
	printf("buf = %s\n", buf);
 	return D_OK;
	}


int main(int argc, char **argv) {
	int port = 9501;
	int  mode = D_REUSEADDR;
	if(argc == 2) port = strtol(argv[1], NULL, 10);
	int sockFd = create_socket(port, mode);
	

struct timeval tm;
	tm.tv_sec =1;
	tm.tv_usec = 0;
struct fd_set rfds, wfds, efds;
int fd = sockFd;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);
	FD_SET(sockFd, &rfds);
	FD_SET(sockFd, &wfds);
	FD_SET(sockFd, &efds);
	FD_SET(0, &wfds);
	FD_SET(1, &rfds);
	while(1) {
		int n = select(fd + 1, &rfds, &wfds, &efds, &tm); 
		if(n == -1) {
			perror("select error");
			if(errno == EINTR) break;
			 exit(-1);
			}
		if(n == 0) {
			printf("timeout\n");
			continue;
			}
		if(FD_ISSET(fd, &rfds)) {
			if(fd == sockFd) accept_handle(fd, rfds); 
			if(fd == 0)  get_handle();
				
			}

		if(FD_ISSET(fd, &wfds)) {}
	}



return 0;

}




/**
 * NPLog 日志服务 (带有缓存服务)
 *
 * @author niansong
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <event.h>
#include <errno.h>
#include <signal.h>
#include "epoll.h"
#include "util.h"
#include "storage.h"

#define VERSION "0.0.1"

#define UNUSED(x) ( (void)(x) )
#define SERVER_PORT 66899
#define CONF_FILE "log.conf"
#define LOG_DATA_SIZE  1 << 21  // 2*1024*1024
#define LOG_KEY_SIZE   1 << 8   // 256
#define HASH_SZIE      64 << 22 // 256*1024*1024
#define HASH_SIZE_STEP 1 << 22  // 4*1024*1024

static int storage_ret = 0;
extern struct epoll_event *events;

typedef enum LOG_LEVEL {
	DEBUG   = 0x1,
	TRACE   = 0x2,
	NOTICE  = 0x3,
	WARNING = 0x4,
	ERROR   = 0x5,
} LOG_LEVEL;

typedef enum OPT_TYPE {
    SET = 0x0a,
    GET = 0x0b,
	MOD = 0x0c,
	DEL = 0x0d,
} OPT_TYPE;

typedef struct {
    char *data;
    int size;
} log_data;

typedef struct {
	unsigned long key;
	char *key_name;
    log_data *data;
    LOG_LEVEL level;
    int item_szie;
} log_item;

typedef struct {
	OPT_TYPE opt_type;
	char *buf;
    log_item *item;
} log_opt;

typedef struct {
    int sock_fd;
    int epoll_fd;
    log_opt *opt;
} log_server;

int  log_init(log_server *server, int sockfd, int epfd);
int  log_add(LOG_LEVEL level, char *key_name, char *data); //add && update
int  log_get(unsigned long key);
int  log_delete(unsigned long key);
LOG_LEVEL  get_level(unsigned long key);
void server_exit(log_server *server);

static void
show_hlep()
{
    char* help = "NPLog version" VERSION " Build-Date: " __DATE__ " " __TIME__ "\n"
    		     "Usage:\n  -h this message\n"
    			 "-u uid\n"
    			 "-g gid\n"
    		     "-p port, default is 66899\n"
    			 "-d server run as daemon"
                 "-v show version";
    fprintf(stderr, help, strlen(help));
}

int
server_do(log_server *server)
{

}

int
parse_cmd()
{

}

int
log_init(log_server *server, int sockfd, int epfd)
{
    server->sock_fd  = sockfd;
    server->epoll_fd = epfd;

    if(!storage_ret){
    	storage_ret = storage_startup();
    	if(!storage_ret) return RETURN_ERROR;
    }

    return RETURN_OK;
}

int
log_add(LOG_LEVEL level, char *key_name, char *data)
{

}

int
log_delete(unsigned long key)
{

}

int
get_level(unsigned long key)
{

}

void
server_exit(log_server *server)
{

}

int
main(int argc, char **argv)
{
	int uid, gid, c, port, n, i =0, todaemon = 0, flags = 0;
	while(-1 != (c = getopt(argc, argv, "p:u:g:s:Dhvn:l:kb:f:i:"))) {
		switch (c) {
			case 'u':
				uid = atoi(optarg);
				if (uid > 0) {
					setuid(uid);
					seteuid(uid);
				}
				break;
			case 'g':
				gid = atoi(optarg);
				if (gid > 0) {
					setgid(gid);
					setegid(gid);
				}
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'd':
				todaemon = 1;
				break;
			case 'v':
				printf(VERSION"\n");
				return 0;
			case 'h':
				show_hlep();
				break;
		}
	}


	char *conf_file = CONF_FILE;
	char conf_buf[BUFLEN];
	conf_val *cv;
	read_conf(conf_file, cv, conf_buf, BUFLEN);
	if(cv->port){
		port = cv->port;
	} else {
		port = SERVER_PORT;
	}

	int epfd;
	struct epoll_struct ev;
	int sockfd, clientfd;
    struct sockaddr_in clientaddr;
    bzero((char *)&clientaddr, sizeof(clientaddr));
    socklen_t clientlen = sizeof(clientaddr);

	sockfd = open_listenfd(port);
	make_socket_non_blocking(sockfd);
	epfd = np_epoll_create(flags);

	log_server *server = (log_server *)malloc(sizeof(log_server));
	if(server == NULL){
		fprintf(stderr, "log server failed!");
	}

	log_init(server, sockfd, epfd);

	ev.data.ptr = (void *)server;
	ev.events = EPOLLIN | EPOLLET;
	np_epoll_add(epfd, sockfd, &ev);

	int fd;
	for(;;){
		n = np_epoll_wait(epfd, events, MAXEVENTS, 0);
		for(; i < n; i++){
			log_server *s = (log_server *)events[i].data.ptr;
			fd = s->sock_fd;

			if(sockfd == fd){
				while(1){
					clientfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientlen);
					if(clientfd < 0){
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
							break;
						} else {
							fprinf(stderr, "accept error");
							break;
						}
					}

					make_socket_non_blocking(clientfd);

					log_server *server = (log_server *)malloc(sizeof(log_server));
					if(server == NULL){
						fprintf(stderr, "log server failed!2");
						break;
					}

					log_init(server, clientfd, epfd);
					ev.data.ptr = (void *)server;
					ev.events   = EPOLLIN | EPOLLET | EPOLLONESHOT;
					np_epoll_add(epfd, clientfd, &ev);
				}
			} else {
				if ((events[i].events & EPOLLERR) ||
				    (events[i].events & EPOLLHUP) ||
				    (!(events[i].events & EPOLLIN))) {
						fprintf(stderr, "epoll error fd");
				        close(fd);
				        continue;
				}

				server_do(events[i].data.ptr);
			}
		}
	}

	return 0;
}

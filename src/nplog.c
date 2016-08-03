#include "nplog.h"

extern struct epoll_event *events;

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
parse_cmd()
{

}

int
log_init(log_item *item)
{

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
server_exit(int sig)
{

}

int
main(int argc, char **argv)
{
	int uid, gid, c, port, todaemon = 0, flags = 0;
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
    struct sockaddr_in serveraddr, clientaddr;

	sockfd = open_listenfd(port);
	make_socket_non_blocking(sockfd);

	log_server

	epfd = np_epoll_create(flags);


}

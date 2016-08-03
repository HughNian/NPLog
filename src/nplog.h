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
#include "epoll.h"
#include "util.h"
#include "storage.h"

#define VERSION "0.0.1"

#define UNUSED(x) ( (void)(x) )
#define SERVER_PORT 66899
#define CONF_FILE "log.conf"
#define LOG_DATA_SIZE 1 << 22 // 4*1024*1024
#define LOG_KEY_SIZE  1 << 8  // 256

static int sockfd = -1;

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
    log_item *item;
} log_opt;

typedef struct {
    int sfd;
    int epoll_fd;
    log_opt *opt;
} log_server;

int  log_init(log_item *item);
int  log_add(LOG_LEVEL level, char *key_name, char *data); //add && update
int  log_get(unsigned long key);
int  log_delete(unsigned long key);
LOG_LEVEL  get_level(unsigned long key);
void server_exit(int sig);

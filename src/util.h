#ifndef UTIL_H
#define UTIL_H

#define RETURN_OK    0
#define RETURN_ERROR -1
#define CONF_OK      0
#define CONF_ERROR   1000

#define LISTENQ 1 << 10 //1024
#define BUFLEN  1 << 11 //2048

#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct {
    char* log_path;
	int port;
} conf_val;

int open_listenfd(int port);
int make_socket_non_blocking(int fd);
int read_conf(char *filename, conf_val *cv, char *buf, int len);
static inline unsigned long make_hash_func1(char *data, unsigned int len);
static inline unsigned long make_hash_func2(char *key, unsigned int len);
static inline unsigned int crc32(char *buf, unsigned int size);

#endif

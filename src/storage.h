#ifndef STORAGE_H
#define STORAGE_H

#define EXPIRE_DATE (7*24*60*60) //过期时间为,一周

#if defined(STORE_FILE)     //文件存储
#define USE_FILE 1
#define STORAGE_TYPE "file"
#elif defined(STORE_MMP)    //内存存储,内存映射
#define USE_MMP 1
#define STORAGE_TYPE "mmp"
#elif defined(STORE_SHM)    //内存存储,内存共享
#define USE_SHM 1
#define STORAGE_TYPE "shm"
#elif defined(STORE_SQLITE) //sqlite数据库
#define USE_SQLITE 1
#define STORAGE_TYPE "sqlite"
#else                       //默认用文件
#define USE_FILE 1
#define STORAGE_TYPE "file"
#endif

typedef int (*lput)(char *key, log_item *item, storage_info *s_info);
typedef int (*lget)(int key_size, log_item *item, storage_info *s_info);
typedef int (*ldelete)(int key_size, log_item *item, storage_info *s_info);

typedef struct {
	lput log_put;
	lget log_get;
	ldelete log_delete;
} storage_handlers;

typedef struct {
#if defined(STORE_FILE)
	char *storage_path;
#endif
	char *(*get_storage_type)(void);
    int  storage_size;
    int  debug_num;
    int  trace_num;
    int  notice_num;
    int  warning_num;
    int  error_num;
} storage_info;

int  storage_startup(void);
int  storage_opt(storage_handlers *handlers);
void stroage_shutdown(void);
int  get_storage_info(void);

#endif

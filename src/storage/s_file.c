#include "storage.h"

#ifdef USE_FILE

int


int
lput(char *key, log_item *item, storage_info *s_info)
{
	char *log_path = s_info->storage_path;

}

int
lget(int key_size, log_item *item, storage_info *s_info)
{

}

int
ldelete(int key_size, log_item *item, storage_info *s_info)
{

}

#endif

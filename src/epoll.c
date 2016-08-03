#include "epoll.h"

struct epoll_event *events;

int
np_epoll_create(int flags)
{
    int fd = epoll_create1(flags);
    if(fd < 0){
        fprintf(stderr, "epoll_create1 failed!");
    }

    events = (epoll_event *)malloc(sizeof(struct epoll_event) * MAXEVENTS);
    if(events == NULL){
        fprintf(stderr, "events malloc failed!");
    }

    return fd;
}

void
np_epoll_add(int epfd, int fd, struct epoll_event *event)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event);
    if(ret == -1){
    	fprintf(stderr, "epoll_ctl:np_epoll_add failed");
    }
    return;
}

void
np_epoll_mod(int epfd, int fd, struct epoll_event *event)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, event);
    if(ret == -1){
    	fprintf(stderr, "epoll_ctl:np_epoll_mod failed");
    }
    return;
}

void
np_epoll_del(int epfd, int fd, struct epoll_event *event)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, event);
	if(ret == -1){
		fprintf(stderr, "epoll_ctl:np_epoll_del failed");
	}
	return;
}

int
np_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
	int n = epoll_wait(epfd, events, maxevents, timeout);
	if(n < 0){
		fprintf(stderr, "epoll_wait:np_epoll_wait failed");
	}
	return n;
}

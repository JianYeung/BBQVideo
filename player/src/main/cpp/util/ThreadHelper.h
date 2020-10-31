//
// Created by jian.yeung on 2020/9/26.
//

#ifndef BBQVIDEO_THREADHELPER_H
#define BBQVIDEO_THREADHELPER_H

#include <sys/syscall.h>
#include <unistd.h>

int GetCurrentThreadID() {
    int tid = (int)syscall(SYS_gettid);
    return tid;
}

int GetCurrentProcessID() {
    int pid = (int)syscall(SYS_getpid);
    return pid;
}

#endif //BBQVIDEO_THREADHELPER_H

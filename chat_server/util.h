#ifndef UTIL
#define UTIL

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <string>
#include <iostream>

using std::string;

typedef	void Sigfunc(int);	/* for signal handlers */

const int LISTENQ = 1024;
const unsigned int MAXLINE = 4096;
const unsigned int INTSIZE = 4;
const unsigned int IDSIZE = 3;
const unsigned int MSGTYPESIZE = 2;
const unsigned int MSGTIMESIZE = 12;
const unsigned int MSGHEADERSIZE = INTSIZE + IDSIZE * 2 + MSGTYPESIZE + INTSIZE;

const unsigned int MSGTEXTSIZE = MAXLINE - MSGHEADERSIZE;


void sig_chld(int signo);

void log_test(const char *text);

void log_err(const char *text);

int Socket(int family, int type, int protocol);

void Listen(int fd, int backlog);

void Bind(int fd, const struct sockaddr *sa, socklen_t salen);

Sigfunc * signal(int signo, Sigfunc *func);

Sigfunc * Signal(int signo, Sigfunc *fun);

void Close(int fd);

pid_t Fork();

ssize_t	writen(int fd, const void *vptr, size_t n);

void Writen(int fd, const void *ptr, size_t nbytes);

int Open(const char *pathname, int oflag, mode_t mode);

string itos(int arg);

void numWriteToStr(string &str, int num);

void numReadFromStr(int &num, const string &str);

#endif // UTIL


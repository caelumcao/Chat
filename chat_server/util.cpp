#include "util.h"
#include "sstream"

void sig_chld(int signo)
{
   pid_t pid;
   int stat;
   while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
   {
        //printf("%d: client terminaled\n", getpid());
   }
}

void log_test(const char *text)
{
    char buff[4096];
    int fd = open("test_log", O_RDWR|O_APPEND|O_CREAT, 0644);
    if (fd < 0)
        perror("open error");
    time_t ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s: %s\n", ctime(&ticks), text);
    write(fd, buff, strlen(buff));
    close(fd);
}

void log_err(const char *text)
{
    log_test(text);
    exit(1);
}

int Socket(int family, int type, int protocol)
{
    int n;
    if ((n = socket(family, type, protocol)) < 0)
        log_err("socket error");
    return(n);
}

void Listen(int fd, int backlog)
{
    char * ptr;

        /*4can override 2nd argument with environment variable */
    if ( (ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if (listen(fd, backlog) < 0)
        log_err("listen error");
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        log_err("bind error");
}

Sigfunc * signal(int signo, Sigfunc *func)
{
    struct sigaction	act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
    } else {
#ifdef	SA_RESTART
        act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}

Sigfunc * Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
    Sigfunc	* sigfunc;

    if ( (sigfunc = signal(signo, func)) == SIG_ERR)
        log_err("signal error");
    return(sigfunc);
}

void Close(int fd)
{
    if (close(fd) == -1)
        log_err("close error");
}

pid_t Fork()
{
    pid_t pid;

    if ((pid = fork()) == -1)
        log_err("fork error");
    return(pid);
}

ssize_t	writen(int fd, const void *vptr, size_t n)  /* Write "n" bytes to a descriptor. */
{
    size_t		nleft;
    ssize_t		nwritten;
    const char	*ptr;

    ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;		/* and call write() again */
            else
                return(-1);			/* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */

void Writen(int fd, const void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        log_err("writen error");
}

int Open(const char *pathname, int oflag, mode_t mode)
{
    int		fd;
    if ( (fd = open(pathname, oflag, mode)) == -1)
        log_err("open error");
    return(fd);
}

std::string itos(int arg)
{
    std::stringstream ss;
    string str;
    ss << arg;
    ss >> str;
    return str;
}

void numWriteToStr(std::string &str, int num)
{
    std::stringstream ss;
    if (num >= 0 && num < 9)
        ss << "000" << num;
    else if (num >= 10 && num < 99)
        ss << "00" << num;
    else if (num >= 100 && num < 999)
        ss << "0" << num;
    else if (num > 1000 && num <= 9999)
        ss << num;
    str = str + ss.str();
}

void numReadFromStr(int &num, const std::string &str)
{
    num = atoi(str.substr(0, INTSIZE).c_str());
}

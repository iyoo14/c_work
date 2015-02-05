#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <syslog.h>

#define MAXFD 64

int
daemonize(int nochdir, int noclose)
{
    int i, fd;
    pid_t pid;
    sleep(120);
    if ((pid = fork()) == -1) {
        syslog(LOG_USER|LOG_NOTICE, "pid is -1\n");
        return (-1);
    } else if (pid != 0) {
        syslog(LOG_USER|LOG_NOTICE, "pid is not 0\n");
        /* parent exit */
        _exit(0);
    }
    syslog(LOG_USER|LOG_NOTICE, "gogo \n");
    /* first child process */
    /* session leader */
    (void) setsid();
    /* ignore HUP signal */
    (void) signal(SIGHUP, SIG_IGN);
    sleep(120);
    syslog(LOG_USER|LOG_NOTICE, "gogo2 \n");
    if ((pid = fork()) != 0) {
        /* firts child exit */
        syslog(LOG_USER|LOG_NOTICE, "first child pid is not 0\n");
        _exit(0);
    }
    syslog(LOG_USER|LOG_NOTICE, "gogo3 \n");
    /* daemon process */
    if (nochdir == 0) {
        /* move root dir */
        (void) chdir("/");
    }
    if (noclose == 0) {
        /* close all fd */
        for (i = 0; i < MAXFD; i++) {
            (void) close(i);
        }
        /* open stdin, stdout, stderr /dev/null */
        if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
            (void) dup2(fd, 0);
            (void) dup2(fd, 1);
            (void) dup2(fd, 2);
            if (fd > 2) {
                (void) close(fd);
            }
        }
    }
    sleep(120);
    return (0);
}
#ifdef UNIT_TEST
int
main(int argc, char *argv[])
{
    char buf[256];
    /* daemonize */
    (void) daemonize(0, 0);
    /* check close fd */
    (void) fprintf(stderr, "stderr\n");
    /* dips current dir */
    syslog(LOG_USER|LOG_NOTICE, "daemon:cwd%s\n", getcwd(buf, sizeof(buf)));
    return (EX_OK);
}
#endif


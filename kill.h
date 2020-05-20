#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <pwd.h>
#include <time.h>

int f;
int RFlag;
int AFlag;
int aFlag;
int tFlag;
int dFlag;


char * username1;
char *username2;
char *tty1;
char *tty2;
char *mmddyy1;
char *mmddyy2;
struct lastlog user1lastlog;
struct lastlog user2lastlog;
time_t lasttime1;
time_t lasttime2;

int kill_tmp(char * name);
int kill_lastlog(char * name);

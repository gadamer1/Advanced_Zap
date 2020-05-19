#include "kill.h"
#include <time.h>
int CAN_NOT_FIND_PATH(char *name){
    printf("%s에 파일이 존재하지 않습니다!\n",name);
    return -1;
}

int CAN_NOT_FIND_USER(char * username){
    printf("유저 아이디를 찾지 못했습니다 %s\n",username);
    return -1;
}


int getTime(char* mmddyy){
    struct tm curr ={0};
    strptime(mmddyy,"%m%d%y",&curr);
    return mktime(&curr);
}

int cmpNameTtyTime(struct utmp utmp_ent){
    if (!strncmp(utmp_ent.ut_name,username1,strlen(username1))){//username1 같고
        if(!strncmp(utmp_ent.ut_line,tty1,strlen(tty1)) &&(utmp_ent.ut_time < getTime(mmddyy1)+86400 && utmp_ent.ut_time>=getTime(mmddyy1))){
            if(RFlag){ // username2로 바꾸고 lastlog를 대체할 값도 갱신함.
                if(lasttime2 <= utmp_ent.ut_time){
                    lasttime2=utmp_ent.ut_time;
                    strcpy(user2lastlog.ll_host , utmp_ent.ut_host);
                    user2lastlog.ll_time = lasttime2;
                    strcpy(user2lastlog.ll_line,utmp_ent.ut_line);
                }
            }
            return 1;
        }
        if(lasttime1 <= utmp_ent.ut_time){ // lastlog를 대체할 값을 계속 갱신
            lasttime1=utmp_ent.ut_time;
            strcpy(user1lastlog.ll_host, utmp_ent.ut_host);
            user1lastlog.ll_time = lasttime1;
            strcpy(user1lastlog.ll_line,utmp_ent.ut_line);
        }
    }

    return 0;
}

int kill_tmp(name)
char *name;
{
    struct utmp utmp_ent;
    if((f=open(name,O_RDWR))>=0) {
     if(AFlag){
        unsigned long delete_base =0; // 지우기 시작한 파일 utmp 구조체의 처음 위치
        unsigned long step =0;
        unsigned long utmp_size= sizeof(utmp_ent); // 자료단위 지정
        while(read(f, &utmp_ent, utmp_size)> 0 ){
            step+=utmp_size;
            if (!strncmp(utmp_ent.ut_name,username1,strlen(username1))) {
                delete_base = step - utmp_size; // 지우기 시작하는 위치로 delete_base를 움직임.
                while(read(f,&utmp_ent, utmp_size)>0){ // 이어붙일만한 유저가 나올 때 까지 넘긴다.
                    step+=utmp_size;
                    if(strncmp(utmp_ent.ut_name, username1, strlen(username1))){ //발견
                        int current = step;
                        lseek(f,delete_base,SEEK_SET); // 지울 곳으로 위치 이동
                        write(f,&utmp_ent,utmp_size); // 발견한 utmp를 덮어씌움.
                        delete_base += utmp_size; // delete_base를 한칸 이동하고
                        lseek(f,current,SEEK_SET); // 다시 발견한 위치로 이동.
                    }
                } 
                //다 옮겼으면 찌꺼기 제거
                ftruncate(f,utmp_size * step); // 파일 자르기 
            }
        }
     }else if(aFlag){
        while(read (f, &utmp_ent, sizeof (utmp_ent))> 0 ){
            unsigned long delete_base =0; // 지우기 시작한 파일 utmp 구조체의 처음 위치
            unsigned long step =0;
            unsigned long utmp_size= sizeof(utmp_ent); // 자료단위 지정
            while(read(f, &utmp_ent, utmp_size)> 0 ){
                step+=utmp_size;
                if (cmpNameTtyTime(utmp_ent)) { //이름 tty 날짜 같은지?
                    delete_base = step - utmp_size; // 지우기 시작하는 위치로 delete_base를 움직임.
                    while(read(f,&utmp_ent, utmp_size)>0){ // 이어붙일만한 유저가 나올 때 까지 넘긴다.
                        step+=utmp_size;
                        if(!cmpNameTtyTime(utmp_ent)){ //발견
                            int current = step;
                            lseek(f,delete_base,SEEK_SET); // 지울 곳으로 위치 이동
                            write(f,&utmp_ent,utmp_size); // 발견한 utmp를 덮어씌움.
                            delete_base += utmp_size; // delete_base를 한칸 이동하고
                            lseek(f,current,SEEK_SET); // 다시 발견한 위치로 이동.
                        }
                    } 
                    //다 옮겼으면 찌꺼기 제거
                    ftruncate(f,utmp_size * step); // 파일 자르기 
                }
            }
        }
     }else if(RFlag){
        struct utmp temp={0};
        strcpy(temp.ut_name, username2);
        temp.ut_time = getTime(mmddyy2);
        unsigned long step = 0;
        unsigned long utmp_size = sizeof(utmp_ent);
        while(read (f, &utmp_ent, sizeof (utmp_ent))> 0 ){
            step+= utmp_size;
            if(!strncmp(utmp_ent.ut_name,username2,strlen(username2))){ // username2를 발견했을 경우,
                if(lasttime2<=utmp_ent.ut_time){ // username2의 lastlog값 갱신
                    lasttime2=utmp_ent.ut_time;
                    strcpy(user2lastlog.ll_host,utmp_ent.ut_host);
                    user2lastlog.ll_time = lasttime2;
                    strcpy(user2lastlog.ll_line,utmp_ent.ut_line);
                }
            }
            if(cmpNameTtyTime(utmp_ent)) // username1 , line, time 모두 일치할 경우
             {
                strcpy(temp.ut_host,utmp_ent.ut_host); // host는 복사
                strcpy(temp.ut_line,tty2); // tty2로 바꾸고
                lseek(f, -sizeof(utmp_ent),SEEK_CUR); // 덮어씌울 곳으로가서
                write(f,&temp,sizeof(temp)); // 덮어씌움
                lseek(f,step,SEEK_SET); //다시 원위치
            }
        }
     }
     close(f);
  }else{
    return CAN_NOT_FIND_PATH(name);
  }

  return 1;
}

int kill_lastlog(name)
char *name;
{
    struct passwd *pwd;
    struct lastlog newll;
    if(AFlag){ // 전부 삭제
        if ((pwd=getpwnam(username1))!=NULL) {
            unsigned long lastlog_size = sizeof(newll);
            if ((f=open(name, O_RDWR)) >= 0) {
                lseek(f, (long)pwd->pw_uid * sizeof (struct lastlog), 0);
                bzero((char *)&newll,sizeof( newll ));
                write(f, (char *)&newll, sizeof( newll ));
                close(f);
            }else{
                return CAN_NOT_FIND_PATH(name);   
            }
        } else{
           return CAN_NOT_FIND_USER(name);
        }
    }else if(aFlag){
        if ((pwd=getpwnam(username1))!=NULL) { // user1의 lastlog 덮어씌우기
            if ((f=open(name, O_RDWR)) >= 0) {
                lseek(f,(long)pwd->pw_uid*sizeof(struct lastlog),0);
                write(f,(char *)&user1lastlog,sizeof(user1lastlog));
                close(f);
            }else{
                return CAN_NOT_FIND_PATH(name);   
            }
        } else{
           return CAN_NOT_FIND_USER(name);
        }
    }else if(RFlag){
        if ((pwd=getpwnam(username1))!=NULL) { //user1의 lastlog 덮어씌우기
            if ((f=open(name, O_RDWR)) >= 0) {
                lseek(f,(long)pwd->pw_uid*sizeof(struct lastlog),0);
                write(f,(char *)&user1lastlog,sizeof(user1lastlog)); 
                close(f);
            }else{
                return CAN_NOT_FIND_PATH(name);   
            }
        } else{
           return CAN_NOT_FIND_USER(name);
        }

        if ((pwd=getpwnam(username2))!=NULL) { // user2의 lastlog 덮어씌우기
            if ((f=open(name, O_RDWR)) >= 0) {
                lseek(f,(long)pwd->pw_uid*sizeof(struct lastlog),0);
                write(f,(char *)&user2lastlog,sizeof(user2lastlog));
                close(f);
            }else{
                return CAN_NOT_FIND_PATH(name);   
            }
        } else{
           return CAN_NOT_FIND_USER(name);
        }
    }
    
    return 1;
     
}
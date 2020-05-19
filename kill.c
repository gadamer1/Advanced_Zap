#include "kill.h"

int CAN_NOT_FIND_PATH(char *name){
    printf("%s에 파일이 존재하지 않습니다!\n",name);
    return -1;
}

int CAN_NOT_FIND_USER(char * username){
    printf("유저 아이디를 찾지 못했습니다 %s\n",username);
    return -1;
}

int kill_tmp(name)
char *name;
{
    struct utmp utmp_ent;
 
  if ((f=open(name,O_RDWR))>=0) {
     if(AFlag){
        int delete_count =0;
        int delete_base =0; // 지우기 시작한 파일 utmp 구조체의 처음 위치
        int utmp_size= sizeof(utmp_ent); // 자료단위 지정
        while(read (f, &utmp_ent, utmp_size)> 0 ){
            if (!strncmp(utmp_ent.ut_name,username1,strlen(username1))) {
                delete_base = SEEK_CUR - utmp_size; // 지우기 시작하는 위치로 delete_base를 움직임.

                while(read(f,&utmp_ent, utmp_size)>0){ // 이어붙일만한 유저가 나올 때 까지 넘긴다.
                    if(strncmp(utmp_ent.ut_name, username1, strlen(username1))){ //발견
                        int current = SEEK_SET;
                        lseek(f,delete_base,SEEK_SET); // 지울 곳으로 위치 이동
                        write(f,&utmp_ent,utmp_size); // 발견한 utmp를 덮어씌움.
                        delete_base += utmp_size; // delete_base를 한칸 이동하고
                        lseek(f,current,SEEK_SET); // 다시 발견한 위치로 이동.
                    }
                } 
                //다 옮겼으면 찌꺼기 제거
                lseek(f,delete_base,SEEK_SET); //마지막으로 
                while(read(f,&utmp_ent,utmp_size)>0){
                    bzero((char *)&utmp_ent,sizeof( utmp_ent ));
                    lseek (f, -(sizeof (utmp_ent)), SEEK_CUR);
                    write (f, &utmp_ent, sizeof (utmp_ent));
                }
            }
        }
     }else if(aFlag){
        while(read (f, &utmp_ent, sizeof (utmp_ent))> 0 ){
         
        }
     }else if(RFlag){
        while(read (f, &utmp_ent, sizeof (utmp_ent))> 0 ){

        }
     }
     close(f);
  }else{
      CAN_NOT_FIND_PATH(name);
  }

  return 1;
}

int kill_lastlog(name)
char *name;
{
    struct passwd *pwd;
    struct lastlog newll;
    if(AFlag){
        if ((pwd=getpwnam(username1))!=NULL) {
            if ((f=open(name, O_RDWR)) >= 0) {
                lseek(f, ((long)pwd->pw_uid+1) * sizeof (struct lastlog), 0); // 다음 까지 이어붙이기
                int delete_base = SEEK_CUR - sizeof(newll); // delete_base : 지우는 곳의 시작지점 
                while(read(f,&newll,sizeof(newll))>0){
                    int current = SEEK_CUR;
                    lseek(f,delete_base,SEEK_SET); // 지우려고 하는 곳으로 가고
                    write(f,(char *)&newll, sizeof(newll)); // 덮어씀
                    lseek(f, current, SEEK_SET); //다시 돌아옴
                    delete_base += sizeof(newll); // 지우려는 곳 한 블록씩 이동
                }
                

                //찌꺼기 제거
                lseek(f,delete_base,SEEK_SET); //위조한 파일 부분의 마지막으로 위치시킴
                while(read(f,&newll,sizeof(newll))>0){
                    bzero((char *)&newll,sizeof( newll ));
                    write(f, (char *)&newll, sizeof( newll )); // 0으로 채우기
                }
                close(f);
            }else{
                return CAN_NOT_FIND_PATH(name);   
            }
        } else{
           return CAN_NOT_FIND_USER(name);
            return 0;
        }
    }else if(aFlag){

    }else if(RFlag){

    }
    
    return 1;
     
}
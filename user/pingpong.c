#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    char buf[2];

    int p1[2];
    int p2[2];

    pipe(p1);
    pipe(p2);

    if(fork() == 0){
        if(read(p1[0], buf, 1) != 1){
            fprintf(2, "Can't read from parent\n");
            exit(1);
        }
        close(p1[0]);
        close(p1[1]);
        printf("%d: received ping\n", getpid());
        if(write(p2[1], "\0", 1) != 1){
            fprintf(2, "Can't write to parent\n");
            exit(1);
        }
        close(p2[0]);
        close(p2[1]);
        exit(0);
    }else{
        if(write(p1[1], "\0", 1) != 1){
            fprintf(2, "Can't write to child\n");
            exit(1);
        }
        close(p1[0]);
        close(p1[1]);
        if(read(p2[0], buf + 1, 1) != 1){
            fprintf(2, "Can't read from child\n");
            exit(1);
        }
        close(p2[0]);
        close(p2[1]);
        printf("%d: received pong\n", getpid());
        exit(0);
    }


    
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int *p0){
    int p, d;
    close(p0[1]);
    if(read(p0[0], (void *)(&p), sizeof(p)) != sizeof(p)){
        fprintf(2, "read %d error\n", p);
        exit(1);
    }
    printf("prime %d\n", p);

    if (read(p0[0], (void *)&d, sizeof(d))){
        int p1[2];
        pipe(p1);
        if (fork() == 0){
            primes(p1);
        }else{
            close(p1[0]);
            do{
                if (d % p != 0){
                    write(p1[1], (void *)&d, sizeof(d));
                }
            }while(read(p0[0], (void *)&d, sizeof(d)));
            close(p0[0]);
            close(p1[1]);
            wait(0);
        }
    }
    exit(0);

}

int main(int argc, char* argv[]){
    int p[2];
    pipe(p);
    if(fork() == 0){
        primes(p);
    }else{
        close(p[0]);
        for(int i = 2; i <= 35; i++){
            if(write(p[1], (void *)(&i), sizeof(i)) != sizeof(i)){
                fprintf(2, "write %d error\n", i);
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}
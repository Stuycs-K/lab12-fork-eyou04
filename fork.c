#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void error() {
    printf("errno %d\n",errno);
    printf("%s\n",strerror(errno));
    exit(1);
}

// int random(int min, int max) {
//     int num = rand() % (max - min + 1) + min;
//     return num;
// }

int random_number() {
    int random_file;
    int num;
    random_file = open("/dev/random", O_RDONLY, 0);
    if (random_file == -1) error();
    read(random_file, &num, sizeof(int));
    int rand = 1 + (num % 5);
    if (rand < 1) return rand * -1;
    return rand;
}

int main() {
    int child_1_num = random_number();
    int child_2_num = random_number();
    printf("%d about to create 2 child processes\n", getpid());
    pid_t child_1, child_2;
    child_1 = fork();
    if (child_1 < 0){
        perror("child_1 fork fail\n");
        exit(1);
    }
    if (child_1 == 0) {
        printf("%d %dsec\n", getpid() , child_1_num);
        sleep(child_1_num);
        printf("%d finished after %d seconds\n",getpid(), child_1_num);
    } else {
        int status;
        pid_t child_pid = wait(&status);

        if (child_pid == -1) {
            perror("child_1 wait failed\n");
            exit(1);
        }
        
        if (!(WIFEXITED(status))) {
            printf("%d child_2 did not exit normally\n", getpid());
        }
        
        printf("Main Process %d is done. Child %d slept for %dsec\n", getpid() , child_1, child_1_num);

        child_2 = fork();

        if (child_2 < 0){
            perror("child_2 fork fail\n");
            exit(1);
        }

        if (child_2 == 0) {
            printf("%d %dsec\n", getpid() ,child_2_num);
            sleep(child_2_num);
            printf("%d finished after %d seconds\n",getpid(), child_2_num);
        } else {
            int status;
            pid_t child_pid = wait(&status);

            if (child_pid == -1) {
                perror("child_2 wait failed\n");
                exit(1);
            }

            if (!(WIFEXITED(status))) {
                printf("%d child_2 did not exit normally\n", getpid());
            }

            printf("Main Process %d is done. Child %d slept for %dsec\n", getpid() , child_2, child_2_num);
        }

    }
}
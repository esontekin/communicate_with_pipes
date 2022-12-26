#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int cp[2], pc[2];

void *wordReverser(void *args) 
{
    char readBuffer[100], reverseBuffer[100];
    int counter = 1;
    int *runCount = (int *)args;

    while (counter < *runCount) {
        counter++;
        memset(reverseBuffer, 0, sizeof reverseBuffer);
        int begin, end, count = 0;
        if(read(pc[0], readBuffer, sizeof(readBuffer)) == -1) {
            perror("Child reead ");
            exit(5);
        }
        printf("Parent  >   Child   : %s\n", readBuffer);
        while(readBuffer[count] != '\0')
            count++;
        end = count-1;
        for(begin = 0; begin < count; begin ++) {
            reverseBuffer[begin] = readBuffer[end];
            end--;
        }

        if(write(cp[1], reverseBuffer, sizeof(reverseBuffer)) ==-1) {
            perror("Child write ");
            exit(4);
        }
    }
    
}


int main (int argc, char* argv[]) {
    int result;
    pthread_t thread1;
    result = pipe(cp);
    if (result < 0) {
        perror("pipe ");
        exit(1);
    }

    result = pipe(pc);
    if(result < 0) {
        perror("pipe ");
        exit(2);
    }

    int error = pthread_create(&thread1, NULL, &wordReverser, (void *)&argc);
    if(error < 0 ) {
        perror("pthread ");
        exit(3);
    }

    for(int i = 1; i < argc; i++) {
        
        if(write(pc[1], argv[i], strlen(argv[i])+1) ==-1) {
            perror("Parent write ");
            exit(4);
        }
        
        char readBuffer[100];
        if(read(cp[0], readBuffer, sizeof(readBuffer)) == -1) {
            perror("Parent read ");
            exit(5);
        }
        printf("Child   >   Parent  : %s\n", readBuffer);
    }
    pthread_join(thread1, NULL);
    close(pc[0]);
    close(cp[1]);
    close(pc[1]);
    close(cp[0]);
}

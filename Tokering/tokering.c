#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int child(int i, int n, int t, int p){
    int token = -1;

    char pipeReadName[20];
    char pipeWriteName[20];

    sprintf(pipeReadName, "pipe%dto%d", i-1, i);
    int pipeRead = open(pipeReadName, O_RDONLY);
    if(pipeRead == -1){
        printf("Error opening read pipe%dto%d, exiting...\n", i-1, i);
        return EXIT_FAILURE;
    }
    
    sprintf(pipeWriteName, "pipe%dto%d", i, (i)%n+1);
    int pipeWrite = open(pipeWriteName, O_WRONLY);
    if(pipeWrite == -1){
        printf("Error opening write pipe%dto%d, exiting...\n", i, (i)%n+1);
        printf("%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    while(1){
        // read from pipe
        read(pipeRead, &token, sizeof(token));
        // process token
        token++;
        // with a probability of p (probability 0 to 1) wait t seconds
        
            printf("[p%d] lock on token (val = %d)\n",i, token);
            sleep(t);
            printf("[p%d] unlock token\n", i);
        
        // write token to pipe i to (i)%n+1
        write(pipeWrite, &token, sizeof(token));
    }
            
}



int main(int argc, char *argv[]){

    if (argc != 4){
    printf("You need to provide 3 arguments (n - number of processes, p-probability of bloking the submission of the token during t - seconds )\n");
    return EXIT_FAILURE;}
    
    //parse arguments
    int n = atoi(argv[1]); //n - number of processes
    int p = atoi(argv[2]); //p-probability of bloking the submission of the token during t seconds
    int t = atoi(argv[3]); //t - seconds

    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    //Creating the pipes
    for(int i = 1; i <= n; i++){
        char pipeName[20];
        sprintf(pipeName, "pipe%dto%d", i, (i)%n+1);
        if(mkfifo(pipeName, mode) == -1){
            printf("Error creating pipe %s, exiting...\n", pipeName);
            return EXIT_FAILURE;
        }
    }

    for(int i = 2; i <= n; i++){
        //fork, set pid and handle error while forking
        size_t pid = fork();
        if(pid < 0){
            printf("Error creating process %d, exiting...\n", i);
            return EXIT_FAILURE;
        }


        if(pid==0){  
                child(i,n,t, p);
        }
    }
                int token = 1;                  // only for process 1
                //Write in pipe(i)to(i+1)
            char pipeReadName[20];
            char pipeWriteName[20];
                sprintf(pipeWriteName, "pipe%dto%d", 1, (1)%n+1);

                int pipeWrite = open(pipeWriteName, O_WRONLY);
                
                if(pipeWrite == -1){
                    printf("Error opening pipe1to2, exiting...\n");
                    return EXIT_FAILURE;
                }

                write(pipeWrite, &token, sizeof(token)); // only for process 1

                //process will read from pipe(n)to1, if it read something, it will increment the token and write it to pipe(1)to2
                
                sprintf(pipeReadName, "pipe%dto%d", n, 1);
                int pipeRead = open(pipeReadName, O_RDONLY);
                if(pipeRead == -1){
                    printf("Error opening pipe%dto%d, exiting...\n", n, 1);
                    return EXIT_FAILURE;
                }

                
                sprintf(pipeWriteName, "pipe%dto%d", 1, (1)%n+1);
                pipeWrite = open(pipeWriteName, O_WRONLY);
                if(pipeWrite == -1){
                    printf("Error opening pipe%dto%d, exiting...\n", 1, (1)%n+1);
                    return EXIT_FAILURE;
                }

                while(1){

                    read(pipeRead, &token, sizeof(token));
                    // process token
                    token++;
                    // with a probability of p (probability 0 to 1) wait t seconds
                    
                        printf("[p%d] lock on token (val = %d)\n",1, token);
                        sleep(t);
                        printf("[p%d] unlock token\n", 1);
                    
                    // write token to pipe 1to2

                    write(pipeWrite, &token, sizeof(token));
                }
    return EXIT_SUCCESS;
}

//Capture signal of Ctrl+C and delete the pipes
/*
    for(int i = 1; i <= n; i++){
        char pipeName[20];
        sprintf(pipeName, "pipe%dto%d", i, (i)%n+1);
        unlink(pipeName);
    }
*/
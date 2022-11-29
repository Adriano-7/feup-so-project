#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


/*Write a program that gets 3 integers - n, p and t - from the command
line. When executed, it creates n processes connected to each other by “named pipes”. The
named pipes are designated pipe1to2, pipe2to3,..., pipento1 and each allows one-way
communication between the i-th and the i+1-th processes. The last named pipe closes the
ring allowing process n to communicate with process 1. Once this process ring is created,

p1 should send a token (a message with an integer with an initial value 0) to the next
process (p1 > p2) and so on (p2 > p3 >... > pn > p1 >...). The token must circulate
between processes endlessly, increasing its value at each hop. Each time a process receives
the token, it must immediately resend it to the next process or, with a probability of p,
block its submission during t seconds and print a message alerting to this fact (lock on token (val = token value)), when it unlocks message is [pi] unlock token. In either case, the value of the token must be incremented.
n - number of processes, p-probability of bloking the submission of the token during t - seconds 
*/

int main(int argc, char *argv[]){
    //Check if the number of arguments is correct
    if (argc != 4){
        printf("You need to provide 3 arguments (n - number of processes, p-probability of bloking the submission of the token during t - seconds )\n");
        return EXIT_FAILURE;}
    //Take the values of the arguments
    int n = atoi(argv[1]); //n - number of processes
    int p = atoi(argv[2]); //p-probability of bloking the submission of the token during t seconds
    int t = atoi(argv[3]); //t - seconds

    //Creation of the pipe ring (n pipes) The last named pipe closes the ring allowing process n to communicate with process 1
    for(int i = 1; i <= n; i++){
        char pipeName[20];
        sprintf(pipeName, "pipe%dto%d", i, (i)%n+1);
        if(mkfifo(pipeName, 0666) == -1){
            printf("Error creating pipe %s, exiting...\n", pipeName);
            return EXIT_FAILURE;
        }
    }
    int token = 0;

    //Creation of the processes and make them communicate through the pipes
    //The first process writes token = 0 in pipe1to2
    

    //we begin by writing the token to pipe1to2
    if(fork() == 0){
        char pipeName[20];
        sprintf(pipeName, "pipe%dto%d", 1, 2);
        FILE *pipe = fopen(pipeName, "w");
        fprintf(pipe, "%d", token);
        fclose(pipe);
        exit(0);
    }
    
    //Second process reads from pipe1to2 and writes in pipe2to3
    //The third process reads from pipe2to3 and writes in pipe3to4
    //...
    //The last process reads from pipe(n-1)to(n) and writes in pipe(n)to1
    for(int i = 2; i <= n; i++){
        if(fork() == 0){
            //Read from pipe(i-1)to(i)
            char pipeName[20];
            sprintf(pipeName, "pipe%dto%d", i-1, i);
            FILE *pipe = fopen(pipeName, "r");
            if(pipe == NULL){
                printf("Error opening pipe %s, exiting...\n", pipeName);
                return EXIT_FAILURE;
            }
            fscanf(pipe, "%d", &token);
            fclose(pipe);
            token++;

            //Theres a probability p of blocking the submission of the token during t seconds

                printf("[p%d]lock on token (val = %d)\n",i,token);
                sleep(t);
                printf("[p%d]unlock token\n", i);


            //Write in pipe(i)to(i+1)
            sprintf(pipeName, "pipe%dto%d", i, (i)%n+1);
            pipe = fopen(pipeName, "w");
            if(pipe == NULL){
                printf("Error opening pipe %s, exiting...\n", pipeName);
                return EXIT_FAILURE;
            }
            fprintf(pipe, "%d", token);
            fclose(pipe);
        }
    }


    //Erase pipes
    for(int i = 1; i <= n; i++){
        char pipeName[20];
        sprintf(pipeName, "pipe%dto%d", i, (i)%n+1);
        unlink(pipeName);
    }
    return EXIT_SUCCESS;
}
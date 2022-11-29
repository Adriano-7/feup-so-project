#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    //Check if the number of arguments is correct
    if (argc != 4){
        printf("You need to provide 3 arguments (n - number of processes, p-probability of bloking the submission of the token during t - seconds )\n");
        return EXIT_FAILURE;}

    int n = atoi(argv[1]); //n - number of processes
    int p = atoi(argv[2]); //p-probability of bloking the submission of the token during t seconds
    int t = atoi(argv[3]); //t - seconds

    //Creation of the pipe ring (n pipes) The last named pipe closes the ring allowing process n to communicate with process 1
    for(int i = 0; i < n-1; i++){
        char pipeName[20];
        sprintf(pipeName, "pipe%dto%d", i+1, i+2);
        if(mkfifo(pipeName, 0666) == -1){
            printf("Error creating pipe %s, exiting...\n", pipeName);
            return EXIT_FAILURE;
        }
    }

    //Creation of the last pipe
    char pipeName[20];
    sprintf(pipeName, "pipe%dto%d", n, 1);
    if(mkfifo(pipeName, 0666)){
        printf("Error creating pipe %s, exiting...\n", pipeName);
        return EXIT_FAILURE;
    }

    //p1->p2, p2->p3, p3->p4, p4->p5, p5->p1
    int token = 0; //Initial value of the token
    int pid = 0; //Process id
    //

    //Writing the token to the first pipe
    int fd = open("pipe1to2", O_WRONLY);
    if(write(fd, &token, sizeof(int))) {
        printf("Error writing to pipe1to2\n");
        return EXIT_FAILURE;
    };

    close(fd);

    //Check read the pipe1to2
     int fa = open("pipe1to2", O_RDONLY);
    if(read(fa, &token, sizeof(int))){
        printf("Error reading from pipe1to2\n");
        return EXIT_FAILURE;
    };
    printf("Token value: %d\n", token);

    /*
    for(int i = 1; i < n-1; i++){
        pid = fork();
        if(pid == 0){
            
            printf("[pid%d] sending token to next process\n", getpid());
            //Open the pipe
            char pipeName[20];
            sprintf(pipeName, "pipe%dto%d", (i+1)%n, (i+2)%n);
            int fd = open(pipeName, O_WRONLY);
            if(fd == -1){
                printf("Error opening pipe %s, exiting...\n", pipeName);
                return EXIT_FAILURE;
            }
            //Send the token
            write(fd, &token, sizeof(int));
            close(fd);
            exit(0);
        }
        
    }*/
}
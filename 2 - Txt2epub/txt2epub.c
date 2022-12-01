#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]){
    if (argc <= 1) {
        printf("You need to provide at least one .txt file to be converted to .epub\n");
        return EXIT_FAILURE;
    }

    int n = argc - 1; //Number of files to be converted
    int pid = 0; //Process id
    int status = 0; //Status of the process

    char* zipArguments = malloc(16*n*sizeof(char));
    zipArguments[0] = "zip"; zipArguments[1] = "ebooks.zip"; zipArguments[2] = "-qq"; zipArguments[argc+1] = NULL;

    for(int i=0 ; i<n ; i++){
        char* outputName = malloc(strlen(argv[i+1]) + 5);
        strcpy(outputName, argv[i+1]);
        outputName[strlen(argv[i+1]) - 4] = '\0';
        strcat(outputName, ".epub");
        zipArguments[i+3] = outputName;

        pid = fork();
        if(pid == 0){
            printf("[pid%d] converting %s\n", getpid(), argv[i+1]);
            
            char *args[] = {"pandoc","--quiet", argv[i+1], "-o", outputName, NULL};
            if(execvp("pandoc", args)<0){ printf("Error in execvp converting to .epub\n"); return EXIT_FAILURE;}
            
            free(outputName);
            wait(&status);
        }
        else{
            for(int i=0 ; i<n ; i++){
                wait(&status);
            }
        }
    }

    if(execvp("zip", zipArguments)<0){ printf("Error in execvp while zipping"); return EXIT_FAILURE;}
    free(zipArguments);
    return EXIT_SUCCESS;
}
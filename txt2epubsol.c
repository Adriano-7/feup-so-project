#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc <= 1) {
        printf("You need to provide at least one .txt file to be converted to .epub\n");
        return EXIT_FAILURE;
    }

    int n = argc - 1; //Number of files to be converted
    int pid = 0; //Process id
    int status = 0; //Status of the process
    char* zip [argc + 2];
    zip[0] = "zip";
    zip[1] = "books.zip"
    zip[argc+1] = NULL;

    for(int i=0 ; i<n ; i++){//Por cada ficheiro
        pid = fork(); //Crio um processo filho
        if(pid == 0){ //Se for o filho, chama o pandoc e converte o ficheiro
            printf("[pid%d] converting %s\n", getpid(), argv[i+1]);

            //Using mallock to generate outputName (argv[i+1]) without .txt adding .epub
            char* outputName = malloc(strlen(argv[i+1]) + 5);
            strcpy(outputName, argv[i+1]);
            outputName[strlen(argv[i+1]) - 4] = '\0';
            strcat(outputName, ".epub");
            
            char *args[] = {"pandoc","--quiet", argv[i+1], "-o", outputName, NULL};
            if(execvp("pandoc", args)<0){ printf("Error in execvp converting to .epub\n"); return EXIT_FAILURE;}
            zip[i+2] = outputName;

            free(outputName);
            wait(&status);

        }
        else{ //O pai espera pelos filhos
            for(int i=0 ; i<n ; i++){
                wait(&status);
            }
        }

    }
    if((pid = fork()) == -1){
        return EXIT_FAILURE;
    }
    else if(pid == 0){
        printf("[pid%d] creating ebooks.zip.. \n", getpid());
        execvp(zip[0], zip_arguments);
        fprintf(stderr, "Command not executed: %s\n",stderror(errno));
        exit(EXIT_FAILURE);
    }
    else{
        wait(NULL);
    }
    return EXIT_SUCCESS;
}

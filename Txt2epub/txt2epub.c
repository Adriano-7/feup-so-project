#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc <= 1) {
        printf("You need to provide at least one .txt file to be converted \n");
        return EXIT_FAILURE;
    }

    int n = argc - 1; //Number of files to be converted
    int pid = 0; //Process id
    int status = 0; //Status of the process

    for(int i=0 ; i<n ; i++){//Por cada ficheiro
        pid = fork(); //Crio um processo filho
        if(pid == 0){ //Se for o filho, chama o pandoc e converte o ficheiro
            printf("[pid%d] converting %s\n", getpid(), argv[i+1]);

            //Using mallock to generate outputName (argv[i+1]) without .txt adding .epub
            char* outputName = malloc(strlen(argv[i+1]) + 5);
            strcpy(outputName, argv[i+1]);
            outputName[strlen(argv[i+1]) - 4] = '\0';
            strcat(outputName, ".epub");

            char *args[] = {"pandoc", argv[i+1], "-o", outputName, NULL};
            execvp("pandoc", args);
            wait(&status);
        }
        else{ //O pai espera pelos filhos
            for(int i=0 ; i<n ; i++){
                wait(&status);
            }
        }
    }
    return 0;
}
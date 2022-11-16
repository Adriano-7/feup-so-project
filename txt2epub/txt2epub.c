#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
            char outputName[sizeof(argv[i+1]) + 5];
            sprintf(outputName, "%s.epub", argv[i+1]);
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
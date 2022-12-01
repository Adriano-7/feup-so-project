#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int numBytesInFile(char *filename){
        FILE *file;
        file = fopen(filename, "r");
        int nBytes = 0;
        char c;
        while((c = fgetc(file)) != EOF){nBytes++;}
        return nBytes;
}

int main(int argc, char* argv[]){
        if (argc <= 3 || argc > 4) {
                printf("You need two insert two arguments (filename, n-fragments & m-character)\n");
                return EXIT_FAILURE;
        }

        char *filename = argv[1];
        int n = atoi(argv[2]); 
        int m = atoi(argv[3]);

        FILE *file;
        file = fopen(filename, "r");
        if(file == NULL){
                printf("Couldn't open the file\n");
                return EXIT_FAILURE;
        }

        int sz = numBytesInFile(filename);        

        srandom(0);
        int rand_pos [n];
        for(int i=0 ; i<n ; i++){
                double r = random()%(sz-m);
                rand_pos[i] = r;
        }

        for(int i=0 ; i<n ; i++){
                char buffer[m]; 
                fseek(file, rand_pos[i], SEEK_SET);
                fread(buffer, sizeof(char), m, file);
                for(int j=0 ; j<m ; j++){
                        if(buffer[j] == '\n'){
                                buffer[j] = ' ';
                        }}
                printf(">%.*s<\n",m,buffer);
        }
        fclose(file);
        return 0;
}
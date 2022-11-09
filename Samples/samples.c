#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int numBytesInFile(){
        //count number of bytes in quote.txt
        FILE *fp;
        fp = fopen("quote.txt", "r");
        int numBytes = 0;
        char c;
        while((c = fgetc(fp)) != EOF){
                numBytes++;
        }
        return numBytes;
}

int main(int argc, char* argv[]){
        /* check if exactly 2 arguments are present */
        if (argc <= 2) {
                printf("You need to give at least two arguments\n");
                return EXIT_FAILURE;
        }


        //Opening the file quote.txt
        FILE *fp;
        fp = fopen("quote.txt", "r");

        //Checking if the file was opened correctly
        if(fp == NULL){
                printf("Error opening the file");
                return EXIT_FAILURE;
        }

        //argv[1] to int
        int n = atoi(argv[1]);
        int m = atoi(argv[2]);

        //Finding the number of bytes in the file
        int sz = numBytesInFile();

        //Generate n random numbers


        int rand_pos [n];
        srand(time(NULL));
        for(int i=0 ; i<n ; i++){
                double r = rand()%sz;
                rand_pos[i] = r;
        }


        //Using fseek to output n number of strings of m bytes
        for(int i=0 ; i<n ; i++){
                char buffer[m];
                fseek(fp, rand_pos[i], SEEK_SET);
                fread(buffer, sizeof(char), m, fp);
                //output content from buffer to terminal
                printf(">%s<\n", buffer);
        }
        return 0;
}
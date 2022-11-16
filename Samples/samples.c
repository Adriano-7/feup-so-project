#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int numBytesInFile(){
        FILE *file;
        file = fopen("quote.txt", "r");
        int nBytes = 0;
        char c;
        while((c = fgetc(file)) != EOF){nBytes++;}
        return nBytes;
}

int main(int argc, char* argv[]){
        /* check if exactly m and n */
        if (argc <= 2) {
                printf("You need two insert two arguments (n-fragments & m-character)\n");
                return EXIT_FAILURE;
        }
        int n = atoi(argv[1]); //Transform arguments to int
        int m = atoi(argv[2]);

        //Opening the file quote.txt
        FILE *file;
        file = fopen("quote.txt", "r");
        //Checking if the file was opened correctly
        if(file == NULL){
                printf("Couldn't open the file\n");
                return EXIT_FAILURE;
        }

        //Finding the number of bytes in the file
        int sz = numBytesInFile();

        srand(time(NULL)); //Creating a sub-random sid

        int rand_pos [n];//Array to store random positions
        for(int i=0 ; i<n ; i++){
                double r = rand()%(sz-m); //generate a random position to star readin
                rand_pos[i] = r;
        }

        //Using fseek to output n number of strings of m bytes
        for(int i=0 ; i<n ; i++){
                char buffer[m]; //Buffer where to store the string
                fseek(file, rand_pos[i], SEEK_SET); //fseek is setting the file position of the stream to the random offset, SEEK_SET means the of offset is calculated from the beggining of the file
                fread(buffer, sizeof(char), m, file); //reads m bytes of data from file into the buffer
                for(int j=0 ; j<m ; j++){
                        if(buffer[j] == '\n'){
                                buffer[j] = ' ';
                        }}
                printf(">%.*s<\n",m,buffer);
        }
        fclose(file);
        return 0;
}
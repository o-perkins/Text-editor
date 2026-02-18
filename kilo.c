#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct termios origtermios;

void DefaultTerminal(){
    printf("default function hit\n");
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origtermios);


}


void RawMode(){
    printf("raw mode function hit\n");
    tcgetattr(STDIN_FILENO, &origtermios);

    struct termios raw;
    
    raw = origtermios;

    raw.c_lflag &= ~(ECHO);

    raw.c_lflag &= ~(ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

void memAdd(char c, char **pointer, int *size)
{
    *size +=1;

    char *tempPointer = realloc(*pointer, *size);
    if (tempPointer == NULL) {
        perror("failed to allocate memory\n");
        free(*pointer);
        exit(1);
    }
    *pointer = tempPointer;
    (*pointer)[*size-2] = c;
    (*pointer)[*size-1]='\0';

}
void backspace(char **pointer,int *memSize){
    if (*memSize == 1){
        return;
    };
    *memSize -=1;
    char *tempPointer = realloc(*pointer, *memSize);
    if (tempPointer == NULL) {
        perror("failed to allocate memory");
        free(*pointer);
        exit(1);
    };
    *pointer = tempPointer;
    (*pointer)[*memSize-1]='\0';

}

char* textbox(){


    char *pointer = malloc(1);
    if (pointer == NULL) {
        perror("failed to allocate memory\n");
    return NULL;
    };
    
    int memSize = 1;    
    char c;

    while (read(STDIN_FILENO, &c, 1) == 1 && c != '~'){
    
    if (c == 127){
        backspace(&pointer, &memSize);
        write(STDOUT_FILENO, "\b \b",3);
    }
    else{
        write(STDOUT_FILENO, &c,1);
        memAdd(c, &pointer, &memSize);
    }
    }
    return pointer;
}

void saveFile(char *pntr)
{
    write(STDOUT_FILENO, "name your file: ", 16);
    char *savepntr = textbox();
    if (savepntr == NULL) {
        perror("failed to allocate memory\n");
    return;
    }
    FILE *file = fopen(savepntr, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        free(savepntr);
        return;
    }
    fprintf(file, "%s", pntr);
    fclose(file);

    printf("\nCaptured text to file: %s\n", savepntr);
    free(pntr);
    free(savepntr);
}


int main() {
    printf("main function hit\n");
    RawMode();
    
    char *pntr = textbox();
    
    printf("\nCaptured text: %s\n", pntr);

    
    saveFile(pntr);
    DefaultTerminal();

	
    return 0;
}

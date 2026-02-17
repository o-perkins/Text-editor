#include <termios.h>
#include <unistd.h>
#include <stdio.h>

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



int main() {
    printf("main function hit\n");
    RawMode();    
    
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
    write(STDOUT_FILENO, &c,1);
    }
    DefaultTerminal();


    return 0;
}

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <termio.h>
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
}

#ifndef false
#define false 0
#endif
#ifndef true
#define true !(false)
#endif
#define WIDTH   80
#define HEIGHT  24
#define MAX_COMMAND_SIZE 128
#define MAX_BUFFER_SIZE 128
#define TAB_LENGTH 4

#define ESC 27
#define BACKSPACE 127
#define ENTER 10
#define TAB 9
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

void draw(char **lines,int y,int x);

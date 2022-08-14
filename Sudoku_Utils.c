#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Running = 1;

/*Colors*/

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define GREY 0x757575
#define RED 0xFF0000
#define BLUE 0x0000FF
#define GREEN 0x00FF00

/*Rendering*/

struct{
    int Width;
    int Height;
}typedef WINDOWSIZE;

struct{
    int Height;
    int Width;
    int *Pixels;
    BITMAPINFO BitMapInfo;
}typedef RENDERBUFFER;

RENDERBUFFER RenderBuffer;

/*Input*/

struct{
    int IsDown;
    int Changed;
}typedef BUTTON;

enum{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    ALT,
    SPACE,
    ENTER,
    BACKSPACE,
    LBUTTON,
    RBUTTON,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    
    BUTTON_COUNT,
};

struct{
    int X;
    int Y;
    int TileNum;
}typedef MOUSE;

struct{
    MOUSE Mouse;
    BUTTON button[BUTTON_COUNT];
}typedef Input;

Input input = {0};

#define PRESS(b) input.button[b].IsDown == 1 && input.button[b].Changed == 1 
#define RELEASE(b) input.button[b].IsDown == 0 && input.button[b].Changed == 1 

/*Clamp*/

int Clamp(int val, int min, int max){
    if(val < min) 
        return min;
    if(val > max) 
        return max;
    return val;
}

/*Game*/
#define tile 68
#define n 9

struct{
    int X;
    int Y;
    int Color;
    int Selected;
    int Number;
}typedef TILEINFO;

int Num1[8][8] = {
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,0,0,0},
    {0,0,1,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
};

int Num0[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
};

int Num2[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
};

int Num3[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
};

int Num4[8][8] = {
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
};

int Num5[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
};

int Num6[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
};

int Num7[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
};

int Num8[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
};

int Num9[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
};
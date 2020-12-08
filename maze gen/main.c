#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BufX 200
#define BufY 200

#define VerticalWall '|'
#define HorizontalWall '-'
#define SpaceFiller '.'
#define Empty ' '

char buffer[BufY][BufX];

HANDLE hwnd;
CONSOLE_SCREEN_BUFFER_INFO info;
CONSOLE_SCREEN_BUFFER_INFO* pInfo;

COORD CursorCoord;

void Internal_SetCursorPos(USHORT y, USHORT x){
    CursorCoord.X = x;
    CursorCoord.Y = y;

    SetConsoleCursorPosition(hwnd, CursorCoord);
}

void SetBufferChar(USHORT y, USHORT x, char c){
    buffer[y][x] = c;
}

char GetBufferChar(USHORT y, USHORT x){
    return buffer[y][x];
}

void DisplayBuffer(USHORT MaxY, USHORT MaxX, USHORT OffsetY, USHORT OffsetX){

    /* MaxY -- The maze's height */
    /* MaxX -- The maze's width */

    /* OffsetY -- The offset from the top of the console, from which to start building the maze */
    /* OffsetX -- The offset from the left side of the console, from which to start building the maze */

    time_t t;

    srand((unsigned) time(&t));

    USHORT FinalX = MaxX + OffsetX;
    USHORT FinalY = MaxY + OffsetY;

    if(FinalX > BufX || FinalY > BufY) return;

    if(OffsetY + OffsetX != 0){
        for(int y = 0; y < OffsetY; y++){
            for(int x = 0; x < OffsetX; x++){
                Internal_SetCursorPos(y, x);
                SetBufferChar(y, x, Empty);
            }
        }
    }
    else{
        // If the given offset is 0, we dont do anything
    }

    for(int y = OffsetY; y < MaxY + OffsetY; y++){
        for(int x = OffsetX; x < MaxX + OffsetX; x++){

            char c = Empty;

            if(GetBufferChar(y - 1, x) == HorizontalWall && GetBufferChar(y - 2, x) == HorizontalWall){
                Internal_SetCursorPos(y, x);
                SetBufferChar(y, x, c);
                return;
            }

            if(GetBufferChar(y, x - 1) == VerticalWall && GetBufferChar(y, x - 2) == VerticalWall){
                Internal_SetCursorPos(y, x);
                SetBufferChar(y, x, c);
                return;
            }

            /* Currently, lacking an algorithm, the maze's doors and walls are randomly placed */
            /* No guarantee for being solvable yet */
            USHORT r = rand() % 10;

            if(x == OffsetX || x == FinalX - 1){
                c = VerticalWall;
            }

            else if(x % 5 == 0){
                if(r > 7) c = Empty;
                else{
                    c = VerticalWall;
                }
            }

            if(y == OffsetY || y == FinalY - 1){
                c = HorizontalWall;
            }

            else if(y % 5 == 0){
                if(r > 6) c = Empty;
                else{
                    c = HorizontalWall;
                }
            }

            Internal_SetCursorPos(y, x);
            SetBufferChar(y, x, c);
            printf("%c", GetBufferChar(y, x));
        }
    }
}

int main(void)
{
    hwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    pInfo = &info;
    GetConsoleScreenBufferInfo(hwnd, pInfo);

    COORD BufferSize;

    BufferSize.X = BufX;
    BufferSize.Y = BufY;

    SetConsoleScreenBufferSize(hwnd, BufferSize);

    DisplayBuffer(19, 31, 1, 5);

    return 0;
}

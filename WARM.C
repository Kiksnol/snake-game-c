/*Dyakonov Nickolay, ND4 , 14.10.2020*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "warm.h"

#define CELL_FREE 0
#define CELL_WARM 1
#define CELL_GOOD 2
#define CELL_BAD 3
#define GAME_OVER 4
#define WIN 5

int Field[GRID_H][GRID_W];
PNT Head, newpos, Tail[MAX], Delta[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
int Dir = 0, TailLen = 4, TailLenStart = 1;
byte Font[2048];
char tmp[10];

void fope( void )
{
  FILE *F;    
  F = fopen("font3.fnt", "rb");
  if (F != NULL)
  {
    fread(Font, 1, 2048, F);
    fclose(F);
  }
}

void GoodFood( void )
{
  int x = rand() % GRID_W, y = rand() % GRID_H;
  
  if (Field[y][x] != CELL_WARM && Field[y][x] != CELL_BAD)
    Field[y][x] = CELL_GOOD;
}

void BadFood( void )
{
  int a = rand() % GRID_W, b = rand() % GRID_H;
  if (Field[b][a] != CELL_WARM && Field[b][a] != CELL_GOOD)
    Field[b][a] = CELL_BAD;
}

void DrawChar( int X0, int Y0, char Ch, int R, int G, int B )
{
  int x, y;

  for (y = 0; y < 8; y++)
    for (x = 0; x < 8; x++)
      if ((Font[Ch * 8 + y] & (0x80 >> x)) != 0)
        PutPixel(X0 + x, Y0 + y, R, G, B);
}

void DrawChars( int X0, int Y0, char *Str, int R, int G, int B )
{
  int i;

  for (i = 0; Str[i] != 0; i++)
    DrawChar(X0, Y0, Str[i], R, G, B), X0 += 8;
}

void GameOver( void )
{
  int x, y;
  for (y = 0; y < GRID_H; y++)
    for (x = 0; x < GRID_W; x++)
      Field[y][x] = GAME_OVER;
  WarmDrawField();
}

void EzWin( void )
{
  int x, y;
  for (y = 0; y < GRID_H; y++)
    for (x = 0; x < GRID_W; x++)
      Field[y][x] = WIN;
  WarmDrawField();
}

void WarmInit( void )
{
  int i, x, y;
  Dir = 0;
  for (y = 0; y < GRID_H; y++)
    for (x = 0; x < GRID_W; x++)
      Field[y][x] = CELL_FREE;

  Head.X = 50;
  Head.Y = 50;
  Tail[0].X = 50;
  Tail[0].Y = 51;

  Field[Head.Y][Head.X] = CELL_WARM;
  for (i = 0; i < TailLen; i++)
    Field[Tail[i].Y][Tail[i].X] = CELL_WARM;
  
  for (i = 0; i < 25; i++)
    GoodFood();
  
  for (i = 0; i < 50; i++)
    BadFood();
}

void FreeCellCheck( void )
{
  newpos.X = Head.X + Delta[Dir].X;
  newpos.Y = Head.Y + Delta[Dir].Y;

  if (newpos.X < 0 && newpos.Y < 0 && newpos.X > GRID_W && newpos.Y > GRID_H && Field[newpos.Y][newpos.X] != CELL_WARM)
    GameOver();
  else if (Field[newpos.Y][newpos.X] == CELL_WARM)
    GameOver();
  else 
    Head = newpos;
}

void WarmStep( void )
{
  int i, x, y;
  static double OldTime;
  double SyncTime = clock() / (double)CLOCKS_PER_SEC, DeltaTime = SyncTime - OldTime;
  
  if (DeltaTime < 0.05)
    return;
  OldTime = SyncTime;

  for (y = 0; y < GRID_H; y++)
    for (x = 0; x < GRID_W; x++)
      if (Field[y][x] != CELL_GOOD && Field[y][x] != CELL_BAD)
        Field[y][x] = CELL_FREE;
  
  Field[Head.Y][Head.X] = CELL_WARM;
  for (i = 0; i < TailLen - 1; i++)
    Field[Tail[i].Y][Tail[i].X] = CELL_WARM;
  
  for (i = TailLen -1; i > 0; i--)
          Tail[i] = Tail[i - 1];
  Tail[0] = Head;
 
  FreeCellCheck();
  if (Field[Head.Y][Head.X] == CELL_GOOD)
  {  
    TailLen++;
    GoodFood();
  }
  if (Field[Head.Y][Head.X] == CELL_BAD)
  {  
    TailLen--;
    BadFood();
  }
  if (TailLen - 1 < 1)
    GameOver();
  if (TailLen > 11)
    EzWin();
  WarmDrawField();
}

void WarmKeyboard( byte Key )
{
  if (Key == 'a' || Key == 'A')
    Dir = (Dir - 1) & 3;
  if (Key == 'd' || Key == 'D')
    Dir = (Dir + 1) & 3;
  else if (Key == 'i' || Key == 'I')
    WarmInit();
}

void WarmDrawField( void )
{
  int x, y;
  
  for (y = 0; y < GRID_H; y++)
    for (x = 0; x < GRID_W; x++)
      switch (Field[y][x])
      {
        case CELL_BAD:
          PutPixel(x, y, 255, 0, 0);
          break;
        case CELL_GOOD:
          PutPixel(x, y, 0, 255, 0);
          break;
        case CELL_WARM:
          PutPixel(x, y, 255, 229, 180);
          break;
        case CELL_FREE:
          PutPixel(x, y, 80, 0, 120);
          break;
        case GAME_OVER:
          PutPixel(x, y, 0, 0, 0);
          DrawChars(90, 50, "Game Over", 255, 0, 0);
          break;
        case WIN:
          PutPixel(x, y, 0, 0, 0);
          DrawChars(90, 50, "Ez Win", 0, 255, 255);
          break;  
      }


}
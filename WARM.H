/*Dyakonov Nickolay, ND4 , 14.10.2020*/

#define GRID_H 100
#define GRID_W 180
#define MAX 100

typedef struct
{
  int X, Y;
} PNT;

typedef unsigned char byte;

void PutPixel( int x, int y, int r, int g, int b );
void fope( void );

void WarmKeyboard ( byte Key );
void WarmStep( void );
void WarmInit( void);
void WarmDrawField( void );
void GameOver( void );
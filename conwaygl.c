#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define COEF 15
#define W (WINDOW_HEIGHT/COEF)
#define H (WINDOW_WIDTH/COEF)
#define DELAY 130

int check_grid(int(*)[W],int(*)[W]);

int check_cell(int(*)[W],int ,int );

int print_grid(int(*)[W],SDL_Texture*,SDL_Renderer*);

int main()
{
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)!=0)
    {
      printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
      return 1;
    }

    //Create Window
    SDL_Window* wmain = SDL_CreateWindow
    (
      "Celular Automata",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_FULLSCREEN_DESKTOP
    );//SDL_WINDOW_FULLSCREEN_DESKTOP

    if(!wmain)
    {
      printf("Erro ao gerar janela: %s\n",SDL_GetError());
      SDL_DestroyWindow(wmain);
      SDL_Quit();
      return 1;
    }

    //Create Renderer
    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *r = SDL_CreateRenderer(wmain,-1,renderer_flags);
    if(!r)
    {
      printf("Erro ao gerar renderizador: %s\n",SDL_GetError());
      SDL_DestroyRenderer(r);
      SDL_DestroyWindow(wmain);
      SDL_Quit();
      return 1;
    }

    SDL_Texture *t = SDL_CreateTexture(r,0,SDL_TEXTUREACCESS_TARGET,WINDOW_WIDTH,WINDOW_HEIGHT);
    if(!t)
    {
      printf("Erro ao gerar renderizador: %s\n",SDL_GetError());
      SDL_DestroyTexture(t);
      SDL_DestroyRenderer(r);
      SDL_DestroyWindow(wmain);
      SDL_Quit();
      return 1;
    }
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
  srand(time(NULL));
  int grid[H][W]={0}, newgrid[H][W];
  int (*g)[W] = grid,(*newg)[W] = newgrid;

  // //beacon
  grid[3][3]=1;grid[3][4]=1;
  grid[4][3]=1;grid[5][6]=1;
  grid[6][6]=1;grid[6][5]=1;

  //pulsar horizontal
  grid[10][12]=1;grid[10][13]=1;grid[10][14]=1;
  grid[10][18]=1;grid[10][19]=1;grid[10][20]=1;

  grid[15][12]=1;grid[15][13]=1;grid[15][14]=1;
  grid[15][18]=1;grid[15][19]=1;grid[15][20]=1;

  grid[17][12]=1;grid[17][13]=1;grid[17][14]=1;
  grid[17][18]=1;grid[17][19]=1;grid[17][20]=1;

  grid[22][12]=1;grid[22][13]=1;grid[22][14]=1;
  grid[22][18]=1;grid[22][19]=1;grid[22][20]=1;

  //pulsar vertical
  grid[12][10]=1;grid[13][10]=1;grid[14][10]=1;
  grid[18][10]=1;grid[19][10]=1;grid[20][10]=1;

  grid[12][15]=1;grid[13][15]=1;grid[14][15]=1;
  grid[18][15]=1;grid[19][15]=1;grid[20][15]=1;

  grid[12][17]=1;grid[13][17]=1;grid[14][17]=1;
  grid[18][17]=1;grid[19][17]=1;grid[20][17]=1;

  grid[12][22]=1;grid[13][22]=1;grid[14][22]=1;
  grid[18][22]=1;grid[19][22]=1;grid[20][22]=1;


  //MIDDLE WSS
  grid[30][31]=1;grid[30][32]=1;grid[30][33]=1;
  grid[31][30]=1;grid[31][31]=1;grid[31][32]=1;grid[31][33]=1;grid[31][34]=1;
  grid[32][30]=1;grid[32][31]=1;grid[32][32]=1;grid[32][35]=1;grid[32][34]=1;
  grid[33][33]=1;grid[33][34]=1;

  //HUGE WSS
  grid[40][30]=1;grid[40][31]=1;grid[40][32]=1;grid[40][33]=1;
  grid[41][29]=1;grid[41][30]=1;grid[41][31]=1;grid[41][32]=1;grid[41][33]=1;grid[41][34]=1;
  grid[42][29]=1;grid[42][30]=1;grid[42][31]=1;grid[42][32]=1;grid[42][35]=1;grid[42][34]=1;
  grid[43][33]=1;grid[43][34]=1;


  SDL_SetRenderTarget(r, t);
  SDL_RenderSetScale(r,COEF,COEF);

  for(int i = 0; i < H; ++i)
  {
    for(int j = 0; j < W; ++j)
    {
      // grid[i][j]=round(rand()/((float)RAND_MAX));

      if(grid[i][j])
      {
      SDL_SetRenderDrawColor(r,04,215,15,255);SDL_RenderDrawPoint(r,i,j);
      }
      else
      {
      SDL_SetRenderDrawColor(r,10,10,10,255);SDL_RenderDrawPoint(r,i,j);
      }
    }
  }
  SDL_SetRenderTarget(r,NULL);
  SDL_RenderCopy(r,t,0,0);
  SDL_RenderPresent(r);
  SDL_Delay(5*DELAY);

  int close=0;
  while(!close)
  {
    for(int k=0; k<10000; ++k)
    {
      SDL_Event e;
      while(SDL_PollEvent(&e))
      {
        switch(e.type)
        {
          case SDL_QUIT:
            close=1; exit(1);
            break;
        }
      }
      if(close){break;}
      SDL_SetRenderTarget(r,t);
      check_grid(g,newg);
      print_grid(newg,t,r);
      SDL_SetRenderTarget(r,NULL);
      SDL_RenderCopy(r,t,0,0);
      SDL_RenderPresent(r);
      int (*swap)[W] = g;
      g = newg;
      newg = swap;
      SDL_Delay(DELAY);
    }
  }

  SDL_DestroyTexture(t);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(wmain);
  SDL_Quit();

  return 0;
}

int check_grid(int grid[H][W],int newgrid[H][W])
{
  for(int i=0;i<H;++i)
  {
    for(int j=0;j<W;++j)
    {
      newgrid[i][j]=check_cell(grid,i,j);
    }
  }
  return 0;
}

int check_cell(int grid[H][W], int i, int j)
{
  int aux = 0, life = 0;
  int y0 = i-1, y1 = i+1;
  int x0 = j-1, x1 = j+1;
  if(y0<0){y0=0;};
  if(y1>=H){y1=H-1;};
  if(x0<0){x0=0;};
  if(x1>=W){x1=W-1;};

  if(grid[i][j]==1){life=1;}

  for (i=y0;i<=y1;++i)
  {
    for (j=x0;j<=x1;++j)
    {
      if(grid[i][j]){++aux;}
    }
  }

  aux -= life;
  switch(aux)
  {
    case 3:
      return 1;
      break;
    case 2:
    case 4:
    case 5:
      if(life){return 1;}
      break;
  }
  return 0;
}

int print_grid(int grid[H][W],SDL_Texture*t, SDL_Renderer*r)
{
  SDL_SetRenderTarget(r,t);
  SDL_SetRenderDrawColor(r,10,10,10,255);
  SDL_RenderClear(r);
  SDL_SetRenderDrawColor(r,04,215,15,255);
  SDL_RenderSetScale(r,COEF,COEF);
  for(int i = 0; i < H; ++i)
  {
    for(int j = 0; j < W; ++j)
    {
      if(grid[i][j]==1)
      {
      SDL_RenderDrawPoint(r,i,j);
      }
    }
  }
  SDL_SetRenderTarget(r,NULL);
  return 0;
}

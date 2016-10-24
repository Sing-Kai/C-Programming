#include <stdio.h>
#include <stdlib.h>
#include "textngraphics.h"
#include "test.h"

#define FNTFILENAME "m7fixed.fnt"
#define MAXCOLOUR 255
#define MILLISECONDDELAY 10000
#define RECTSIZE 10
#define RHEIGHT 6
#define RWIDTH 8

void create_SDL_text(cell teleTextCells[][COL], SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT]);
void create_colour(SDL_Simplewin *sw, int r, int g, int b);
void create_double_height(cell teleTextCells[][COL], SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT]);
void create_SDL_rectangle(SDL_Simplewin *sw, int x, int y, int scolour, int bcolour, int mode);
void match_colour(SDL_Simplewin *sw, int fcolour);
void match_graphic_colour(SDL_Simplewin *sw, int colour);
void double_height_top(SDL_Simplewin *sw, fntrow fontdata[][FNTHEIGHT], unsigned char chr, int ox, int oy, int fcolour, int bcolour);
void double_height_bottom(SDL_Simplewin *sw, fntrow fontdata[][FNTHEIGHT], unsigned char chr, int ox, int oy, int fcolour, int bcolour);
void check_graphics_mode(cell teleTextCells[][COL], SDL_Simplewin sw);
void check_sixel_details(SDL_Simplewin *sw, int sixel[][SIXELCOL], int x, int y, int scolour, int bcolour, int mode);
int is_background_colour(int value);
int is_double_height(int value);
int is_text_colour(int fcolour);
int change_rect_colour(int mode, int scolour, int bcolour);
int blast_colour_change(int colour);
void Neill_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[][FNTHEIGHT], unsigned char chr, int ox, int oy, int fcolour, int bcolour);
void display_background_only(SDL_Simplewin *sw, int ox, int oy, int fcolour);

/* Original Neill SDL functions unamended*/
void Neill_SDL_Init(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy);
void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname);


int create_graphics(cell teleTextCells[][COL])
{
   SDL_Simplewin sw;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   Neill_SDL_Init(&sw);
   Neill_SDL_ReadFont(fontdata, FNTFILENAME);
   Neill_SDL_Events(&sw);

   create_SDL_text(teleTextCells, sw, fontdata);
   create_double_height(teleTextCells, sw, fontdata);
   check_graphics_mode(teleTextCells, sw);
   run_graphics_test_functions();

   SDL_RenderPresent(sw.renderer);
   SDL_UpdateWindowSurface(sw.win);
   SDL_Delay(MILLISECONDDELAY);

   SDL_Quit();

   return(EXIT_SUCCESS);
}

void create_SDL_text(cell teleTextCells[][COL], SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT])
{
   int i, j, fcolour, bcolour, mode, hexValue, scolour;

   for(i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         fcolour = teleTextCells[i][j].foreground;
         bcolour = teleTextCells[i][j].background;
         scolour = teleTextCells[i][j].sixelColour;
         mode = teleTextCells[i][j].mode;
         hexValue = teleTextCells[i][j].hexValue;

         if(mode == alphanumeric_mode){
            Neill_SDL_DrawChar(&sw, fontdata, teleTextCells[i][j].text, j * FNTWIDTH, i * FNTHEIGHT, fcolour, bcolour);
         }
         /* check for blast through text */
         else if((mode == contiguous_mode || mode == separate_mode) && (is_blast_through(hexValue))){
            fcolour = blast_colour_change(scolour);
            Neill_SDL_DrawChar(&sw, fontdata, teleTextCells[i][j].text, j * FNTWIDTH, i * FNTHEIGHT, fcolour, bcolour);
         }
         else{
            display_background_only(&sw, j * FNTWIDTH, i * FNTHEIGHT, bcolour);        
         }
      }
   }
}

void display_background_only(SDL_Simplewin *sw, int ox, int oy, int bcolour)
{
   unsigned x, y;
   for(y = 0; y < FNTHEIGHT; y++){
      for(x = 0; x < FNTWIDTH; x++){
         match_colour(sw, bcolour);
         SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
      }
   }
}

/* loops through teleTextCells to and checks for double height control codes*/
void create_double_height(cell teleTextCells[][COL], SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT])
{
   int i, j, fcolour, bcolour;

   for(i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         if(is_double_height(teleTextCells[i][j].height)){

            /* checks if row above is double height code if true then create bottom double height character*/
            if(is_double_height(teleTextCells[i-1][j].height)){
               fcolour = teleTextCells[i-1][j].foreground;
               bcolour = teleTextCells[i-1][j].background;
               double_height_bottom(&sw, fontdata, teleTextCells[i-1][j].text, j * FNTWIDTH, i * FNTHEIGHT, fcolour, bcolour);
            }
            else{
               fcolour = teleTextCells[i][j].foreground;
               bcolour = teleTextCells[i][j].background;
               double_height_top(&sw, fontdata, teleTextCells[i][j].text, j * FNTWIDTH, i * FNTHEIGHT, fcolour, bcolour);
            }
            /* if row below is double hight then create bottom double height character */
            fcolour = teleTextCells[i+1][j].foreground;
            bcolour = teleTextCells[i+1][j].background;
            double_height_bottom(&sw, fontdata, teleTextCells[i+1][j].text, j * FNTWIDTH, i * FNTHEIGHT, fcolour, bcolour);
         }
      }
   }
}

void check_graphics_mode(cell teleTextCells[][COL], SDL_Simplewin sw)
{
   int i, j, scolour, bcolour, mode, hexValue;

   for(i=0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         scolour = teleTextCells[i][j].sixelColour;
         bcolour = teleTextCells[i][j].background;
         mode = teleTextCells[i][j].mode;
         hexValue = teleTextCells[i][j].hexValue;

         if(mode == contiguous_mode || mode == separate_mode){
            check_sixel_details(&sw, teleTextCells[i][j].sixel, j* FNTWIDTH, i* FNTHEIGHT, scolour, bcolour, mode);
         }
         if(teleTextCells[i][j].heldgraphic == hold_graphics){
            check_sixel_details(&sw, teleTextCells[i][j].sixel, j* FNTWIDTH, i* FNTHEIGHT, scolour, bcolour, mode);
         }
      }
   }

}

/* loops though sixel string and checks where to populate with a SDL rectangle */
void check_sixel_details(SDL_Simplewin *sw, int sixel[][SIXELCOL], int x, int y, int scolour, int bcolour, int mode)
{
   int i, j, sixelValue, position_x, position_y;

   position_x = x;
   position_y = y;

   for(i = 0; i < SIXELROW; i++){
      for(j = 0; j < SIXELCOL; j++){

         sixelValue = sixel[i][j];

         x = position_x + RWIDTH * j;
         y = position_y + RHEIGHT * i;

         if(sixelValue != 0 ){
            create_SDL_rectangle(sw, x, y, scolour, bcolour, mode);
         }      
      }
   }
}

void create_SDL_rectangle(SDL_Simplewin *sw, int x, int y, int scolour, int bcolour, int mode)
{
   SDL_Rect rectangle, empty_rect;
   int boarder_colour;

   rectangle.w = RWIDTH;
   rectangle.h = RHEIGHT;

   empty_rect.w = RWIDTH;
   empty_rect.h = RHEIGHT;

   rectangle.x = x;
   rectangle.y = y;
   match_graphic_colour(sw, scolour);
   SDL_RenderFillRect(sw -> renderer, &rectangle);

   empty_rect.x = x;
   empty_rect.y = y;

   /* if in separate mode then change the rectangle to match background colour*/
   boarder_colour = change_rect_colour(mode, scolour, bcolour);
   match_graphic_colour(sw, boarder_colour);
   SDL_RenderDrawRect(sw -> renderer, &empty_rect);

}

int change_rect_colour(int mode, int scolour, int bcolour)
{
   if(mode == separate_mode){
      return bcolour;     
   }
   else{
      return scolour;
   }
}

void Neill_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy, int fcolour, int bcolour)
{
   unsigned x, y;
   for(y = 0; y < FNTHEIGHT; y++){
      for(x = 0; x < FNTWIDTH; x++){
         if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){

            if(is_text_colour(fcolour)){
               match_colour(sw, fcolour);
            }
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);

         }
         else{
            match_colour(sw, bcolour);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
         }
      }
   }
}

void double_height_top(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy, int fcolour, int bcolour)
{
   unsigned x, y;
   unsigned int height = FNTHEIGHT/2;

   for(y = 0; y < height; y++){
      for(x = 0; x < FNTWIDTH; x++){
         if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
            if(is_text_colour(fcolour)){
               match_colour(sw, fcolour);
            }
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy +1);     
         }
         else{
            match_colour(sw, bcolour);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy +1);
         }
     }
  }
}

void double_height_bottom(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy, int fcolour, int bcolour)
{
   unsigned x, y;
   unsigned int height = FNTHEIGHT/2;

   for(y = height; y < FNTHEIGHT; y++){
      for(x = 0; x < FNTWIDTH; x++){
         if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
            if(is_text_colour(fcolour)){
               match_colour(sw, fcolour);
            }
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy +1);     
         }
         else{
            match_colour(sw, bcolour);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y*2 + oy +1);
         }
     }
  }
}

int is_double_height(int value)
{
   if(value == double_height){
      return 1;
   }
   return 0;
}

int is_text_colour(int value)
{
   if(value >= red && value <= white){
      return 1;
   }
   return 0;
}

int is_background_colour(int value)
{
   if(value >= red && value <= white){
      return 1;
   }
   if(value == black_background){
      return 1;
   }
   return 0;
}

/* converts blast through text graphics colour to alphanumeric text colour*/
int blast_colour_change(int colour)
{
   if(colour == red_graphic){ colour = red;}
   if(colour == green_graphic){ colour = green;}
   if(colour == yellow_graphic){ colour = yellow;}
   if(colour == blue_graphic){ colour = blue;}
   if(colour == magenta_graphic){ colour = magenta;}
   if(colour == cyan_graphic){ colour = cyan;}
   if(colour == white_graphic){ colour = white;}

   return colour;
}

void match_colour(SDL_Simplewin *sw, int colour)
{
   int r = 0, g = 0, b = 0;

   if(colour == red){ create_colour(sw, MAXCOLOUR, g, b);}
   if(colour == green){ create_colour(sw, r, MAXCOLOUR, b);}
   if(colour == yellow){ create_colour(sw, MAXCOLOUR, MAXCOLOUR, b);}
   if(colour == blue){ create_colour(sw, r, g, MAXCOLOUR);}
   if(colour == magenta){ create_colour(sw, MAXCOLOUR, g, MAXCOLOUR);}
   if(colour == cyan){ create_colour(sw, r, MAXCOLOUR, MAXCOLOUR);}
   if(colour == white){ create_colour(sw, MAXCOLOUR, MAXCOLOUR, MAXCOLOUR);}
   if(colour == black_background){ create_colour(sw, r, g, b);}

}

void match_graphic_colour(SDL_Simplewin *sw, int colour)
{
   int r = 0, g = 0, b = 0;

   if(colour == red_graphic){ create_colour(sw, MAXCOLOUR, g, b);}
   if(colour == green_graphic){ create_colour(sw, r, MAXCOLOUR, b);}
   if(colour == yellow_graphic){ create_colour(sw, MAXCOLOUR, MAXCOLOUR, b);}
   if(colour == blue_graphic){ create_colour(sw, r, g, MAXCOLOUR);}
   if(colour == magenta_graphic){ create_colour(sw, MAXCOLOUR, g, MAXCOLOUR);}
   if(colour == cyan_graphic){ create_colour(sw, r, MAXCOLOUR, MAXCOLOUR);}
   if(colour == white_graphic){ create_colour(sw, MAXCOLOUR, MAXCOLOUR, MAXCOLOUR);}
   if(colour == black_background){ create_colour(sw, r, g, b);}

}

void create_colour(SDL_Simplewin *sw, int r, int g, int b)
{
   Neill_SDL_SetDrawColour(sw, r, g, b);
}

void create_background_colour(SDL_Simplewin *sw, int r, int g, int b)
{
   Neill_SDL_SetDrawColour(sw, r, g, b);
}


/* NEIL'S SDL FUCNTIONS */

void Neill_SDL_Init(SDL_Simplewin *sw)
{
   if(SDL_Init(SDL_INIT_VIDEO) != 0){
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->finished = 0;

   sw->win= SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
   if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   /* Set screen to black */
   Neill_SDL_SetDrawColour(sw, 0, 0, 0);
   SDL_RenderClear(sw->renderer);
   SDL_RenderPresent(sw->renderer);

}

/* Gobble all events & ignore most */
void Neill_SDL_Events(SDL_Simplewin *sw)
{
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             sw->finished = 1;
       }
    }
}

/* Trivial wrapper to avoid complexities of renderer & alpha channels */
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b)
{
   SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname)
{

    FILE *fp = fopen(fname, "rb");
    size_t itms;
    if(!fp){
       fprintf(stderr, "Can't open Font file %s\n", fname);
       exit(1);
   }
   itms = fread(fontdata, sizeof(fntrow), FNTCHARS*FNTHEIGHT, fp);
   if(itms != FNTCHARS*FNTHEIGHT){
       fprintf(stderr, "Can't read all Font file %s (%d) \n", fname, (int)itms);
       exit(1);
   }
   fclose(fp);

}

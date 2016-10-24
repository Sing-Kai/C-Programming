#include <stdio.h>
#include <stdlib.h>
#include "textngraphics.h"
#include "html.h"

void create_html(cell teleTextCells[][COL]);
void end_tags(FILE *fPointer);
void start_tags(FILE *fPointer);
void break_tag(FILE *fPointer);
void create_line(FILE *fPointer);
void new_line(FILE *fPointer);
void create_html_text(FILE *fPointer, cell teleTextCells[][COL]);
void parastart_tag(FILE *fPointer);
void paraend_tag(FILE *fPointer);
void add_character(FILE *fPointer, char text);
void whitefont(FILE *fPointer);
void fontend(FILE *fPointer);
void default_fcolour(FILE *fPointer);
void set_default_colour(FILE *fPointer);
void default_bcolour(FILE *fPointer);
void style_start_tag(FILE *fPointer);
void style_end_tag(FILE *fPointer);
void set_default_colour(FILE *fPointer);
void span_start_tag(FILE *fPointer);
void span_end_tag(FILE *fPointer);
void find_font_colour(FILE *fPointer, int fcolour);
void change_font_colour(FILE *fPointer, int fcolour, int previous_fcolour);
void span_font_style(FILE *fPointer, char *colour);
void span_colour_tag(FILE *fPointer, char *colour);
void endbrace_tag(FILE *fPointer);
void stylecolour_tag(FILE *fPointer);
void change_background_colour(FILE *fPointer, int bcolour, int previous_bcolour);
void find_bcolour(FILE *fPointer, int bcolour);
void span_bcolour_style(FILE *fPointer, char *colour);

void create_html(cell teleTextCells[][COL])
{
   FILE *fPointer;
   fPointer = fopen("teletext_html.html", "w");

   start_tags(fPointer);
   set_default_colour(fPointer);
   create_html_text(fPointer, teleTextCells);
   end_tags(fPointer);

   fclose(fPointer);
}

/* defaults to black background */
void set_default_colour(FILE *fPointer)
{
   style_start_tag(fPointer);
   default_bcolour(fPointer);
   style_end_tag(fPointer);
}

void create_html_text(FILE *fPointer, cell teleTextCells[][COL])
{
   int i, j;
   char text;
   int fcolour;
   int bcolour;
   int previous_fcolour;
   int previous_bcolour;

   for(i = 0; i < ROW; i++){

      parastart_tag(fPointer);
      whitefont(fPointer);

      for(j = 0; j < COL; j++){
         fcolour = teleTextCells[i][j].foreground;
         bcolour = teleTextCells[i][j].background;
         previous_fcolour = teleTextCells[i][j-1].foreground;
         previous_bcolour = teleTextCells[i][j-1].background;
         text = teleTextCells[i][j].text;
         change_background_colour(fPointer, bcolour, previous_bcolour);
         change_font_colour(fPointer, fcolour, previous_fcolour);
         add_character(fPointer, text);    
      }
      fontend(fPointer);
      paraend_tag(fPointer);
   }
}

void change_background_colour(FILE *fPointer, int bcolour, int previous_bcolour)
{
   if(bcolour != previous_bcolour){
      find_bcolour(fPointer, bcolour);         
   }
   if(bcolour == black_background && previous_bcolour != black_background){
      fprintf(fPointer, SPANEND);
   }
   
}

void find_bcolour(FILE *fPointer, int bcolour)
{
   if(bcolour == red){
      span_bcolour_style(fPointer, REDCOLOUR);   
   }
   if(bcolour == green){  
      span_bcolour_style(fPointer, GREENCOLOUR); 
   }
   if(bcolour == blue){ 
      span_bcolour_style(fPointer, BLUECOLOUR);   
   }
   if(bcolour == yellow){   
      span_bcolour_style(fPointer, YELLOWCOLOUR);  
   }
   if(bcolour == magenta){
      span_bcolour_style(fPointer, MAGENTACOLOUR);    
   }   
   if(bcolour == cyan){
      span_bcolour_style(fPointer, CYANCOLOUR);   
   }  
}

/* creates first part of background colour change tag*/
void span_bcolour_style(FILE *fPointer, char *colour)
{
   span_start_tag(fPointer);
   fprintf(fPointer, STYLE);
   fprintf(fPointer, SPANBCOLOUR);
   span_colour_tag(fPointer, colour);
   endbrace_tag(fPointer); 
}


void change_font_colour(FILE *fPointer, int fcolour, int previous_fcolour)
{
   if(previous_fcolour != fcolour){
      find_font_colour(fPointer, fcolour);
   }
   if(fcolour == white && previous_fcolour != white){
      fprintf(fPointer, SPANEND);
   }
}


void find_font_colour(FILE *fPointer, int fcolour)
{
   if(fcolour == red){
      span_font_style(fPointer, REDCOLOUR);   
   }
   if(fcolour == green){  
      span_font_style(fPointer, GREENCOLOUR); 
   }
   if(fcolour == blue){ 
      span_font_style(fPointer, BLUECOLOUR);   
   }
   if(fcolour == yellow){   
      span_font_style(fPointer, YELLOWCOLOUR);  
   }
   if(fcolour == magenta){
      span_font_style(fPointer, MAGENTACOLOUR);    
   }   
   if(fcolour == cyan){
      span_font_style(fPointer, CYANCOLOUR);   
   }   

}

/* creates first part of font tag*/
void span_font_style(FILE *fPointer, char *colour)
{
   span_start_tag(fPointer);
   stylecolour_tag(fPointer);
   span_colour_tag(fPointer, colour);
   endbrace_tag(fPointer);   
}

void span_colour_tag(FILE *fPointer, char *colour)
{
   fprintf(fPointer, colour);
}

void endbrace_tag(FILE *fPointer)
{
   fprintf(fPointer, ENDBRACE);
}

void stylecolour_tag(FILE *fPointer)
{
   fprintf(fPointer, STYLECOLOUR);
}

void span_start_tag(FILE *fPointer)
{
   fprintf(fPointer, SPANSTART);
}

void span_end_tag(FILE *fPointer)
{
   fprintf(fPointer, SPANEND);
}

/* writes individual chacters to file*/
void add_character(FILE *fPointer, char text)
{
   if(text == ASCIISPACE){ 
      fprintf(fPointer, HTMLSPACE);
   } 
   else{
      fputc(text, fPointer);  
   }
}

void whitefont(FILE *fPointer)
{ 
   fprintf(fPointer, FONTCOLOUR);
   fprintf(fPointer, WHITECOLOUR);
   fprintf(fPointer, ENDBRACE);
}

void fontend(FILE *fPointer)
{
   fprintf(fPointer, FONTEND);
}

void start_tags(FILE *fPointer)
{
   fprintf(fPointer, DOCTYPE);
   fprintf(fPointer, HTMLSTART);
   fprintf(fPointer, BODYSTART);
}

void end_tags(FILE *fPointer)
{
   fprintf(fPointer, BODYEND);
   fprintf(fPointer, HTMLEND);
}

void break_tag(FILE *fPointer)
{   
   fprintf(fPointer, BREAKTAG);   
}

void new_line(FILE *fPointer)
{
   fprintf(fPointer, "\n");
}

void parastart_tag(FILE *fPointer)
{
   fprintf(fPointer, PARASTART);
}

void paraend_tag(FILE *fPointer)
{
   fprintf(fPointer, PARAEND);
}

void default_bcolour(FILE *fPointer)
{
   fprintf(fPointer, BCOLOUR);
}

void default_fcolour(FILE *fPointer)
{
   fprintf(fPointer, FCOLOUR);
}

void style_start_tag(FILE *fPointer)
{
   fprintf(fPointer, STYLESTART);
}

void style_end_tag(FILE *fPointer)
{
   fprintf(fPointer, STYLEEND);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "textngraphics.h"
#include "test.h"
#include "html.h"

void check_input_commands(int argc);
void read_hexfile(char *file, hex hexArray[][COL]);
void initialize_hexArray(hex hexArray[][COL]);
void initialize_teleTextCells(cell teleTextCells[][COL]);
void initialize_sixel(int sixel[][SIXELCOL]);
void update_sixel_string(int sixel[][SIXELCOL], int value);
void update_text_details(cell teleTextCells[][COL], hex hexArray[][COL]);
void update_background_colour(cell teleTextCells[][COL]);
void update_mode(cell teleTextCells[][COL]);
void update_heldgraphic(cell teleTextCells[][COL]);
void update_holdgraphic_sixel(cell teleTextCells[][COL]);
void update_sixel_details(cell teleTextCells[][COL]);
void update_all_teletextcells(cell teleTextCells[][COL], hex hexArray[][COL]);
void print_struct_details(cell teleTextCells[][COL]);
void print_hexArray(hex hexArray[][COL]);
void print_sixel(int sixel[][SIXELCOL]);
int get_mode(int value, int mode);
int get_sixel_colour(int value, int sixelColour);
int get_foreground_colour(int value, int colour);
int get_height(int value, int height);
int is_alphanumeric(int value);
int is_graphic_code(int value);
int is_graphic_colour(int value);
int is_graphic_mode(int value);
int is_blast_through(int value);
int is_heldgraphic(int value, int hold_value);
int make_ascii(int value);
int check_remainder(int x, int y); 
int is_black_background(int value);
int is_new_background(int value);
int is_control_code(int value);


int main(int argc, char *argv[])
{
   hex hexArray[ROW][COL];
   cell teleTextCells[ROW][COL];

   check_input_commands(argc);
   initialize_hexArray(hexArray);
   read_hexfile(argv[1], hexArray);
   initialize_teleTextCells(teleTextCells);

   update_all_teletextcells(teleTextCells, hexArray);

   /* display SDL*/
   create_graphics(teleTextCells);

   /* run tests*/
   run_teletext_test_functions();

   /* outputs html file*/
   create_html(teleTextCells);

   return(0);
}

void check_input_commands(int argc)
{
   int totalCommands = 2;

   if (argc != totalCommands){
      printf("Please input correct number of commands!\n");
      exit(1);
   }
}

void read_hexfile(char *file, hex hexArray[][COL])
{
   FILE *filePointer;

   filePointer = fopen(file, "rb");

   if(filePointer == NULL){
      perror("Failed to open.\n");
      exit(EXIT_FAILURE);
   }

   fread(hexArray, HEX, ARRAYMAX, filePointer);
   fclose(filePointer);
}

/*calls functions that update details of teleTextCell structure */
void update_all_teletextcells(cell teleTextCells[][COL], hex hexArray[][COL])
{
   update_text_details(teleTextCells, hexArray);
   update_background_colour(teleTextCells);
   update_mode(teleTextCells);
   update_heldgraphic(teleTextCells);
   update_sixel_details(teleTextCells);
}

void print_hexArray(hex hexArray[][COL])
{
   int i, j;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){
         printf("%x ", hexArray[i][j]);

      }
   printf("\n");
   }
}

void initialize_hexArray(hex hexArray[][COL])
{
   int i, j;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){
         hexArray[i][j] = 0;
      }
   }
}

void initialize_teleTextCells(cell teleTextCells[][COL])
{
   int i, j;

   for(i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){
         teleTextCells[i][j].text = 0;
         teleTextCells[i][j].hexValue = 0;
         teleTextCells[i][j].foreground = white;
         teleTextCells[i][j].background = 0;
         teleTextCells[i][j].sixelColour = 0;
         teleTextCells[i][j].height = 0;
         teleTextCells[i][j].mode = 0;
         teleTextCells[i][j].heldgraphic = 0;
         initialize_sixel(teleTextCells[i][j].sixel);
      }
   }
}

void print_struct_details(cell teleTextCells[][COL])
{
   int i, j;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

        printf("%d ", teleTextCells[i][j].text);
/*
        printf("%d ", teleTextCells[i][j].foreground);
        printf("%d ", teleTextCells[i][j].height); 
        printf("%d ", teleTextCells[i][j].background);
        printf("%d ", teleTextCells[i][j].mode); 
        printf("%d ", teleTextCells[i][j].heldgraphic);
        printf("%s ", teleTextCells[i][j].sixel);
        printf("%d ", teleTextCells[i][j].sixelColour);
*/

      }
      printf("\n");
   }
}

/* updates teleTextCell with hexArray data */
void update_text_details(cell teleTextCells[][COL], hex hexArray[][COL])
{
   int i, j;
   int text = 0, value = 0, height = 0;
   int foreground_colour = white;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         value = hexArray[i][j];
         text = make_ascii(value);

         teleTextCells[i][j].text = text;
         teleTextCells[i][j].hexValue = value;

         foreground_colour = get_foreground_colour(value, foreground_colour);
         teleTextCells[i][j].foreground = foreground_colour;
       
         height = get_height(value, height);
         teleTextCells[i][j+1].height = height;

      }
   }
}

int make_ascii(int value)
{
   int asciiValue = 0;

   if(value < CONTROLSTART){
      return value;
   }
   else if(is_control_code(value)){
      asciiValue = ASCIISPACE;
      return asciiValue;
   }
   else{
      asciiValue = value - CONTROLSTART;
   }

   return asciiValue;
}

int is_control_code(int value)
{
   if(CONTROLSTART <= value && value <= CONTROLEND){
      return 1;
   }
   return 0;
}

int get_foreground_colour(int value, int colour)
{
   if(value == red){return red;}
   if(value == green){return green;}
   if(value == yellow){return yellow;}
   if(value == blue){return blue;}
   if(value == magenta){return magenta;}
   if(value == cyan){return cyan;}
   if(value == white){return white;}

   return colour;
}

int get_height(int value, int height)
{
   if(value == single_height){
      return 0;
   }
   if(value == double_height){
      return double_height;
   }
   return height;
}

void update_background_colour(cell teleTextCells[][COL])
{
   int i, j;
   int value = 0, previous_value = 0;
   int background_colour;

   for (i = 0; i < ROW; i++){
      
      background_colour = black_background;

      for (j = 0; j < COL; j++){

         value = teleTextCells[i][j].hexValue;
         previous_value = teleTextCells[i][j-1].hexValue;
         
         if(is_new_background(value) && is_alphanumeric(previous_value)){
            background_colour = previous_value;      
         }
         /* defaults to white background colour if new background code present but no text colour code before */
         if(is_new_background(value) && !(is_alphanumeric(previous_value))){
            background_colour = white;
         }
         if(is_black_background(value)){
            background_colour = black_background;
         }

         teleTextCells[i][j].background = background_colour;
      }
   }
}

int is_black_background(int value)
{
   if(value == black_background){
      return 1;
   }
   return 0;
}

int is_new_background(int value)
{
   if(value == new_background){
      return 1;
   }
   return 0;
}

int is_alphanumeric(int value)
{
   if(value >= red && value <= white){
      return 1;
   }
   return 0;
}

int is_graphic_code(int value)
{
   if(is_graphic_colour(value)){
      return 1;
   }
   if(is_graphic_mode(value)){
      return 1;
   }
   return 0;
}

int is_graphic_colour(int value)
{
   if(value >= red_graphic && value <= white_graphic){
      return 1;
   }
   return 0;
}

int is_graphic_mode(int value)
{
   if(value == contiguous_mode || value == separate_mode){
      return 1;
   }
   return 0;
}

void update_mode(cell teleTextCells[][COL])
{
   int i, j;
   int value = 0, mode = 0;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         value = teleTextCells[i][j].hexValue;
         mode = get_mode(value, mode);
         teleTextCells[i][j+1].mode = mode;

         if(is_alphanumeric(value)){
            teleTextCells[i][j].mode = alphanumeric_mode;
         }
         if(is_graphic_code(value)){
            teleTextCells[i][j].mode = alphanumeric_mode;
         }
      }
   }
}

int get_mode(int value, int mode)
{
   if(value == contiguous_mode){
      mode = contiguous_mode;
   }
   if(is_graphic_colour(value)){
      mode = contiguous_mode;
   }
   if(value == separate_mode){
      mode = separate_mode;
   }
   if(is_alphanumeric(value)){
      mode = alphanumeric_mode;
   }
   return mode;
}

void update_heldgraphic(cell teleTextCells[][COL])
{
   int i, j;
   int value = 0, hold_mode = 0;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         value = teleTextCells[i][j].hexValue;
         hold_mode = is_heldgraphic(value, hold_mode);
         teleTextCells[i][j].heldgraphic = hold_mode;
         update_holdgraphic_sixel(teleTextCells);

      }
   }
}

/* If in held graphics mode copy previous sixel and update in teleTextCells*/
void update_holdgraphic_sixel(cell teleTextCells[][COL])
{
   int i, j;
   int hexValue = 0, sixel_value = 0, colour = 0;

   for (i = 0; i < ROW; i++){   
      for (j = 0; j < COL; j++){

         hexValue = teleTextCells[i][j].hexValue;

         if(hexValue == hold_graphics){
            sixel_value = teleTextCells[i][j-1].hexValue;
         }

         if(teleTextCells[i][j].heldgraphic == hold_graphics){
            colour = hexValue;
            teleTextCells[i][j].sixelColour = colour;
            update_sixel_string(teleTextCells[i][j].sixel, sixel_value);
         }
      }
   }
}

int is_heldgraphic(int value, int hold_mode)
{
   if(value == hold_graphics){
      return hold_graphics;
   }
   if(is_alphanumeric(value)){
      return 0;
   }
   if(value == release_graphics){
      return 0;
   }
   if(is_graphic_mode(value)){
      return 0;
   }

   return hold_mode;
}

void initialize_sixel(int sixel[][SIXELCOL])
{
   int i, j;

   for (i = 0; i < SIXELROW; i++){
      for (j = 0; j < SIXELCOL; j++){
         sixel[i][j] = 0;
      }
   }
}

void print_sixel(int sixel[][SIXELCOL])
{
   int i, j;

   for (i = 0; i < SIXELROW; i++){
      for (j = 0; j < SIXELCOL; j++){
         printf("%d ", sixel[i][j]);
      }
   }
}

/* updates the sixel mode and sixel colour */
void update_sixel_details(cell teleTextCells[][COL])
{
   int i, j;
   int mode = 0, hexValue = 0, scolour = 0;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         mode = teleTextCells[i][j].mode;
         hexValue = teleTextCells[i][j].hexValue;
         scolour = get_sixel_colour(hexValue, scolour);

         if(is_graphic_mode(mode) && !(is_blast_through(hexValue))){
            update_sixel_string(teleTextCells[i][j].sixel, hexValue);
         }
         if(is_graphic_mode(mode)){
            teleTextCells[i][j].sixelColour = scolour;
         }         
      }
   }
}

int get_sixel_colour(int value, int sixelColour)
{
   if(is_graphic_colour(value)){
      sixelColour = value;
   }
   return sixelColour;
}

/* populates the sixel string according to the value*/
void update_sixel_string(int sixel[][SIXELCOL], int value)
{
   int i, j;

   for (i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         value -= GRAPHICSTART;

         if(check_remainder(value, SIXELSIXTYFOUR)){value -= SIXELSIXTYFOUR; sixel[2][1] = 1;}
         if(check_remainder(value, SIXELSIXTEEN)){value -= SIXELSIXTEEN; sixel[2][0] = 1;}
         if(check_remainder(value, SIXELEIGHT)){value -= SIXELEIGHT; sixel[1][1] = 1;}
         if(check_remainder(value, SIXELFOUR)){value -= SIXELFOUR; sixel[1][0] = 1;}
         if(check_remainder(value, SIXELTWO)){value -= SIXELTWO; sixel[0][1] = 1;}
         if(check_remainder(value, SIXELONE)){value -= SIXELONE; sixel[0][0] = 1;}

      }
   }
}

int check_remainder(int x, int y)
{
   if((x-y) >= 0){
      return 1;
   }
   return 0;
}

int is_blast_through(int value)
{
   if(value >= BLAST_TXT_START && value <= BLAST_TXT_END){
      return 1;
   }
   return 0;
}

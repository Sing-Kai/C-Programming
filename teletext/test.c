#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "textngraphics.h"


struct test {
   char text;
   int hexValue;
   int foreground;
   int background;
   int sixelColour;
   int height;
   int mode;
   int heldgraphic;
   int sixel[SIXELROW][SIXELCOL];
};

void run_teletext_test_functions(void);
void run_graphics_test_functions(void);
void test_get_height(void);
void test_is_alphanumeric(void);
void test_is_graphic_mode(void);
void test_is_graphic_colour(void);
void test_is_blast_through(void);
void test_is_heldgraphic(void);
void test_make_ascii(void);
void test_get_mode(void);
void test_initialize_teleTextCells(void);
void test_initialize_sixel(void);
void test_initialize_hexArray(void);
void test_check_remainder(void);
void test_update_mode(void);
void test_is_background_colour(void);
void test_is_text_colour(void);
void test_is_double_height(void);
void test_blast_colour_change(void);
void test_change_rect_colour(void);
void test_is_control_code(void);
void test_is_black_background(void);
void test_is_new_background(void);

void run_teletext_test_functions(void)
{
   test_get_height();
   test_is_alphanumeric();
   test_is_graphic_mode();
   test_is_graphic_colour();
   test_is_blast_through();
   test_is_heldgraphic();
   test_is_black_background();
   test_is_control_code();
   test_is_new_background();
   test_make_ascii();
   test_get_mode();
   test_initialize_teleTextCells();
   test_initialize_sixel();
   test_initialize_hexArray();
   test_check_remainder();
   test_update_mode();

}

void run_graphics_test_functions(void)
{
   test_is_background_colour();
   test_is_text_colour();
   test_is_double_height();
   test_blast_colour_change();
   test_change_rect_colour();

}

void test_get_height(void)
{
   int actual; 
   int expect_double = double_height;
   int expect_zero = 0;

   if((actual = get_height(expect_double, expect_double)) != expect_double) {
      printf("Test failed - get_height. Expected %d, got %d...\n", expect_double, actual);
   }
   else if((actual = get_height(expect_zero, expect_zero)) != expect_zero){
      printf("Test failed - get_height. Expected %d, got %d...\n", expect_zero, actual);      
   }
   else{
      printf("Test passed - get_height\n");
   }
}

void test_is_alphanumeric(void)
{
   int actual; 
   int expect_one = 1;
   int expect_zero = 0;   

   if((actual = is_alphanumeric(red)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_alphanumeric(green)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);      
   }
   else if((actual = is_alphanumeric(blue)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_alphanumeric(yellow)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_alphanumeric(magenta)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_alphanumeric(cyan)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);
   }    
   else if((actual = is_alphanumeric(white)) != expect_one){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_alphanumeric(expect_zero)) != expect_zero){
      printf("Test failed - is_alphanumeric. Expected %d, got %d...\n", expect_zero, actual);
   }                   
   else{
      printf("Test passed - is_alphanumeric\n");
   }
}

void test_is_graphic_mode(void)
{
   int actual; 
   int expect_one = 1;
   int expect_zero = 0;   

   if((actual = is_graphic_mode(contiguous_mode)) != expect_one){
      printf("Test failed - is_graphic_mode. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_graphic_mode(separate_mode)) != expect_one){
      printf("Test failed - is_graphic_mode. Expected %d, got %d...\n", expect_one, actual);      
   }
   else if((actual = is_graphic_mode(expect_zero)) != expect_zero){
      printf("Test failed - is_graphic_mode. Expected %d, got %d...\n", expect_zero, actual);
   }                   
   else{
      printf("Test passed - is_graphic_mode\n");
   }
}

void test_is_graphic_colour(void)
{
   int actual; 
   int expect_one = 1;
   int expect_zero = 0;   

   if((actual = is_graphic_colour(red_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_graphic_colour(green_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_one, actual);      
   }
   else if((actual = is_graphic_colour(yellow_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_zero, actual);
   }
   else if((actual = is_graphic_colour(blue_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_zero, actual);
   }          
   else if((actual = is_graphic_colour(magenta_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_zero, actual);
   }          
   else if((actual = is_graphic_colour(cyan_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_zero, actual);
   }          
   else if((actual = is_graphic_colour(white_graphic)) != expect_one){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_zero, actual);
   }          
   else if((actual = is_graphic_colour(expect_zero)) != expect_zero){
      printf("Test failed - is_graphic_colour. Expected %d, got %d...\n", expect_zero, actual);
   }                             
   else{
      printf("Test passed - is_graphic_colour\n");
   }
}

void test_is_blast_through(void)
{
   int actual; 
   int expect_one = 1;

   if((actual = is_blast_through(BLAST_TXT_START)) != expect_one) {
      printf("Test failed - is_blast_through. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_blast_through(BLAST_TXT_END)) != expect_one){
      printf("Test failed - is_blast_through. Expected %d, got %d...\n", expect_one, actual);      
   }
   else{
      printf("Test passed - is_blast_through\n");
   }
}

/* doesn't test functions that have been tested already'*/
void test_is_heldgraphic(void)
{
   int actual; 
   int expect_hold = hold_graphics;
   int expect_release = release_graphics;

   if((actual = is_heldgraphic(expect_hold, expect_hold)) != expect_hold) {
      printf("Test failed - is_heldgraphic. Expected %d, got %d...\n", expect_hold, actual);
   }
   else if((actual = is_heldgraphic(expect_release, expect_release)) != 0){
      printf("Test failed- is_heldgraphic. Expected %d, got %d...\n", 0, actual);      
   }
   else{
      printf("Test passed - is_heldgraphic\n");
   }

}

void test_make_ascii(void)
{
   int actual; 
   int expect = ASCIISPACE;

   if((actual = make_ascii(CONTROLSTART)) != expect) {
      printf("Test failed - make_ascii. Expected %d, got %d...\n", expect, actual);
   }
   else if((actual = make_ascii(CONTROLEND)) != expect){
      printf("Test failed - make_ascii. Expected %d, got %d...\n", expect, actual);      
   }
   else{
      printf("Test passed - make_ascii\n");
   }

}

void test_get_mode(void)
{
   int actual; 
   int expect_c = contiguous_mode;
   int expect_s = separate_mode;
   int expect_zero = 0;

   if((actual = get_mode(expect_c, expect_c)) != expect_c) {
      printf("Test failed - get_mode. Expected %d, got %d...\n", expect_c, actual);
   }
   else if((actual = get_mode(expect_s, expect_s)) != expect_s){
      printf("Test failed - get_mode. Expected %d, got %d...\n", expect_s, actual);      
   }
   else if((actual = get_mode(expect_zero, expect_zero)) != expect_zero){
      printf("Test failed - get_mode. Expected %d, got %d...\n", expect_zero, actual);      
   }
   else{
      printf("Test passed - get_mode\n");
   }

}


void test_initialize_teleTextCells(void)
{
   cell test_struct[ROW][COL];
   int i, j;
   int expect_zero = 0;
   
   initialize_teleTextCells(test_struct);

   for(i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         assert((test_struct[i][j].text) == expect_zero);
         assert((test_struct[i][j].hexValue) == expect_zero);
         assert((test_struct[i][j].foreground) == white);
         assert((test_struct[i][j].background) == expect_zero);
         assert((test_struct[i][j].sixelColour) == expect_zero);
         assert((test_struct[i][j].height) == expect_zero);
         assert((test_struct[i][j].mode) == expect_zero);
         assert((test_struct[i][j].heldgraphic) == expect_zero);

      }
   }

}

void test_initialize_sixel(void)
{
   int test_sixel[SIXELROW][SIXELCOL];
   int i, j;

   initialize_sixel(test_sixel);

   for (i = 0; i < SIXELROW; i++){
      for (j = 0; j < SIXELCOL; j++){
         assert(test_sixel[i][j] == 0);
      }
   }
}

void test_initialize_hexArray(void)
{
   hex test_array[ROW][COL];
   int i, j;
   
   initialize_hexArray(test_array);

   for(i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){
         assert(test_array[i][j] == 0);
      }
   }
}

void test_check_remainder(void)
{
   int x = 2;
   int y = 1;
   int expect_one = 1;
   int actual;
   
   if((actual = check_remainder(x, y)) != expect_one){
      printf("Test failed - check_remainder. Expected %d, got %d...\n", expect_one, actual);
   }
   else{
      printf("Test passed - check_remainder\n");
   }
}

void test_update_mode(void)
{
   cell test_struct[ROW][COL];
   int i, j;
   
   initialize_teleTextCells(test_struct);
   update_mode(test_struct);

   for(i = 0; i < ROW; i++){
      for (j = 0; j < COL; j++){

         if(test_struct[i][j].mode != alphanumeric_mode){
            assert((test_struct[i][j].mode) == contiguous_mode);
            assert((test_struct[i][j].mode) == separate_mode);
         }
         else{
            assert((test_struct[i][j].mode) == alphanumeric_mode);
         }
      }
   }
}

void test_is_background_colour(void)
{
   int actual; 
   int expect_one = 1;

   if((actual = is_background_colour(red)) != expect_one) {
      printf("Test failed - is_background_colour. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_background_colour(white)) != expect_one){
      printf("Test failed - is_background_colour. Expected %d, got %d...\n", expect_one, actual);      
   }
   else if((actual = is_background_colour(black_background)) != expect_one){
      printf("Test failed - is_background_colour. Expected %d, got %d...\n", expect_one, actual);      
   }   
   else{
      printf("Test passed - is_background_colour\n");
   }
}

void test_is_text_colour(void)
{
   int actual; 
   int expect_one = 1;

   if((actual = is_text_colour(red)) != expect_one) {
      printf("Test failed - is_text_colour. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_text_colour(white)) != expect_one){
      printf("Test failed - is_text_colour. Expected %d, got %d...\n", expect_one, actual);      
   }
   else{
      printf("Test passed - is_text_colour\n");
   }
}

void test_is_double_height(void)
{
   int actual; 
   int expect_one = 1;

   if((actual = is_double_height(double_height)) != expect_one) {
      printf("Test failed - is_double_height. Expected %d, got %d...\n", expect_one, actual);
   }
   else{
      printf("Test passed - is_double_height\n");
   }

}

void test_blast_colour_change(void)
{
   int actual; 
   int expect_red = red;
   int expect_white = white;
   int expect_zero = 0;

   if((actual = blast_colour_change(red_graphic)) != expect_red) {
      printf("Test failed - blast_colour_change. Expected %d, got %d...\n", expect_red, actual);
   }
   else if((actual = blast_colour_change(white_graphic)) != expect_white) {
      printf("Test failed - blast_colour_change. Expected %d, got %d...\n", expect_white, actual);
   }
   else if((actual = blast_colour_change(expect_zero)) != expect_zero) {
      printf("Test failed - blast_colour_change. Expected %d, got %d...\n", expect_zero, actual);
   }   
   else{
      printf("Test passed - blast_colour_change\n");
   }

}

void test_change_rect_colour(void)
{
   int actual; 
   int expect_one = 1;
   int expect_zero = 0;

   if((actual = change_rect_colour(separate_mode, expect_zero, expect_one)) != expect_one){
      printf("Test failed - change_rect_colour. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = change_rect_colour(expect_zero, expect_zero, expect_one))!= expect_zero){
      printf("Test failed - change_rect_colour. Expected %d, got %d...\n", expect_zero, actual);
   }
   else{
      printf("Test passed - change_rect_colour\n");
   }

}

void test_is_control_code(void)
{
   int actual; 
   int expect_one = 1;

   if((actual = is_control_code(CONTROLSTART)) != expect_one){
      printf("Test failed - is_control_code. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_control_code(CONTROLEND))!= expect_one){
      printf("Test failed - s_control_code. Expected %d, got %d...\n", expect_one, actual);
   }
   else{
      printf("Test passed - is_control_code\n");
   }
}



void test_is_black_background(void)
{
   int actual; 
   int expect_one = 1;
   int expect_zero = 0;

   if((actual = is_black_background(black_background)) != expect_one){
      printf("Test failed - is_black_background. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_control_code(expect_zero))!= expect_zero){
      printf("Test failed - is_black_background. Expected %d, got %d...\n", expect_zero, actual);
   }
   else{
      printf("Test passed - is_black_background\n");
   }
}

void test_is_new_background(void)
{
   int actual; 
   int expect_one = 1;
   int expect_zero = 0;

   if((actual = is_new_background(new_background)) != expect_one){
      printf("Test failed - is_new_background. Expected %d, got %d...\n", expect_one, actual);
   }
   else if((actual = is_new_background(expect_zero))!= expect_zero){
      printf("Test failed - is_new_background. Expected %d, got %d...\n", expect_zero, actual);
   }
   else{
      printf("Test passed - is_new_background\n");
   }
}


#include <SDL.h>
#include <math.h>

#define WWIDTH 800
#define WHEIGHT 600

/* Font stuff */
typedef unsigned short fntrow;
#define FNTWIDTH (sizeof(fntrow)*8)
#define FNTHEIGHT 18
#define FNTCHARS 96
#define FNT1STCHAR 32

#define SDL_8BITCOLOUR 256

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

/* All info required for windows / renderer & event loop */
struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
};
typedef struct SDL_Simplewin SDL_Simplewin;


/* teletext.c */
#define ROW 25
#define COL 40
#define ARRAYMAX 1000
#define HEX 8
#define CONTROLSTART 128
#define CONTROLEND 159
#define ASCIISPACE 32
#define GRAPHICSTART 160
#define SIXELONE 1
#define SIXELTWO 2
#define SIXELFOUR 4
#define SIXELEIGHT 8
#define SIXELSIXTEEN 16
#define SIXELSIXTYFOUR 64
#define SIXELROW 3
#define SIXELCOL 2
#define FNTFILENAME "m7fixed.fnt"
#define BLAST_TXT_START 192
#define BLAST_TXT_END 223

struct cell {
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
typedef struct cell cell;

typedef unsigned char hex;

enum alphanumeric_colour {red = 129, green, yellow, blue, magenta, cyan, white};
enum height {single_height = 140, double_height};
enum graphics_colour {red_graphic = 145, green_graphic, yellow_graphic, blue_graphic, magenta_graphic, cyan_graphic, white_graphic};
enum mode {alphanumeric_mode, contiguous_mode = 153, separate_mode};
enum background_colour {black_background = 156, new_background};
enum heldgraphic_mode {hold_graphics = 158, release_graphics};

/* all teletext.c */
void initialize_sixel(int sixel[][SIXELCOL]);
void initialize_hexArray(hex hexArray[][COL]);
void initialize_teleTextCells(cell teleTextCells[][COL]);
void update_text_details(cell teleTextCells[][COL], hex hexArray[][COL]);
void update_background_colour(cell teleTextCells[][COL]);
void update_mode(cell teleTextCells[][COL]);
void update_heldgraphic(cell teleTextCells[][COL]);
void update_holdgraphic_sixel(cell teleTextCells[][COL]);
void update_sixel_details(cell teleTextCells[][COL]);
void update_sixel_string(int sixel[][SIXELCOL], int value);
void update_all_teletextcells(cell teleTextCells[][COL], hex hexArray[][COL]);
int is_alphanumeric(int value);
int is_graphic_code(int value);
int is_graphic_colour(int value);
int is_graphic_mode(int value);
int is_blast_through(int value);
int is_heldgraphic(int value, int hold_value);
int get_sixel_colour(int value, int sixelColour);
int get_mode(int value, int mode);
int get_foreground_colour(int value, int colour);
int get_height(int value, int height);
void check_input_commands(int argc);
void read_hexfile(char *file, hex hexArray[][COL]);
int make_ascii(int value);
void print_hexArray(hex hexArray[][COL]);
void print_sixel(int sixel[][SIXELCOL]);
void print_struct_hexArray(cell teleTextCells[][COL]);
int check_remainder(int x, int y);
int is_black_background(int value);
int is_new_background(int value);
int is_control_code(int value);

/* graphics.c */
int create_graphics(cell teleTextCells[][COL]);
int is_blast_through(int value);
int is_background_colour(int value);
int is_double_height(int value);
int is_top_double(cell teleTextCells[][COL], int value);
int is_bottom_double(int value);
int is_text_colour(int fcolour);
int blast_colour_change(int colour);
int change_rect_colour(int mode, int scolour, int bcolour);

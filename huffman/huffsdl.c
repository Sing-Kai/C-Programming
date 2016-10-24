#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "neillsdl2.h"

#define ASCIIMAX 128
#define PARENTSYMBOL '#'
#define BYTES 8
#define SPACE ' '
#define LEFT '|'
#define RIGHT '-'
#define COL 40
#define ROW 40
#define FNTFILENAME "m7fixed.fnt"
#define RADIUS 5

struct node {
   int charValue;
   int frequency;
   struct node *left;
   struct node *right;
   struct node *next;
};

typedef struct node node;

node *createLinkList(char *file, node *headNode);
void checkInputCommands(int argc);
node *initializeNode(int value, int frequency);
void printList(node *temp);
node *mergeSort(node *headNode);
node *merge(node *headOne, node *headTwo);
node *createParentNode(node *firstNode, node *secondNode);
node *newLinkedList(node *headNode, node *parentNode);
node *createBinaryTree(node *headNode);
int findListLength(node *temp);
int maxDepth(node *parentNode);
int getCharLevel(node *parentNode, int value);
int getCharLevelRecur(node *parentNode, int value, int level);
void printAll(node *parentNode, node *secondList);
void printTotalByte(node *parentNode, node *secondList);
int printHuffCode(node *parentNode, int value, int counter);
void printSpace(int spaces);
int makeUpperCase(int letter);
void createArray(char array[][COL]);
void printArray(char array[][COL]);
void drawTreeInArray(node *parent, char array[][COL], int x, int y);
int countRightNodes(node *parent);
void freeBinaryTree(node *parentNode);

/* SDL functions below*/
void printSDLarray(char array[][COL], SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT]);
void printSDLcomment(SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT]);
void drawSDLCircle(SDL_Renderer *rend, int cx, int cy, int r);
void drawLines(SDL_Renderer *rend, int cx, int cy, int dx, int dy);
void drawNodes(char array[][COL], SDL_Simplewin sw);
void makeGreen(SDL_Simplewin sw);
void makeRed(SDL_Simplewin sw);
void makeBlack(SDL_Simplewin sw);
void drawChar(SDL_Simplewin *sw, fntrow fontdata[][FNTHEIGHT], unsigned char chr, int ox, int oy);

int main(int argc, char *argv[])
{
   node *firstList = NULL;
   node *parentNode = NULL;
   char array[ROW][COL];
   SDL_Simplewin sw;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   checkInputCommands(argc);

   firstList = createLinkList(argv[1], firstList);

   parentNode = createBinaryTree(firstList);
   createArray(array);
   drawTreeInArray(parentNode, array, 0, 0);

   Neill_SDL_Init(&sw);
   Neill_SDL_ReadFont(fontdata, FNTFILENAME);

   printSDLcomment(sw, fontdata);
   printSDLarray(array, sw, fontdata);
   drawNodes(array, sw);

   freeBinaryTree(parentNode);

   SDL_RenderPresent(sw.renderer);
   SDL_UpdateWindowSurface(sw.win);
   SDL_Delay(10000);

   SDL_Quit();

   return(EXIT_SUCCESS);

}

void printSDLarray(char array[][COL], SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT])
{
   int i;

   for(i=0; i<ROW; i++){   
      Neill_SDL_DrawString(&sw, fontdata, array[i], 50, 80 + i * FNTHEIGHT);     
   }

}

/* walks through array to and draws objects onto SDL screen */
void drawNodes(char array[][COL], SDL_Simplewin sw)
{
   int i, j;

   for(i=0; i<ROW; i++){     
      for(j=0; j<COL; j++){
         /* if char is a node symbol draw green circle*/
         if (array[i][j] == PARENTSYMBOL){
            makeGreen(sw);           
            drawSDLCircle(sw.renderer, 50 + j*FNTHEIGHT, 350 + i*FNTHEIGHT, RADIUS);
         }  
         /* if char is a right symbol draw a line*/
         else if (array[i][j] == RIGHT){
            makeBlack(sw);
            drawLines(sw.renderer, 50 + j*FNTHEIGHT , 350 + i*FNTHEIGHT, 55 + j*FNTHEIGHT , 350 + i*FNTHEIGHT); 
         }
         /* if char is a left symbol draw a line*/
         else if(array[i][j] == LEFT){
            makeBlack(sw);
            drawLines(sw.renderer, 50 +j*FNTHEIGHT , 350 + i*FNTHEIGHT, 50 + j*FNTHEIGHT , 355 + i*FNTHEIGHT);
         }
         /* if char is a character draw a red circle*/
         else if(isalpha(array[i][j])){
            makeRed(sw);
            drawSDLCircle(sw.renderer, 50 + j*FNTHEIGHT, 350 + i*FNTHEIGHT, RADIUS);
         }
      }
   }
}

void printSDLcomment(SDL_Simplewin sw, fntrow fontdata[][FNTHEIGHT])
{
   Neill_SDL_DrawString(&sw, fontdata, "Huffman Tree in SDL!", 50, 10 + 2* FNTHEIGHT);
}

void drawSDLCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
   Neill_SDL_RenderDrawCircle(rend, cx, cy, r);
   Neill_SDL_RenderFillCircle(rend, cx, cy, r);

}

void drawLines(SDL_Renderer *rend, int cx, int cy, int dx, int dy )
{
   SDL_RenderDrawLine(rend, cx, cy, dx, dy);
}

void makeGreen(SDL_Simplewin sw)
{
   Neill_SDL_SetDrawColour(&sw, 0, 250, 0);  
}

void makeRed(SDL_Simplewin sw)
{
   Neill_SDL_SetDrawColour(&sw, 250, 0, 0);  
}

void makeBlack(SDL_Simplewin sw)
{
   Neill_SDL_SetDrawColour(&sw, 0, 0, 0);  
}

void drawChar(SDL_Simplewin *sw, fntrow fontdata[][FNTHEIGHT], unsigned char chr, int ox, int oy)
{
   Neill_SDL_DrawChar(sw, fontdata, chr, ox, oy);
}

void drawTreeInArray(node *parentNode, char array[][COL], int y, int x)
{
   node *temp;
   int i, total = 0;
   temp = parentNode;

   if (!temp){
      return;
   }

   array[y][x] = temp -> charValue;  
   
   if(temp -> right == NULL || temp -> left == NULL){
      array[y][x] = temp -> charValue;  
   }

   if(temp -> left != NULL){
      array[y+1][x] = LEFT;    
      drawTreeInArray(temp -> left, array, y+2, x);    
   }

   if(temp -> right != NULL){

      total = countRightNodes(temp->left);

      for(i = 0; i <= total; i++){
         x = x+1;
         array[y][x] = RIGHT;           
      }
      drawTreeInArray(temp -> right, array, y, x+1);      
   }

}

/* walks through binary tree and counts all right nodes*/
int countRightNodes(node *parentNode){

   node *temp;
   int count = 0;

   temp = parentNode;

   if(temp->right != NULL){
      count += 1 + countRightNodes(temp->right);
   }
   if(temp->left != NULL){
      count += countRightNodes(temp->left);
   }

   return count;
}

void printArray(char array[][COL])
{
   int i, j;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){
         printf("%c", array[i][j]);
      }
   printf("\n");
   }
}

void createArray(char array[][COL])
{
   int i, j;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){
         array[i][j] = SPACE;
      }
   }
}


/*finds huffman code and path to a node */
int printHuffCode(node *parentNode, int value, int counter)
{
   node *temp;
   
   temp = parentNode;
   
   if (!temp){
      return 0;
   }

   if(printHuffCode(temp -> left, value, counter-1)){
      printf("0"); 
      return 1;
   }
   else if(printHuffCode(temp -> right, value, counter-1)){  
      printf("1");
      return 1;
   }

   if(temp -> charValue == value){
      printSpace(counter);
      return 1;
   }

   return 0;
}

void printSpace(int spaces)
{
   int i;

   for(i = 0; i < spaces; i++){
      printf("%c", SPACE);
   }
}
/* extracts data from a file and returns a point to a linked list*/
node *createLinkList(char *file, node *headNode)
{
   FILE *filePointer;
   char letter;
   int i;
   node *temp, *current = NULL;
   int array[ASCIIMAX] = {0};

   filePointer = fopen(file, "r");

   if(filePointer == NULL){
      perror("Failed to open.\n");
      exit(EXIT_FAILURE);
   }

   while((letter = fgetc(filePointer)) != EOF){

      letter = makeUpperCase(letter);      

      for (i = 0; i<ASCIIMAX; i++){
         if (i == (int)letter && isalpha(letter)){
            array[i] = array[i] + 1;
         }
      }
   }

   for(i = 0; i <ASCIIMAX; i++){

      if(array[i] != 0){

         if (!headNode){
            headNode = current = initializeNode(i, array[i]);
         }
         else{
            temp = initializeNode(i, array[i]);
            current -> next = temp;
            current = current -> next;
         }
      }
   }

   fclose(filePointer);

   return headNode;
}

void checkInputCommands(int argc)
{
   int totalCommands = 2;

   if (argc != totalCommands){
      printf("Please input correct number of commands!\n");
      exit(1);
   }
}

node *initializeNode(int value, int frequency)
{
   node *temp;

   temp = (node *)malloc(sizeof(node));

   if(!temp){
      fprintf(stderr, "Can not allocate node\n");
      exit(1);
   }

   temp -> charValue = value;
   temp -> frequency = frequency;
   temp -> left = NULL;
   temp -> right = NULL;
   temp -> next = NULL;

   return temp;
}

void printList(node *temp)
{
   while(temp){
      printf("%c %d ", temp -> charValue, temp -> charValue);
      printf("frequency %d ", temp -> frequency);
      printf("left %p ", (void *)temp -> left);
      printf("right %p ", (void *)temp -> right);
      printf("next %p\n", (void *)temp -> next);
      temp = temp -> next;
   }
   printf("\n");
}

node *mergeSort(node *headNode)
{
   node *headOne;
   node *headTwo;

   if((headNode == NULL) || (headNode -> next == NULL))
      return headNode;

   headOne = headNode;
   headTwo = headNode -> next;

   while((headTwo != NULL) && (headTwo -> next != NULL)){
      headNode = headNode -> next;
      headTwo = headNode -> next -> next;
   }

   headTwo = headNode -> next;
   headNode -> next = NULL;

   return merge(mergeSort(headOne), mergeSort(headTwo));
}


node *merge(node *headOne, node *headTwo)
{
   node *headThree;

   if(headOne == NULL){
      return headTwo;
   }

   if(headTwo == NULL){
      return headOne;
   }

   if(headOne -> frequency < headTwo -> frequency){
      headThree = headOne;
      headThree -> next = merge(headOne -> next, headTwo);
   }
   else{
      headThree = headTwo;
      headThree -> next = merge(headOne, headTwo -> next);
   }

   return headThree;
}

node *createParentNode(node *firstNode, node *secondNode)
{
   node *parentNode;

   int total = 0;

   parentNode = (node *)malloc(sizeof(node));

   if(!parentNode){
      fprintf(stderr, "Can not allocate node\n");
      exit(1);
   }

   total = ((firstNode -> frequency) + (secondNode -> frequency));

   parentNode -> charValue = PARENTSYMBOL;
   parentNode -> frequency = total;
   parentNode -> left = firstNode;
   parentNode -> right = secondNode;
   parentNode -> next = NULL;

   return parentNode;
}

/* removes links from the first and second node in list and insert new parent node*/
node *newLinkedList(node *linkedListHead, node *parentNode)
{
   node *temp;

   temp = linkedListHead;

/* parent pointer to the third element of linked list */
   parentNode -> next = temp -> next -> next;

/* disconnects second element from linked list*/
   temp -> next -> next = NULL;

/* disconnects first element from linked list*/
   temp -> next = NULL;

   return parentNode;

}

node *createBinaryTree(node *headNode)
{
   node *temp, *temp2, *parentNode, *firstNode, *secondNode;

   temp = headNode;
   parentNode = NULL;

   while((temp -> next) != NULL){

      /* sorts linked list*/
      temp = mergeSort(temp);

      firstNode = temp;
      secondNode = temp -> next;

      /*ensures final most frequent char is assigned to the right of tree*/
      if((findListLength(temp) == 2) && ((temp -> charValue) != PARENTSYMBOL)){
         temp2 = firstNode;
         firstNode = secondNode;
         secondNode = temp2;
      }

      /* creates a parent node and links two smallest nodes to left and right of parent*/
      parentNode = createParentNode(firstNode, secondNode);

      /*links new parent node to linked list*/
      temp = newLinkedList(temp, parentNode);

   }

   return parentNode;
}

int findListLength(node *temp)
{
   int length = 0;

   while(temp){
      temp = temp -> next;
      length++;
   }

   return length;
}

int maxDepth(node *parentNode)
{
   int leftDepth = 0, rightDepth = 0;
   node *temp;

   temp = parentNode;

   if(temp == NULL){
      return 0;
   }
   else{
      leftDepth = maxDepth(temp -> left);
      rightDepth = maxDepth(temp -> right);

      if (leftDepth > rightDepth){
         return (leftDepth + 1);
      }
      else{
         return (rightDepth + 1);
      }
   }
}

int getCharLevel(node *parentNode, int value)
{
   return getCharLevelRecur(parentNode, value, 1);
}

int getCharLevelRecur(node *parentNode, int value, int level)
{
   node *temp;
   int nextLevel;

   temp = parentNode;

   if(temp == NULL){
      return 0;
   }

   if(temp -> charValue == value){
      return level;
   }

   nextLevel = getCharLevelRecur(temp -> left, value, level + 1);

   if(nextLevel != 0){
      return nextLevel;
   }

   nextLevel = getCharLevelRecur(temp -> right, value, level + 1);

   return nextLevel;
}

void printAll(node *parentNode, node *secondList)
{
   int depth, huffmanLength = 0;
   node *temp;

   temp = secondList;  

   depth = maxDepth(parentNode);

   while(temp){
      printf("'%c' :", temp -> charValue);
      printHuffCode(parentNode, temp -> charValue, depth);
      huffmanLength = (getCharLevel(parentNode, temp->charValue)-1);
      printf(" (%4d *%5d)", huffmanLength, temp -> frequency);
      printf("\n");
      temp = temp -> next;
   }

   printTotalByte(parentNode, secondList);
}

void printTotalByte(node *parentNode, node *linkedList)
{
   int total = 0;
   int codeLength = 0, frequency = 0;
   node *temp;

   temp = linkedList;

   while(temp){
      codeLength = getCharLevel(parentNode, temp->charValue)-1;
      frequency = temp -> frequency;
      total += (codeLength * frequency);
      temp = temp -> next;
   }

   printf("%d bytes\n", (total/BYTES));

}

/* checks if letter is part of alphabet then makes uppercase*/
int makeUpperCase(int letter)
{
   int newLetter;

   newLetter = letter;

   if(isalpha(letter)){
     newLetter = toupper(letter);
   }

   return newLetter;
}

void freeBinaryTree(node *parentNode)
{
   if(parentNode != NULL){
      freeBinaryTree(parentNode -> left);
      freeBinaryTree(parentNode -> right);
      free(parentNode);
   }
}






























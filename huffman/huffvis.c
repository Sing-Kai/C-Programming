#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ASCIIMAX 128
#define PARENTSYMBOL '#'
#define BYTES 8
#define SPACE ' '
#define LEFT '|'
#define RIGHT '-'
#define COL 40
#define ROW 30

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
node *merge(node *nodeOne, node *nodeTwo);
node *createParentNode(node *firstNode, node *secondNode);
node *newLinkedList(node *headNode, node *parentNode);
node *createBinaryTree(node *headNode);
int findListLength(node *temp);
int maxDepth(node *parentNode);
int getCharLevel(node *parentNode, int value);
int getCharLevelRecur(node *parentNode, int value, int level);
void printAll(node *parentNode, node *secondList);
void printTotalByte(node *parentNode, node *secondList);
void printCheck(int value);
int printHuffCode(node *parentNode, int value, int counter);
void printSpace(int spaces);
int totalFrequency(node *first, node *second);
int makeUpperCase(int letter);
void createArray(char array[][COL]);
void printArray(char array[][COL]);
void drawTreeInArray(node *parent, char array[][COL], int x, int y, int total);
int countRightNodes(node *parent);
void freeList(node *list);
void freeBinaryTree(node *parentNode);

int main(int argc, char *argv[])
{
   node *firstList = NULL;
   node *parentNode = NULL;
   char array[ROW][COL];

   checkInputCommands(argc);

   firstList = createLinkList(argv[1], firstList);
   parentNode = createBinaryTree(firstList);

   createArray(array);
   drawTreeInArray(parentNode, array, 0, 0, 0);
   printArray(array);

   freeBinaryTree(parentNode);

   return 0;
}
/* traverse tree recursively and plot characters in 2D array*/
void drawTreeInArray(node *parentNode, char array[][COL], int y, int x, int totalx)
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
      drawTreeInArray(temp -> left, array, y+2, x, totalx);    
   }

   if(temp -> right != NULL){
/* finds the total number of right nodes in sub left tree*/
      total = countRightNodes(temp->left);

      for(i = 0; i <= total; i++){
         x = x+1;
         array[y][x] = RIGHT;           
      }

      totalx = total;

      drawTreeInArray(temp -> right, array, y, x+1, totalx);      
   }

}

int countRightNodes(node *parentNode){

   node *temp;
   int count = 0;

   temp = parentNode;

/* if right node present increment counter */
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

/* creates an array of frequencys of ASCII characters*/
   while((letter = fgetc(filePointer)) != EOF){

      letter = makeUpperCase(letter);      

      for (i = 0; i<ASCIIMAX; i++){
         if (i == (int)letter && isalpha(letter)){
            array[i] = array[i] + 1;
         }
      }
   }
/* creates linked list from array where value is large than zero*/
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
   node *nodeOne;
   node *nodeTwo;

   if((headNode == NULL) || (headNode -> next == NULL))
      return headNode;

   nodeOne = headNode;
   nodeTwo = headNode -> next;

   while((nodeTwo != NULL) && (nodeTwo -> next != NULL)){
      headNode = headNode -> next;
      nodeTwo = headNode -> next -> next;
   }

   nodeTwo = headNode -> next;
   headNode -> next = NULL;

   return merge(mergeSort(nodeOne), mergeSort(nodeTwo));
}


node *merge(node *nodeOne, node *nodeTwo)
{
   node *nodeThree;

   if(nodeOne == NULL){
      return nodeTwo;
   }

   if(nodeTwo == NULL){
      return nodeOne;
   }

   if(nodeOne -> frequency < nodeTwo -> frequency){
      nodeThree = nodeOne;
      nodeThree -> next = merge(nodeOne -> next, nodeTwo);
   }
   else{
      nodeThree = nodeTwo;
      nodeThree -> next = merge(nodeOne, nodeTwo -> next);
   }

   return nodeThree;
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

   total = totalFrequency(firstNode, secondNode);

   parentNode -> charValue = PARENTSYMBOL;
   parentNode -> frequency = total;
   parentNode -> left = firstNode;
   parentNode -> right = secondNode;
   parentNode -> next = NULL;

   return parentNode;
}

int totalFrequency(node *firstNode, node *secondNode)
{
   int total = 0;

   total = ((firstNode -> frequency) + (secondNode -> frequency));

   return total;
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
      printCheck(temp -> charValue);
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

/* check character is printable*/
void printCheck(int value)
{
   if(isprint(value)){
      printf("'%c' :", value);      
   }
   else{
      printf("%03d :", value);
   }
}

void freeList(node *headNode)
{
   node *temp;

   while(headNode != NULL){
      temp = headNode;
      headNode = headNode -> next;
      free(temp);
   }
}

void freeBinaryTree(node *parentNode)
{
   if(parentNode != NULL){
      freeBinaryTree(parentNode -> left);
      freeBinaryTree(parentNode -> right);
      free(parentNode);
   }
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































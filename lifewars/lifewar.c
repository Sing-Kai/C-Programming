#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ROW 150
#define COL 90
#define STRINGLEN 11
#define MAXLINES 99
#define TOTALGAMES 50
#define GENERATIONS 5000
#define PLAYER1 '+'
#define PLAYER2 '@'
#define EMPTYSPACE ' '



int readFile(char board[][COL], char *filename, char symbol);
void initializeBoard(char board[][COL],int x, int y, char symbol);
int lifeRules(char oldBoard[][COL], char newBoard[][COL]);
void switchBoard(char oldBoard[][COL], char newBoard[][COL]);
int neighbourCellCount(char board[][COL], int i, int j, char symbol);
void runGameOfLife(char board1[][COL], char board2[][COL], int *player1total, int *player2total);
void collisonCheck(char board[][COL],char *file1, char *file2);
int boardCompare(char board1[][COL], char board2[][COL]);
void printBoard(char board[][COL]);
void createBoard(char board[][COL]);
int randomXY(int num);
int newCoordinate(int randNum, int fileNum, int dimension);
int countFinalCells(char board[][COL], char symbol);
int whoWins(int playerA, int playerB);
void printFinalStatement(int playerA, int playerB);


int main(int argc, char *argv[])
{
   char board1[ROW][COL], board2[ROW][COL];
   int i, player1total, player2total;

   createBoard(board1);
   createBoard(board2);

   srand(time(NULL));
   collisonCheck(board1, argv[1], argv[2]);

   player1total = 0;
   player2total = 0;

   for(i=1; i<=TOTALGAMES; i++){
      runGameOfLife(board1, board2, &player1total, &player2total);
      printf("%d\t%d\t%d\t",i, player1total, player2total);
      printf("%d\n", whoWins(player1total, player2total));
   }

   printFinalStatement(player1total, player2total);

   return 0;
}

void runGameOfLife(char board1[][COL], char board2[][COL], int *player1total, int *player2total)
{
   int i;

   for (i=0; i< GENERATIONS; i++){
      lifeRules(board1, board2);
      switchBoard(board1, board2);
      *player1total += countFinalCells(board1, PLAYER1);
      *player2total += countFinalCells(board1, PLAYER2);
   }
}

/* if collisons on boards occur continues loop and resets boards*/
void collisonCheck(char mainBoard[][COL], char *file1, char *file2)
{
   char tempBoard[ROW][COL];
   int restart = 1;

   do {
      createBoard(mainBoard);
      createBoard(tempBoard);
      readFile(mainBoard, file1, PLAYER1);
      readFile(tempBoard, file2, PLAYER2);
      restart = boardCompare(mainBoard, tempBoard);
   }  while (restart);
}

/* reads file and updates board with player coordinates*/
int readFile(char board[][COL],char *filename, char symbol)
{
   FILE *filePointer;
   int i, x, y, randX, randY, finalX, finalY;
   char headerString[STRINGLEN];

   filePointer = fopen(filename, "r");

   if (filePointer == NULL){
      perror("Can not open file");
      return 1;
   }
   /* reads header string*/
   fgets(headerString, STRINGLEN, filePointer);

   i = 0;
   randX = randomXY(COL);
   randY = randomXY(ROW);

   while ((i < MAXLINES) && (fscanf(filePointer, "%d %d", &x, &y)!=EOF)){
      finalX = newCoordinate(x, randX, COL);
      finalY = newCoordinate(y, randY, ROW);
      initializeBoard(board, finalY, finalX, symbol);
      i++;     
   }

   fclose(filePointer);

   return 0;
}

void initializeBoard(char board[][COL], int y, int x, char symbol)
{
   int i, j;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){
         if (i == y && j == x){
             board[i][j] = symbol;
         }
      }
   }
}

/* Apply game of life rules to board*/
int lifeRules(char oldBoard[][COL], char newBoard[][COL])
{
   int i, j, neighbourCount, player1Neighbour, player2Neighbour;

   neighbourCount = 0;
   player1Neighbour = 0;
   player2Neighbour = 0;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){

         player1Neighbour = neighbourCellCount(oldBoard, i, j, PLAYER1);
         player2Neighbour = neighbourCellCount(oldBoard, i, j, PLAYER2);
         neighbourCount = player1Neighbour + player2Neighbour;
         /* cells stay the same*/
         if (neighbourCount == 2){newBoard[i][j] = oldBoard[i][j];}
         /* cell death*/
         if (neighbourCount < 2){newBoard[i][j] = EMPTYSPACE;}
         /* cell death*/          
         if (neighbourCount > 3){newBoard[i][j] = EMPTYSPACE;}  
         /* cell birth if surrounded by same 3 cells*/ 
         if (neighbourCount == 3){

             if (neighbourCellCount(oldBoard, i, j, PLAYER1) == 3){
                newBoard[i][j] = PLAYER1;
             }
             else if (neighbourCellCount(oldBoard, i, j, PLAYER2) == 3){
                newBoard[i][j] = PLAYER2;
             }
             else{
                newBoard[i][j] = oldBoard[i][j];
             }
         }               
      }
   }
   return 0;
}

/* Returns count of live neighbour cells */
int neighbourCellCount(char board[][COL], int i, int j, char symbol)
{
   int count, sameRow, sameCol;
   int rowAbove, rowBelow, leftCol, rightCol;

   count = 0;
   sameRow = i, sameCol = j;

   /* calculation to wrap around board */
   rowAbove = ((i + ROW - 1) % ROW);
   rowBelow = ((i + 1) % ROW);
   leftCol = ((j + COL - 1) % COL);
   rightCol = ((j + 1) % COL);

   /*Count alive cells above cell positioned i,j*/
   if (board[rowAbove][leftCol] == symbol) {count++;}
   if (board[rowAbove][sameCol] == symbol) {count++;}
   if (board[rowAbove][rightCol] == symbol) {count++;}
   /*Count live cells left and right of cell positioned i,j*/
   if (board[sameRow][leftCol] == symbol) {count++;}
   if (board[sameRow][rightCol] == symbol) {count++;}
   /*Count alive cells below cell positioned i,j*/
   if (board[rowBelow][leftCol] == symbol) {count++;}
   if (board[rowBelow][sameCol] == symbol) {count++;}
   if (board[rowBelow][rightCol] == symbol) {count++;}

   return count;
}

void switchBoard(char oldBoard[][COL], char newBoard[][COL])
{
   int i, j;

   for (i=0; i<ROW; i++){
      for(j=0; j<COL; j++){
         oldBoard[i][j] = newBoard[i][j];
      }
   }
}

int randomXY(int num)
{
   int randomCoordinate = 0;

   randomCoordinate = (rand() % num);

   return randomCoordinate;
}

/* Generates new coordinate relative from random and file coordinate*/
int newCoordinate(int randNum, int fileNum, int dimension)
{
   int newCoordinate = 0;

   newCoordinate = (randNum + fileNum);

   /* wraps round board if coordinates are out of bounds*/
   if (newCoordinate < 0){
      newCoordinate = ((newCoordinate + (dimension * 2)) % dimension);
   }
   if (newCoordinate >= dimension){
      newCoordinate = (newCoordinate % dimension);
   }
   return newCoordinate;
}

/*Compares player1 board with player2 board, combines boards if no collison */
int boardCompare(char board1[][COL], char board2[][COL])
{
   int i, j;

   for(i=0; i<ROW; i++){
      for(j=0; j<COL; j++){
         if ((board1[i][j] == PLAYER1) && (board2[i][j] == PLAYER2)){
            return 1;
         }
         if (board1[i][j] == EMPTYSPACE){
            board1[i][j] = board2[i][j];
         }
      }
   }
   return 0;
}

int countFinalCells(char board[][COL], char symbol)
{
   int i,j;
   int counter =0;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){
         if (board[i][j] == symbol){
            counter++;
         }
      }
   }
   return counter;
}

void printBoard(char board[][COL])
{
   int i, j;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){
         printf("%c", board[i][j]);
      }
   printf("\n");
   }
}

void createBoard(char board[][COL])
{
   int i, j;

   for (i=0; i<ROW; i++){
      for (j=0; j<COL; j++){
         board[i][j] = EMPTYSPACE;
      }
   }
}

int whoWins(int playerA, int playerB)
{
   if (playerA > playerB){
      return 1;
   }
   else if (playerA < playerB){
      return 2;
   }
   return 0;
}

void printFinalStatement(int playerA, int playerB)
{

    if (whoWins(playerA, playerB) == 1){
       printf("Player 1 wins by %d cells to %d\n", playerA, playerB);
    }
    else if(whoWins(playerA, playerB) == 2){
       printf("Player 2 wins by %d cells to %d\n", playerB, playerA); 
    }
    else{
       printf("This game is a draw\n");
    }
}






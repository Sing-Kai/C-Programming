#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
   char *word;
   int flagMarker;
   struct node *parentWord;
   struct node *next;
};

enum flag {unmarked, marked};

typedef struct node node;
typedef enum flag flag;

void extractData(char *file, int *totalWords, int *maxWordLength);
int maxLength(int a, int b);
int checkWordLength(char *wordA, char *wordB);
node *initializeNode(char *word);
void printList(node *list);
int findEditDistance(char *wordA, char *wordB, int wordLength);
void reverseParentPointer(node* headList, char *targetWord);
void printWordLadder(node* headList, char *sourceWord, char *targetWord);
void isWordPresent(char *word, node *headList);
node *searchNode(node *headList, char *word);
void updateParent(node *headList, char *word, int wordLength);
void searchAllParents(node *headList, char *sourceWord, char *targetWord, int wordLength, int totalWords);
void sourceWordParents(node *headList, char *sourceWord, int wordLength);
void markedWordParents(node *headList, int wordLength);
void checkInputCommands(int argc);
char *InputWord(char *sourceWord, int maxWordLength);
int findWordLength(char *wordA);
void checkAllErrors(node *headList, char *sourceWord, char *targetWord);
void checkPathPresent(node *headList, char *sourceWord, char *targetWord);
node *createLinkedList(char *file, node *headList, node *current, int wordLength, int maxWordLen);
int countNode(node *headList);
void freeList(node *headList);

int main(int argc, char *argv[])
{
   char *sourceWord = NULL, *targetWord = NULL;
   node *headList = NULL, *current = NULL;
   int totalWords = 0, maxWordLength = 0, wordLength = 0, nodeCount = 0;

   checkInputCommands(argc);

   extractData(argv[1], &totalWords, &maxWordLength);

   printf("Total words is %d\n", totalWords);
   printf("Source word: ");
   sourceWord = InputWord(sourceWord, maxWordLength);
   printf("Target word: ");
   targetWord = InputWord(targetWord, maxWordLength);

   wordLength = findWordLength(sourceWord);

   headList = createLinkedList(argv[1], headList, current, wordLength, maxWordLength);

   nodeCount = countNode(headList);

   checkAllErrors(headList, sourceWord, targetWord);

   searchAllParents(headList, sourceWord, targetWord, wordLength, nodeCount);

   reverseParentPointer(headList, targetWord);

   checkPathPresent(headList, sourceWord, targetWord);

   printWordLadder(headList, sourceWord, targetWord);

   freeList(headList);

   return 0;
}

void checkInputCommands(int argc)
{
   int totalCommands = 2;

   if (argc != totalCommands){
      printf("Please input correct number of commands!\n");
      exit(1);
   }
}

char *InputWord(char *word, int maxWordLength)
{
   word = (char*)malloc((maxWordLength + 1) * sizeof(char));

   scanf("%s", word);

   return word;
}

int maxLength(int a, int b)
{
   int maxNumber = 0;

   if (a > b){
      maxNumber = a;
   }
   else{
      maxNumber = b;
   }

   return maxNumber;
}

int findWordLength(char *wordA)
{  
   return strlen(wordA);
}

int checkWordLength(char *wordA, char *wordB)
{
   if (strlen(wordA) != strlen(wordB)){
      return 0;
   }

   return 1;
}

/* extracts information from file then and allocates node to linked list*/
void extractData(char *file, int *totalWords, int *maxWordLength)
{
   FILE *filePointer;
   char letter;
   int lines = 0, letterCount = 0, maxWordLen = 0;

   filePointer = fopen(file, "r");

   if(filePointer == NULL){
      perror("Failed to open.\n");
      exit(EXIT_FAILURE);
   }

   /* finds total lines and max word length */ 
   while((letter = fgetc(filePointer)) != EOF){

      if(letter == '\n'){
         lines++;
         letterCount = 0;    
      }
      else{
         letterCount++;
      }
      maxWordLen = maxLength(letterCount, maxWordLen);        
   }

   *totalWords = lines;
   *maxWordLength = maxWordLen;
   
   fclose(filePointer);
}

/* creates list ensuring words are same as user input word length */
node *createLinkedList(char *file, node *headList, node *current, int wordLength, int maxWordLen)
{
   FILE *filePointer;
   char *word;
   node *temp;
   
   filePointer = fopen(file, "r");

   if(filePointer == NULL){
      perror("Failed to open.\n");
      exit(EXIT_FAILURE);
   }
 
   word = malloc(maxWordLen * sizeof(char *));

   while(fscanf(filePointer, "%s", word) != EOF) {

      if (strlen(word) == (unsigned int)wordLength){

         if (!headList){
            headList = current = initializeNode(word);
         }
         else{
            temp = initializeNode(word);
            current -> next = temp;
            current = current -> next;
         }
      }
   }

   fclose(filePointer);

   return headList;
} 

/* initialize node structure with data */
node *initializeNode(char *word)
{
   node *temp;
   int len = strlen(word);
   char *newWord;

   newWord = (char*)malloc(len * sizeof(char*)); 
   temp = (node *)malloc(sizeof(node));

   if(!temp){
      fprintf(stderr, "Can not allocate node\n");
      exit(1);
   }

   strcpy(newWord, word); 

   temp -> word = newWord;
   temp -> flagMarker = unmarked;
   temp -> parentWord = NULL;
   temp -> next = NULL;

   return temp;
}

void printList(node *temp)
{
   while(temp){
      printf("%s %p\t", temp -> word, (void *)temp);
      printf("parent pointer %p\t", (void *)temp -> parentWord);
      printf("read marker %d\n", temp -> flagMarker);
      temp = temp -> next;
   }
   printf("\n");
}

int findEditDistance(char *wordA, char *wordB, int wordLength)
{
   int i, distance = 0;

   for(i = 0; i < wordLength; i++){
      if(wordA[i] != wordB[i]){
         distance++;
      }
   }

   return distance;
}


void checkAllErrors(node *headList, char *sourceWord, char *targetWord)
{
   isWordPresent(sourceWord, headList);
   isWordPresent(targetWord, headList);
}

/* check if word is present in linked list */
void isWordPresent(char *word, node *headList)
{
   node *temp;
   int wordIsPresent;

   temp = headList;
   wordIsPresent = 0;

   while (temp){
      if (strcmp(temp -> word, word) == 0){
         wordIsPresent = 1;
      }
      temp = temp -> next;
   }

   if (wordIsPresent == 0){
      printf("%s is not in list or invalid\n", word);
      exit(1);
   }
}

/* returns node pointer of a word */
node *searchNode(node *headList, char *word)
{
   node *temp;
   flag updateFlag = marked;

   temp = headList;

   while(temp){
      if(strcmp(temp -> word, word) == 0){
         temp -> flagMarker = updateFlag;
         return temp;
      }
      temp = temp -> next;
   }

   return NULL;
}

/* allocate words that is edit distance of 1 from source word then loop though all whats that have been read*/
void searchAllParents(node *headList, char *sourceWord, char *targetWord, int wordLength, int nodeCount)
{
   int i;
   node *targetWordNode;

   targetWordNode = searchNode(headList, targetWord);
   /*resets flag to zero as searchNode function sets it to 1*/
   targetWordNode -> flagMarker = 0;

   sourceWordParents(headList, sourceWord, wordLength); 

   for (i = 0; i < nodeCount; i++){

      markedWordParents(headList, wordLength);

      /* once target word has a parent terminate loop*/
      if (targetWordNode -> parentWord != NULL){
         i = nodeCount;      
      }  
   }
}

/* initializing first set of words that is edit distnace 1 from source word*/
void sourceWordParents(node *headList, char *sourceWord, int wordLength)
{     
   node *temp, *sourceWordNode;
   flag updateFlag;

   temp = headList;
   updateFlag = marked;
   sourceWordNode = searchNode(headList, sourceWord);   

   while(temp){
      if (findEditDistance(temp -> word, sourceWord, wordLength) == 1){
         temp -> parentWord = sourceWordNode;
         temp -> flagMarker = updateFlag;
      }
      temp = temp -> next;
   }
}

/* if words have been marked then finds parent word*/
void markedWordParents(node *headList, int wordLength)
{
   node *current;

   current = headList;

   while(current){         
      if ((current -> flagMarker) == marked && (current -> parentWord) != NULL){          
         updateParent(headList, current -> word, wordLength);
      }       
      current = current -> next;
   } 
}

/* updates node to point to parent if edit distance is 1 and not marked*/
void updateParent(node *headList, char *parentWord, int wordLength)
{
   node *temp, *parentWordNode;
   char *tempWord;
   flag updateFlag;
   
   temp =  headList;
   updateFlag = marked;
   parentWordNode = searchNode(headList, parentWord);

   while(temp){
   
      tempWord = temp -> word;

      if(findEditDistance(tempWord, parentWord, wordLength) == 1 && (temp -> flagMarker) != marked){
         temp -> parentWord = parentWordNode;
         temp -> flagMarker = updateFlag;
      }

      temp = temp -> next;   
   }
}

/* if no path present then prints message */
void checkPathPresent(node *headList, char *sourceWord, char *targetWord)
{
   node *sourceWordNode, *targetWordNode;

   sourceWordNode = searchNode(headList, sourceWord);
   targetWordNode = searchNode(headList, targetWord);

   if ((sourceWordNode -> parentWord) == NULL && (targetWordNode -> parentWord) == NULL){
      printf("Could not find path from %s to %s\n", sourceWordNode -> word, targetWordNode -> word);
      exit(1);
   }

}

/* reverses the link of target parent word that leads to source word*/
void reverseParentPointer(node* headList, char *targetWord)
{
   node *previous = NULL;
   node *next;
   node *current;

   current = searchNode(headList, targetWord);

   while(current){
      next = current -> parentWord;
      current -> parentWord = previous;
      previous = current;
      current = next;
   }
}

void printWordLadder(node* headList, char *sourceWord, char *targetWord)
{
   node *sourceNode;

   sourceNode = searchNode(headList, sourceWord);

   while (sourceNode -> parentWord != NULL){      
      printf("%s -> ", sourceNode -> word);
      sourceNode = sourceNode -> parentWord;
   }

   printf("%s\n", targetWord);
}

int countNode(node *headList)
{
   node *temp;
   int count;

   temp = headList;
   count = 0;

   while(temp){
      temp = temp -> next;
      count++;
   }

  return count;
}

void freeList(node *headList)
{
   node *temp;

   while(headList != NULL){
      temp = headList;
      headList = headList -> next;
      free(temp);
  }
}


























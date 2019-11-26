/*
 * Calculates UP TO the top 10 tweeters (by volume of tweets)
 * in a given CSV file of tweets.
 * This program takes in one command line argument: the path of the CSV file
 * Output: 10 lines of <tweeter>: <count of tweets>
 * Each output line is in decreasing order of tweets
 *
 * Additionally, this program handles both valid and invalid inputs
 * For invalid inputs, this program outputs "Invalid Input Format"
 * Length of valid csv file: maximum 1024 characters
 * Max length of file: 20000 lines
 * In a valid file, tweets contain no commas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dict.h"

#define MAXCHARS (1024)
#define MAXLENFILE (20000)

/*
 * Pair of {Person: Count of tweets}
 */
typedef struct PersonCountPair
{
    char* person;
    int count;
} PersonCountPair;

struct node {
   char* data;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

/*
Simple linked list to store names. Used to find values later on
*/


void insert(char* data1) {
   struct node *ptr = head;
   //create a link
   bool flag = false;
   while(ptr != NULL) {   
       
      if (strcmp(ptr->data, data1 ) == 0)  {
         
          return;
          }
      ptr = ptr->next;
   }

   struct node *link = (struct node*) malloc(sizeof(struct node));

   //link->key = key;
   link->data = data1;

   //point it to old first node
   link->next = head;

   //point first to new first node
   head = link;
}

//display the list
void printList() {

   struct node *ptr = head;

   printf("\n[head] =>");
   //start from the beginning
   while(ptr != NULL) {        
      printf(" %s =>",ptr->data);
      ptr = ptr->next;
   }

   printf(" [null]\n");
}

/*
//display the key value pairs
void printDict(){
    struct node *ptr = head;
    while(ptr != NULL) {        
      printf("For %s we have count: %d \n", ptr->data,DictSearch(d, ptr->data) );
      ptr = ptr->next;
   }

}
*/

/*
 * FileCheck: Checks if the file has a valid input format i.e. *.csv
 * Args:
 *  fileName: Name of the file 
 * Returns:
 *  a boolean indication wheter a file is valid or not.
 */
bool fileCheck(char* fileName)
{
    if (fileName == NULL)
    {
        printf("Invalid Input Format\n");
        return false;
    }
        // Code modified from:
        // https://stackoverflow.com/questions/10347689/how-can-i-check-whether-a-string-ends-with-csv-in-c
        char *dot = strrchr(fileName, '.');
        if (dot && !strcmp(dot, ".csv"))
        {
            return true; // File ends with .csv
        }
        else
        {
                printf("Invalid Input Format\n"); 
            return false;
    }
    return false;
} 


/*
 * getPosNameColumn: Returns the position (index) of the name column in the CSV file
 * Args:
 *  fp: file 
 * Returns:
 *  index of the column that contains the names             
 */
int getPosNameColumn(FILE *fp)
{
    int posNameColumn = 0, indexInLine = 0;
    char line[MAXCHARS];
    char nameHeaderNoQuotes[] = "name";
    char nameHeaderWithQuotes[] = "\"name\"";
    char* lineCopy = NULL;
    char* token = NULL;

    fgets(line, MAXCHARS, fp);
    lineCopy = strdup(line);
    token = strsep(&lineCopy, ",");
    
    while ( (token = strsep(&lineCopy, ",")) != NULL)
    {
        if(strcmp(nameHeaderNoQuotes, token) == 0 || strcmp(nameHeaderWithQuotes, token) == 0)
        {
            posNameColumn = indexInLine;
            break;
        }
        indexInLine += 1;
    }
    //printf("pos name col: %d\n", posNameColumn);    
    return posNameColumn;

} 


/*
 * TODO
 * Fills an array of {Person : Tweet count} structs
 */
void fillPersonCountArray( char* fileName, int posNameColumn)
{
    int indexInFile = 0, indexInLine = 0;
    char line[MAXCHARS];
    char* lineCopy = NULL;
    char* token = NULL;
    char* name = NULL;
    
    FILE* fp = fopen(fileName, "r");
    fgets(line, MAXCHARS, fp); 
    Dict d = DictCreate();
    int scoobydoo_c = 0;
    while (fgets(line, MAXCHARS, fp))// Enter line
    {
        
        lineCopy = strdup(line);
        indexInLine = 0;

        while ( (token = strsep(&lineCopy, ",")) != NULL) // enter word within the token
        {
            
            if(indexInLine == (posNameColumn+1))        //find the name column
            {
                //Extract names 
                if(token[0] == '\"' && token[strlen(token)-1] == '\"')  
                {
                    
                    // Remove quotes
                    int nameLength= strlen(token) - 2; // -2 to remove quotes
                    int newNameLength = nameLength + 1; // +1 because of nullterm
                    
                    name = (char*) malloc(newNameLength * sizeof(char));
                    memcpy(name, &token[1], nameLength);
                    name[newNameLength] = '\0';
                } 
                else // No quotes in the name
                {
                    size_t size = strlen(token) + sizeof(char);
                    name = (char*) malloc(size);
                    strncpy(name, token, size);
                    
                }
                const char *cur_count = DictSearch(d, name);
                int temp;
                const char *new_count;
                temp = atoi(cur_count)+1;
                
                //get current_count 
                //const char *cur_count = DictSearch(d, name);
               
                

                sprintf(new_count, "%d",  temp);
                DictDelete( d, name);
                DictInsert(d, name, new_count);
                insert(name);
                
                break;
                //printf("Name is %s\n", name);

            }
            indexInLine += 1;
        }
        
        indexInFile += 1;
    }
   
   //Debugging
    //printList();
    struct node *ptr = head;
    while(ptr != NULL) {        
      printf("For %s we have count: %d \n", ptr->data,atoi(DictSearch(d, ptr->data) ));
      ptr = ptr->next;
   }
   
    free(name);
    

/*
    //Get length of unique counts in dataset
    struct node *ptr = head;
    int length = 0; 
    while(ptr != NULL) {        
      length += 1;
      ptr = ptr->next;
   }
   //If no elements present then just end the program
   if (length == 0){
       return;
   }
*/
} // fillPersonCountArray()


/*
 * Reads and parses the CSV file
 * Gathers the {Person: Tweet count}
 */
void readFile(char* fileName)
{
    int posNameColumn = 0;
    FILE* fp = fopen(fileName, "r");    

    // Get the position of the name column in the CSV file
    posNameColumn = getPosNameColumn(fp);
    
    
    
    // TODO
    fillPersonCountArray(fileName, posNameColumn);

} // readFile()




int main(int argc, char* argv[])
{

    bool fileIsValid = false;
    fileIsValid = true;

/*
    if (fileCheck(argv[1])) 
        fileIsValid = true;
    else 
        fileIsValid = false;
*/
    if (fileIsValid)
    {
        
        //readFile(argv[1]); 
        readFile("test.csv"); 
      
    }
     
    
  
    //dict_print(dictionary);
    
    return 0;
} // main()

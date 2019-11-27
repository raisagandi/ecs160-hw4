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

//Helper function 
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
    /* TODO: THIS IS THE PART CAUSING AN ERROR
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
    */

   
   FILE* fp = fopen(fileName, "r");
   int row_count = 0;
   char line[2056];
   //Check for max char and max lines
   while (fgets(line, MAXCHARS, fp)){
       row_count++;
       if (strlen(line) > MAXCHARS){
           printf("Too many characters in line");
           return false;
       }
   }
   if (row_count > MAXLENFILE){
       printf("Too many lines ");
       return false;
   }
   
   

   

    return false;
} 


 
void merge(int arr[], char* name_list[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    char* L1[n1];
    char* R1[n2];
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
        L1[i] = name_list[l + i];   
    }

    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1+ j];
        R1[j] = name_list[m + 1+ j];
    }

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            name_list[k] = L1[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            name_list[k] = R1[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        name_list[k] = L1[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        name_list[k] = R1[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[],char* list_name[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        mergeSort(arr, list_name, l, m);
        mergeSort(arr,list_name, m+1, r);
        merge(arr, list_name, l, m, r);
    }
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
 *          fillPersonCountArray: Finds count for each user -> sorts that according to count -> prints top 10 values
 *                          Args:
 *                              fileName: Name of the csv file 
 *                              posNameColumn: Column number that contains the name of the tweet
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
                // Get count in dictionary, if name isn't there then 0 is returned.
                const char *cur_count = DictSearch(d, name);
                int temp;
                char* new_count;
                temp = atoi(cur_count)+1; 
                sprintf(new_count, "%d",  temp);
                DictDelete( d, name);
                DictInsert(d, name, new_count);
                insert(name);
                break;
            }
            indexInLine += 1;
        }
        indexInFile += 1;
    }
    // Get number of names to initialize array
    int name_count = 0;
    struct node *ptr = head;
    while(ptr != NULL) {        
      ptr = ptr->next;
      name_count += 1;
   }
   char* all_names[name_count];
   int all_count[name_count];
   ptr = head;
   int index ;
   //Fill in array with names and count
   for( index = 0; index < name_count; index = index + 1 ){
       
       all_names[index] = ptr->data;
       all_count[index] = atoi(DictSearch(d, ptr->data) );
       ptr = ptr->next;
       //printf("For %s we have count: %d \n", all_names[index],all_count[index]);
   }
   
   // Run mergeSort to sort the values
   mergeSort(all_count, all_names, 0, (name_count - 1)); 
   DictDestroy(d);
   //If we have less  than 10 values then print out sorted list for whatever values are available
   if (name_count<10){
       for (int i=name_count-1; i >= 0; i--) {
       printf("For %s we have : %d\n", all_names[i], all_count[i]);

   }

   }
   //Print the top 10 values other wise
   else{
       for (int i=name_count-1; i >= name_count-10; i--) {
       printf("For %s we have : %d\n", all_names[i], all_count[i]);

   }
   } 
free(name);
 
} // fillPersonCountArray()


/*
 *      readFile: Reads the file and prints out the top 10 values
 *           Args:
 *                 - filename: Name of file
 */
void readFile(char* fileName)
{
    int posNameColumn = 0;
    FILE* fp = fopen(fileName, "r");    

    // Get the position of the name column in the CSV file
    posNameColumn = getPosNameColumn(fp);
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
    fileCheck(argv[1]);

    if (fileIsValid)
    { 
        readFile(argv[1]); 
    }
    return 0;
} // main()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

#define MAXCHARS (1024)
#define MAXLENFILE (20000)

/**
 * - - - - - - - - - - - 
 * | DOUBLY LINKED LIST |
 * | DATA STRUCTURE     |
 * | FOR TWEETER DATA   |
 * - - - - - - - - - - --
 */

/* Tweeter, a linked list node */
typedef struct Tweeter
{
    char* name;
    int tweetCount;
    struct Tweeter* prev;
    struct Tweeter* next;
} Tweeter;

/* The linked list */
typedef struct TweeterList
{
    int length;
    Tweeter* front;
    Tweeter* rear;
} TweeterList;


/*
 * Initialize a new tweeter
 */
Tweeter* createTweeter(char* tweeterName)
{
    Tweeter* newTweeter = (Tweeter*)malloc(sizeof(Tweeter));
    newTweeter->name = tweeterName;
    newTweeter->tweetCount = 1;
    newTweeter->prev = NULL;
    newTweeter->next = NULL;
} // createTweeter()

/*
 * Initialize an empty list
 */
TweeterList* createTweeterList()
{
    TweeterList* newList = (TweeterList*)malloc(sizeof(TweeterList));
    newList->length = 0;
    newList->front = NULL;
    newList->rear = NULL;
} // createTweeterList()

/*
 * Destructor for tweeter list
 */
/*
int destroyTweeterList(TweeterList* tweeterList)
{
    if(!tweeterList || tweeterList->length == 0)
    {
        exit(1);
    }
    
    // Deallocate the tweeters
    Tweeter* head = tweeterList->front;
    do {
        Tweeter* next = head->next;
	free(head->name);
	free(head);
	head = NULL;
    } while(next != NULL);

    // Deallocate tweeter list
    free(tweeterList);
    tweeterList = NULL;
    return 0;
} // destroyTweeterList()
*/


void swapTweeters(TweeterList* tweeterList, Tweeter* current, Tweeter* prior, 
		  Tweeter* leftNode, Tweeter* rightNode)
{
    current->next = prior;
    current->prev = leftNode;

    prior->next = rightNode;
    prior->prev = current;

    // Change front and rear references if necessary
    if(leftNode != NULL)
        leftNode->next = current;
    else
	tweeterList->front = current;

    if(rightNode != NULL)
	rightNode->prev = prior;
    else
	tweeterList->rear = prior;
    
} // swapTweeters()

/*
 * Move tweeter to correct position after being incremented
 *    it's tweetCount should be <= the previous tweeter's count
 *
 * Such that the list stays in descending order of tweet count
 *      from front to rear.
 */
void moveTweeter(TweeterList* tweeterList, Tweeter* current)
{
    Tweeter* prior = current->prev;
    Tweeter* leftNode = NULL;
    Tweeter* rightNode = NULL;

    // Current is at front of list, then no need to move
    if(prior == NULL)
    {
        return;
    }

    while(current != NULL && prior != NULL &&
		    current->tweetCount > prior->tweetCount) 
    {
        if(prior->prev != NULL) // Setting left node
            leftNode = prior->prev;
	else
	    leftNode = NULL;

        if(current->next != NULL) // Setting right node
            rightNode = current->next;
	else
	    rightNode = NULL;

        // Swapping current and prior tweeters
        swapTweeters(tweeterList, current, prior, leftNode, rightNode);	

        prior = current->prev; // Reset prior
    }

} // swapTweeter()

/*
 * Inserting tweeter
 *
 * The doubly linked list is filled in descending order of tweetcount
 *     from the front. i.e. the front tweter has the most tweets
 * 
 * If it's a new tweeter, insert at rear of list
 * Else, we increment the tweetcount for that tweeter
 *       and then move it to the correct position in the list
 *
 * A small example:
 *
 * <front>                   <rear>
 * - - - - - - -      - - - - - - -
 * | John      |  --> | Jesslyn   |
 * | 15 tweets | <--  | 13 tweets |
 * - - - - - - -      - - - - - - -
 */
void insertTweeter(TweeterList* tweeterList, char* tweeterName)
{

    // The first tweeter encountered
    if(tweeterList->length == 0)
    {
	Tweeter* newTweeter = createTweeter(tweeterName);
	tweeterList->front = newTweeter;
	tweeterList->rear = newTweeter;
        tweeterList->length += 1;
	return;
    }
    
    bool foundTweeter = false;
    Tweeter* current = tweeterList->front;
    for( ; current != NULL; current = current->next)
    {
        if(strcmp(current->name, tweeterName) == 0) // Found!
	{
            foundTweeter = true;
	    current->tweetCount += 1;
            moveTweeter(tweeterList, current);
	    return;
	}	
    }

    if(!foundTweeter) // Tweeter is new to our list
    {
        Tweeter* newTweeter = createTweeter(tweeterName);
	tweeterList->rear->next = newTweeter;
	newTweeter->prev = tweeterList->rear;
	tweeterList->rear = newTweeter;
	tweeterList->length += 1;
    }
   
} // insertTweeter()


/*
 * Debugging print list
 */
void printList(TweeterList* list)
{
    printf("Printing the list of tweeters...\n");

    if(!list || list->length == 0)
    {
        printf("List is empty\n");
        return;
    }

    Tweeter* current = list->front;
    for( ; current != NULL; current = current->next)
    {
        printf("Name: %s, tweet count: %d\n", current->name, current->tweetCount);
    }

} // printList()

/**
 * _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
 * | These are method definitions for  |
 * | ERROR CHECKING FOR FILE VALIDITY  |
 **/ 

/**
 * Returns true if filename is null
 */
bool fileIsNull(char* fileName)
{
    if (fileName == NULL)
        return true;
    return false;
} // fileIsNull()


/**
 * Returns true if file does not end with *.csv
 */
bool fileIsNotCSV(char* fileName)
{
    regex_t regex;
    int retVal = regcomp(&regex,"[*.csv]",0);
    if (retVal == 0)
    {
        retVal = regexec(&regex, fileName, 0, NULL, 0);
        if(retVal != 0) // Filename did not match our regex
        {
            printf("File is not a CSV file\n");
	    return true;
	}
    } 
    return false;    
} // fileIsNotCSV()

/*
 * Returns true if file has
 * - too many chars per line OR
 * - too many lines
 */
bool fileHasTooManyCharsOrLines(char* fileName)
{
    // Check if we exceed the max chars per line
    FILE* fp = fopen(fileName, "r");
    int rowCount = 0;
    int bufferSize = 1000000; // A large number
    char line[bufferSize];

    while (fgets(line, bufferSize, fp))
    {
        rowCount += 1;
        if (strlen(line) > MAXCHARS)
        {
            printf("Too many characters in a line\n");
            return true;
	}
    }

    // Check if we exceed the max num of lines in file
    if(rowCount > MAXLENFILE)
    {
        printf("Too many lines\n");
	return true;
    }

    return false;
} // fileHasTooManyCharsOrLines()

/*
 * fileCheck: Checks if the file has a valid input format
 * i.e. *.csv
 * Args:
 *   - fileName: Name of the file
 * Returns:
 *   - a boolean indicator of whether a file is valid or not. 
 */
bool fileCheck(char* fileName)
{
    if( fileIsNull(fileName) || fileIsNotCSV(fileName) 
                             || fileHasTooManyCharsOrLines(fileName) )
    {
        printf("Invalid Input Format\n");
	exit(1);
    }

    return true;
} // fileCheck()



/**
 * - - - - - - - - - - - -
 * | PROCESS THE CSV FILE |
 * | AND TWEETER DATA     |
 */

/*
 * Returns the position of the name column
 * name header can be either the strings:
 * 1. name
 * 2. "name"
 */
int getPosNameColumn(FILE* fp)
{
    int posNameColumn = 0;
    int indexInLine = 1; // Indexing starts at 1
    char line[MAXCHARS];
    char nameHeaderNoQuotes[] = "name";
    char nameHeaderWithQuotes[] = "\"name\"";
    char* lineCopy = NULL;
    char* token = NULL;

    // Read the file, parse by comma as delimiter
    fgets(line, MAXCHARS, fp);
    lineCopy = strdup(line);
    token = strsep(&lineCopy, ",");

    // Find the name column
    while ((token = strsep(&lineCopy, ",")) != NULL)
    {
        if(strcmp(nameHeaderNoQuotes, token) == 0 ||
	   strcmp(nameHeaderWithQuotes, token) == 0)
        {
            posNameColumn = indexInLine;
            break;	    
	}
	indexInLine += 1;
    }
    
    return posNameColumn;

} // getPosNameColumn()


/**
 * Prints (up to) the top ten tweeters & their tweet counts
 * from a given CSV file
 */ 
void processTweeterData(char* fileName, int posNameColumn)
{
    int indexInFile = 0, indexInLine = 0;
    char line[MAXCHARS];
    char* lineCopy = NULL;
    char* token = NULL;
    char* name = NULL;

    FILE* fp = fopen(fileName, "r");
    fgets(line, MAXCHARS, fp);
    TweeterList* tweeterList = createTweeterList();

    while (fgets(line, MAXCHARS, fp))
    {
        lineCopy = strdup(line);
	indexInLine = 0;

	// Parse tokens with comma as delimiter
	while( (token = strsep(&lineCopy, ",")) != NULL)
        {
	    if(indexInLine == posNameColumn) // Name column
	    {
	       if(token[0] == '\"' && token[strlen(token)-1] == '\"')
	       {
	            // Remove quotes
		    int nameLength = strlen(token) - 2; // -2 to remove quotes
		    int newNameLength = nameLength + 1; // +1 for null termntr

		    // Get the name from in-between the quotes
		    name = (char*) malloc(newNameLength * sizeof(char));
		    memcpy(name, &token[1], nameLength); 
		    name[newNameLength] = '\0';
	       }
	       else // No quotes in name
               {
	           size_t size = strlen(token) + sizeof(char);
		   name = (char*) malloc(size);
		   strncpy(name, token, size);
	       }

	       insertTweeter(tweeterList, name);
	    }
	    indexInLine += 1;
	}
        indexInFile += 1;
    }

    // Debugging
    printList(tweeterList);

} // processTweeterData()


void readFile(char* fileName)
{
    int posNameColumn = 0;
    FILE* fp = fopen(fileName, "r");

    posNameColumn = getPosNameColumn(fp);
    processTweeterData(fileName, posNameColumn);

} // readFile()



/**
  *- - - - - - - - - - - - 
 * | PROGRAM STARTS HERE  |
 * */
int main(int argc, char* argv[])
{
    bool fileIsValid = false;
    fileIsValid = fileCheck(argv[1]);

    if (fileIsValid) // File is valid THUS FAR
    {
        readFile(argv[1]);
    }

} // main()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

#define MAXCHARS (1024)
#define MAXLENFILE (20000)


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
    
    printf("pos name column: %d\n", posNameColumn);
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
    // TODO: create a linked list of tweeters

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

	       printf("name: %s\n", name);
	    }
	    indexInLine += 1;
	}
        indexInFile += 1;
    }

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

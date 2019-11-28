#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

#define MAXCHARS (1024)
#define MAXLENFILE (20000)

/**
 * _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
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
}


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
}

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
}

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

int main(int argc, char* argv[])
{
    bool fileIsValid = false;

    fileIsValid = fileCheck(argv[1]);
} // main()

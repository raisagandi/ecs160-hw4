/*
 * Calculates top 10 tweeters (by volume of tweets)
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


/*
 * FileCheck: Checks if the file has a valid input format i.e. *.csv
 * 		Args:
 * 			 fileName: Name of the file 
 * 		Returns:
 * 			 a boolean indication wheter a file is valid or not.
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
 * 				Args:
 * 					 fp: file 
 * 				Returns:
 * 						index of the column that contains the names				
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
		printf("token is : %s ", token);
		if(strcmp(nameHeaderNoQuotes, token) == 0 || strcmp(nameHeaderWithQuotes, token) == 0)
		{
			posNameColumn = indexInLine;
			break;
		}
		indexInLine += 1;
	}
	
	return posNameColumn;

} 


/*
 * removeSurroundingQuotes: Remove surrounding quotes from name, if any
 * 							e.g. "Amy" becomes Amy
 * 						Args:
 * 							name: given a name, removes quotes and converts into a consistent format.
 */
void removeSurroundingQuotes(char* name)
{
	
	printf("%c\n", name[0]);
	printf("%c\n", name[strlen(name)-1]);
	
	if (name[0] == '\"' && name[strlen(name) - 1] == '\"')
	{
		printf("here\n");
		for(int pos = 0; pos < strlen(name) - 1; pos++)
		{
			name[pos] = name[pos+1];
		}

	}
	printf("%s\n", name);
}

/*
 * TODO
 * Fills an array of {Person : Tweet count} structs
 */
void fillPersonCountArray(PersonCountPair* personCountArray, FILE* fp, int posNameColumn)
{
	int indexInFile = 0, indexInLine = 0;
	char line[MAXCHARS];
	char* lineCopy = NULL;
    char* token = NULL;
	char* name = NULL;

	fgets(line, MAXCHARS, fp); // Ignore first line
	
	while (fgets(line, MAXCHARS, fp))
	{
		lineCopy = strdup(line);
		indexInLine = 0;

		while (token == strsep(&lineCopy, ","))
		{
			if(indexInLine == posNameColumn)
			{
				size_t size = strlen(token) + sizeof(char); // Account for null terminator
				name= (char*) malloc(size);
				strncpy(name, token, size);

				if(name[0] == '\"' && name[strlen(name)-1] == '\"')
				{
					// Remove surrounding quotes		
				}
				break;
			}
			indexInLine += 1;
		}
		indexInFile += 1;
	}

	free(name);
} // fillPersonCountArray()


/*
 * Reads and parses the CSV file
 * Gathers the {Person: Tweet count}
 */
void readFile(char* fileName)
{
	int posNameColumn = 0;
	char line[MAXCHARS];
	char* lineCopy = NULL;
	char* token = NULL;
	FILE* fp = fopen(fileName, "r");	

	// Get the position of the name column in the CSV file
	posNameColumn = getPosNameColumn(fp);
	printf("You entered: %d \n", posNameColumn);
	// Create an array to hold each person-count pair
	PersonCountPair* personCountArray = (PersonCountPair*)malloc(sizeof(PersonCountPair) * MAXLENFILE);
	
	// TODO
	// fillPersonCountArray(personCountArray, fp, posNameColumn);

} // readFile()


int main(int argc, char* argv[])
{

	bool fileIsValid = false;

	if (fileCheck(argv[1])) 
		fileIsValid = true;
	else 
		fileIsValid = false;

	if (fileIsValid)
	{
		readFile(argv[1]); 
	}

	return 0;
} // main()

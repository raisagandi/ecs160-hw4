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
 * Returns true if file is valid
 * false otherwise
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
} // fileCheck()


/*
 * Returns the position (index) of the name column in the CSV file
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

	while (token = strsep(&lineCopy, ","))
	{
		if(strcmp(nameHeaderNoQuotes, token) == 0 || strcmp(nameHeaderWithQuotes, token) == 0)
		{
			posNameColumn = indexInLine;
			break;
		}
		indexInLine += 1;
	}
	return posNameColumn;
} // getPosNameColumn()


void fillPersonCountArray(PersonCountPair* personCountArray, FILE* fp, int posNameColumn)
{
	int indexInFile = 0, indexInLine = 0;
	char line[MAXCHARS];
	char* lineCopy = NULL;
       	char* token = NULL;

	fgets(line, MAXCHARS, fp); // Ignore first line
	
	while (fgets(line, MAXCHARS, fp))
	{
		lineCopy = strdup(line);

		while (token = strsep(&lineCopy, ","))
		{
			printf("token: %s\n", token);
		}
	}

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
	
	// Create an array to hold each person-count pair
	PersonCountPair* personCountArray = (PersonCountPair*)malloc(sizeof(PersonCountPair) * MAXLENFILE);
	
	fillPersonCountArray(personCountArray, fp, posNameColumn);
	
	// {ben: 1}, {ariana: 1}, {marcy: 1}

		/*
		// check if the name has quotes surrounding it
		if (name[0] == '\"' && name[strlen(name) - 1] == '\"')
		{
			// TODO
			// remove the surrounding quotes
			//removeSurroundingQuotes(name);
		}
		*/
		// TODO
		// check if the person's name is in the array
		// if not, then put it in the next index of the array (provided it's inbounds)
		// increment the index, check if it's inbounds
		// and then put the name and set count to 1
		

		// else, increment the count in the struct
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

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

/**
 * return true if file is valid
 * return false otherwise
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


void readFile(char* fileName)
{
	int posNameColumn = 0, posTextColumn = 0, index = 0;
	char nameHeader[] = "name";
	char line[MAXCHARS];
	FILE* fp = fopen(fileName, "r");	

	// Check position of name and tweet in the csv file
	fgets(line, MAXCHARS, fp);
	printf("%s", line);
        char* token = strtok(line, ",");
	
	while(token != NULL)
	{
		if (strcmp(nameHeader, token) == 0)
                        posNameColumn = index;
		token = strtok(NULL, ",");
		index += 1;
	}	

	// Now, read the tweets	
}


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
}

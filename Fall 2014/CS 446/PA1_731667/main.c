#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
	//Process ID and file
	pid_t pid;
	FILE * fin;

	//Counters
	int rowNum = 0;
	int colNum = 0;

	int index = 0;
	int rowIndex = 0;
	int colIndex = 0;

	//Variables for Input
	int character = 1;
	int prevChar = 1;

	char name[32];
    char names[32][32][32];

    //Variable to keep track of data
    int numRows;
    int numCols[32];
    
	int numTabs = 0;
	int tabs = 0;

    int parentRow = 0;
    int foundItself = 0;
    int foundAt = 0;

    //Check if the correct number of arguments are passed to the program
    if( argc != 2 )
    {
    	//If not shout at them and leave
        printf("This progam needs only the filename. \n");
        exit(1);
    }

    fin = fopen(argv[1], "r");

    //Check if the file opened
    if( fin == NULL )
    {
    	//If note shout and leave
    	printf("The file failed to open. \n");
        exit(1); 
    }

    //Get characters until the end of the file
    while(character != EOF )
    {
    	//Get the character
        character = getc(fin);
        name[index] = character;
        
        index++; 

        //Check if it reaches the end of the name
        if( character == ' ')
        {
        	//If so, denote the null char and copy to the names array
            name[index - 1] = '\0';
            strcpy(names[rowNum][colNum],name);
            
            //update counters
            colNum++;
            index = 0;
            
        }
        
        //Check if reaches end of the name
        if( character == '\n' && prevChar != '\n')
        {
        	//Replace final char and copy to names array
        	name[index - 1] = '\0';
            strcpy(names[rowNum][colNum],name);

            //update counters
            numCols[rowNum] = colNum + 1;
            rowNum++;
            colNum = 0;
            index = 0;
            numRows++;	
        }

        //Keep track of the last character just in case an empty line is inputted
        prevChar = character;
    }

    //close the file
    fclose(fin);

    //Reset the indices
    rowIndex = 0;
    colIndex = 0;

    //Loop for every name in each line
    while(colIndex < numCols[parentRow])
    {
    	//Keep track of the tabs to print: IE insert a tab based on children
    	if(colIndex == 2)
    	{
    		numTabs++;
    	}

    	//fork
    	pid = fork();

    	//check if for failed
    	if( pid < 0 )
    	{
    		//shout and leave if so
            printf("Fork() failed\n");
            exit(1);
        }

        //Child
        else if( pid == 0 )
        {
        	//Start looking for itself below it
        	rowIndex = parentRow + 1;

        	//Loop until the name finds itself at the first name of the row
        	while( rowIndex < numRows && foundItself == 0 )
        	{
        		if( strcmp( names[parentRow][colIndex], names[rowIndex][0] ) == 0 )
        		{
        			foundItself = 1;
        			foundAt = rowIndex;
        		}

        		rowIndex++;
        	}

        	//If it did not find itself, print its name and process, it is a child process
        	if(foundItself == 0)
        	{
        		//Case: no spouse
        		if( colIndex == 0 && numCols[parentRow] == 1)
        		{
                    //Print tabs based on position
                    while(tabs < numTabs )
                    {
						printf("\t");
						tabs++;
                    }
                    
                    //Print itself and process id
					printf("%s(%i)\n", names[parentRow][colIndex], getppid() );
        		}

        		//Case: spouse
        		else if( colIndex == 0 && numCols[parentRow] > 1 )
        		{
                    //Print tabes
                    while(tabs < numTabs)
                    {
						printf("\t");
						tabs++;
                    }
                    
                    //Print itself pid and spouse
					printf("%s(%i)", names[parentRow][colIndex], getppid() );
                    colIndex++;
                    printf("-%s\n", names[parentRow][colIndex]);
        		}	

        		//Case child
        		else if(colIndex != 1 )
        		{
					while(tabs < numTabs)
                	{
						printf("\t");
						tabs++;
                	}
                    
                    
                // print name and pid
				printf("%s(%i)\n", names[parentRow][colIndex], getppid() );
        		}

        		return 0;
        	}

        	//reset counters
        	else
        	{
        		foundItself = 0;
        		parentRow = foundAt;
        		colIndex = 0;
        	}
        }

        //move on after waiting
        else
        {
        	colIndex++;
        	waitpid(pid, NULL, 0);
        }
    }

    return 0;
}
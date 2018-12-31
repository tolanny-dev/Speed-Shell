/******************************************************************************
	Author: David Shittu

	This program simulates a console. It recieves input from the user, reads it
	and stores in an array. The contents of the array are then displayed to the
	console.

	Source: davidshell.cpp

	Version: 1.0.0
******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

using namespace std;

// Parse function gets input from user and stores in an array
void parse (char **arr, int &len);
//Post: Reads input from user and creates a string array (arr) of words gottten
//from the user input it also keeps track of the amount of words in the array
//with the len variable

int main()
{
	cout<<"DaviD Shell TM"<<endl;

	while (true) //Forever loop to keep shell running until terminated by user
	{
		char **arr; // Array to collect user input
		arr = new char * [200];
		for (int i = 0; i < 200; i++)
		{
			arr[i] = new char [200];
		}

	  int argCount = 0;
		parse(arr, argCount);

		//QUIT COMMAND
		 //Terminates shell when user enters quit
		if ((strcmp(arr[0], "QUIT") == 0) || (strcmp(arr[0], "quit") == 0))
		{
			cout<<"\n Shell Terminated "<<endl;
			cout<<"DaviD Shell TM"<<endl;
			break;
		}

		// SHOW COMMAND
		// This command shows contents of given file
		if ((strcmp(arr[0], "SHOW") == 0) || (strcmp(arr[0], "show") == 0))
		{
			ifstream fl;

			// Attempt to open the file
			fl.open(arr[1]);

			// To check to make sure the it is opened
			if(fl.fail())
			{
				cout<< "There was an error opening file"<<endl;
			}
			else
			{
				char c;

				while(!fl.eof())
				{
					fl.get(c);
					cout<<c;
				}
			}
		}

		// HELP COMMAND
		// This command will list all available shell commands
		if ((!strcmp(arr[0], "HELP")) || (strcmp(arr[0], "help") == 0))
		{
			cout<<"Commands that can be run in this shell --> "<<endl <<endl;
			cout<<"RUN"<<endl;
			cout<<"RUN <<FileName>>"<<endl;
			cout<<"This command will run an executable file"<<endl<<endl;
			cout<<"COPY"<<endl;
			cout<<"COPY <<oldName>> <<newName>>"<<endl;
			cout<<"This command will copy contents of one file into another"<<endl<<endl;
			cout<<"LIST"<<endl;
			cout<<"LIST <<directoryName>>"<<endl;
			cout<<"This command will list all items in given directory"<<endl<<endl;
			cout<<"HELP"<<endl;
			cout<<"This command will list all available shell commands"<<endl<<endl;
			cout<<"SHOW"<<endl;
			cout<<"SHOW <<fileName>>"<<endl;
			cout<<"This command shows contents of given file"<<endl<<endl;
			cout<<"CD"<<endl;
			cout<<"This command changes directory"<<endl<<endl;
			cout<<"QUIT"<<endl;
			cout<<"This command terminates the shell"<<endl;
			cout<<"DaviD Shell TM"<<endl;
		}


		// COPY COMMAND
		// This command will copy contents of one file into another
		if ((strcmp(arr[0], "COPY") == 0) || (strcmp(arr[0], "copy") == 0))
		{
			ifstream sFile;

			// Attempt to open the file
			sFile.open(arr[1]);

			// Output filestream variable
			ofstream outputFile;

			outputFile.open(arr[2]);

			// To check to make sure the it is opened
			if(outputFile.fail() || sFile.fail())
			{
				cout<< "Error Opening File"<<endl;
			}
			else
			{
				char ch;

				while(!sFile.eof())
				{
					sFile.get(ch);
					outputFile.put(ch);
				}
			}
		}


		// RUN COMMAND
		// This command will run an executable file
		if ((strcmp(arr[0], "RUN") == 0) || (strcmp(arr[0], "run") == 0))
		{
		    pid_t stat1;
				pid_t stat2;

		    if(argCount != 2 && argCount != 3) {
		    	cerr << "RUN: Expecting 1 or 2 args." << endl;
		    	continue;
		    }

		    stat1 = fork();

		    if (stat1 < 0)
			{
		        cout<<"Error forking a child process"<<endl;
		        exit(1);
		    }
		    else if (stat1 == 0)
			{   /* for the child process:

				*/
				if(argCount == 2)
				{
					execlp(arr[1], arr[1], NULL);
		        }
		        else if(argCount == 3)
		        {
		            execlp(arr[1], arr[1], arr[2], NULL);
		        }
		    }
		    else
		    {
		    	waitpid(stat1, &stat2, 0);
		    }
		}

		// LIST COMMAND
		// This command will list all items in given directory
		if ((strcmp(arr[0], "LIST") == 0) || (strcmp(arr[0], "list") == 0))
		{
			DIR * direc;
			struct dirent * ent;

			//It will open the current directory
			//If there is no directory specified
			if(argCount == 1)
			{
				direc = opendir(getcwd(NULL,256));
			}
			//It will open the specified directory
			//Specified directory is the one entered by the user after LIST
			else if(argCount == 2)
			{
				direc = opendir(arr[1]);
			}
			else
			{
				//If user inputs two arguments i.e LIST <foldername> <foldername>
				cerr << "You can only open one file at a time." << endl;
				continue;
			}

			if(direc == NULL)
			{
				cerr<<"Error";
			}
			int i = 0;
			while((ent = readdir(direc)) != NULL)
			{
				cout << ent->d_name<<endl;
				i++;
			}
			closedir(direc);

		}


		//CD COMMAND
		// This command changes directory
		if ((strcmp(arr[0], "CD") == 0) || (strcmp(arr[0], "cd") == 0))
		{
			if( argCount != 2 )
			{
		  		cerr << "Directory must be specified." << endl;
		  		continue;
			}

			if( chdir(arr[1]) == -1 )
			{
				perror("Unable to change directories");
		  		continue;
			}
			else
			{
				chdir(arr[1]);
				cout << "new directory:" << arr[1] << endl;
			}
		}
}

	return 0;
}


void parse(char **arr, int &len) //Parse function definition
{
	int i = 0;
	char ch;

	char tmpry[200];

	cout << "\n [*_*] --->> ";

	cin.get(ch);

	while (ch !='\n') //check for whitespace to avoid it in the array
	{
		while(isspace(ch))
		{
			i =0;
			if(ch =='\n')
	    {
				break;
			}
			cin.get(ch);
		}

		while(!isspace(ch)) // push individual characters into the array
		{
			if(ch =='\n')
	    {
				break;
	    }
			tmpry[i++] = ch;
			cin.get(ch);
		}

		if (i > 0)
		{
			tmpry[i] = '\0';
			strcpy(arr[len],tmpry); // string copy method to copy the word to arr[]
			len++;
		}
	}
}

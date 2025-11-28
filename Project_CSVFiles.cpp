#include <iostream>
#include "UserInterface.h"
#include <string>
#include <fstream>


int main()
{
	UserInterface UserInterface;
	UserInterface.run();	

	//Example use:
	//Enter command : open
	//Enter filename: TestFile.csv
	//Does the file have a header? (Type yes or no): yes
	//Enter delimiter or press Enter for default ','): ,
	//Enter command: removecol
	//Enter column name or index: 1
	//Enter command: print
	//Enter command: exit
}


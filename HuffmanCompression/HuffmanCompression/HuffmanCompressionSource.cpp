#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Please rerun the program and type a source file to be compressed." << endl;
	}

	else
	{
		//argv[1] contains the filename to be provided
		string inFileName = argv[1];
		ifstream fin(inFileName);
		if (fin.fail())
			cout << "Could not open target file, please rerun the program with a valid text file (.txt)." << endl;
		else
		{
			string outFileName = inFileName.substr(0, inFileName.length() - 3) + "mcp"; //assumes a .txt file 
			
			//run the compression algorithm
		}
	}

	return 0;
}
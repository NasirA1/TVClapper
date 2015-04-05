#include <iostream>
#include "File.h"
#include "IR.h"
#include <stdlib.h>

using namespace std;


void usage()
{
	cout << "Usage: IRParser [inputfile] [outputfile] [-v]" << endl;
}


int main(int argc, char* argv[])
{

	if(argc < 3)
	{
		usage();
		exit(0);
	}

	const char* inputfile = argv[1];
	const char* outputfile = argv[2];
	bool verbose = false;

	if(argc >= 4)
	{
		verbose = string(argv[3]) == "-v";
	}

	File inFile(inputfile, "r");
	string line;
	IRSignal irSignal;

	while(!inFile.isEof())
	{
		try
		{
			line = inFile.readNextLine();
			IR ir(line);
			irSignal.push_back(ir);
			cout << ir.toString() << endl;
		}
		catch(int e)
		{
			if(verbose) cout << "Skipping [" << line << "]" << endl;
		}
	}

	cout << irSignal.toIGArray() << endl;

	File outFile(outputfile, "w");
	outFile.write(irSignal.toIGArray().c_str());
}

#include "File.h"
#include <string.h>

File::File(char const* const filename, const char* mode)
	: eof_(false)
{
	file_ = fopen(filename, mode);

	if (!file_) 
	{
		throw runtime_error("File open failure");
	}
}


File::~File() 
{
	if (fclose(file_)) 
	{
		// failed to flush latest changes.
		// handle it
	}
}


void File::write(char const* const str) 
{
	if (EOF == fputs(str, file_)) 
	{
		throw runtime_error("File write failure");
	}
}


string File::readNextLine()
{
	char line[20] = {0};

	if(fgets(line, sizeof(line), file_) != NULL) 
	{
		const int len = strlen(line);
		line[len - 1] = '\0'; //remove newline char
		return line;
	}
	else
	{
		eof_ = true;
		return "";
	}
}

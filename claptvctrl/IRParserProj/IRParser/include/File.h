#include <cstdio>
#include <stdexcept> // std::runtime_error

using namespace std;


class File 
{
private:
	FILE* file_;
	bool eof_;

public:
	File(char const* const filename, const char* mode);
	~File();
	void write(char const* const str);
	string readNextLine(void);
	inline bool isEof(void) const { return eof_; }

private:
	// prevent copying and assignment; not implemented
	File(const File&);
	File& operator=(const File&);
};

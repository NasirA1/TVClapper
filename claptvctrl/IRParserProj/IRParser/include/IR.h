#include <string>
#include <vector>

using namespace std;

const string PULSE_TOK("pulse:");
const string SPACE_TOK("space:");


enum IRTYPE
{
	PULSE, SPACE
};

struct IR
{
	IRTYPE type;
	unsigned int length;

	IR(const string& irString);
	string toString();

private:
	unsigned int parseLength(const string& str);
};



class IRSignal : public vector<IR>
{
public:
	string toIGArray();
};
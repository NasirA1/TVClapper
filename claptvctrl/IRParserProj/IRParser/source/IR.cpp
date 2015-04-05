#include "IR.h"
#include "stringutils.h"
#include <sstream>
#include <stdlib.h>


IR::IR(const string& irString)
{
	string tok = trim_copy(irString);

	if(tok.length() >= 6) tok = tok.substr(0, 6);

	if(tok == PULSE_TOK) type = PULSE;
	else if(tok == SPACE_TOK) type = SPACE;
	else throw -1;

	length = parseLength(irString);
}


unsigned int IR::parseLength(const string& str)
{
	int pos = str.find(':') + 2;
	string subs = str.substr(pos, str.length() - pos);
	return atoi(subs.c_str());
}


string IR::toString()
{
	stringstream ss;
	ss << "(" << (type == PULSE ? "PULSE" : "SPACE") << "," << length << ")";
	return ss.str();
}


string IRSignal::toIGArray()
{
	stringstream ss;
	ss << "unsigned int signal[] = {" << endl;

	for(vector<IR>::iterator it = this->begin(); it != this->end(); ++it)
	{
		if(it->type == PULSE)
		{
			ss << '\t' << it->length << " | IG_PULSE_BIT," << endl;
		}
		else
		{
			ss << '\t' << it->length << "," << endl;
		}
	}

	ss << "};" << endl;
	return ss.str();
}

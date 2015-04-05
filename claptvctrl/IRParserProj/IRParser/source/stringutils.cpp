#include "stringutils.h"


string trim_right_copy(const string& s, const string& delimiters)
{
  return s.substr(0, s.find_last_not_of(delimiters) + 1);
}

string trim_left_copy(const string& s, const string& delimiters)
{
  return s.substr(s.find_first_not_of(delimiters));
}

string trim_copy(const string& s, const string& delimiters)
{
	if(s.length() <= 0) return s;
  return trim_left_copy(trim_right_copy(s, delimiters), delimiters);
} 

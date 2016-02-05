#ifndef LUALA_H
#define LUALA_H

#include <string>

using std::string;

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class Luala
{
private:
	lua_State *LS;

public:
	Luala(string filename);
	~Luala();

	int getInt(string text);
	string getString(string text);
	bool getBool(string text);
};

#endif
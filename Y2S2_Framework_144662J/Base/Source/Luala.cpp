#include "Luala.h"

Luala::Luala(string filename)
{
	LS = lua_open();
	luaL_openlibs(LS);
	luaL_dofile(LS, filename.c_str());
}

Luala::~Luala()
{
	lua_close(LS);
}

int Luala::getInt(string text)
{
	lua_getglobal(LS, text.c_str());
	return lua_tonumber(LS, -1);
}

string Luala::getString(string text)
{
	lua_getglobal(LS, text.c_str());
	return lua_tostring(LS, -1);
}

bool Luala::getBool(string text)
{
	lua_getglobal(LS, text.c_str());
	return lua_toboolean(LS, -1);
}
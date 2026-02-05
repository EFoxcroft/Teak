#include <iostream>
//#include <cassert>
//#include <string>
//#include <vector>
//#include <memory>
//#include <chrono>
using namespace std;

extern "C"
{
    // #include "../lua.h"
    // #include "../lualib.h"
    // #include "../lauxlib.h"
}

#define lua_version "Lua 5.4"

int main()
{
/*
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dostring(L, "local mango = 6 + 9 + 6 * 9 ; io.write(mango, '\\n')");

    char strong[] = "a = 4 ; b = 3.0 ; c = a * b";
    luaL_dostring(L, strong);
    lua_getglobal(L, "a");
    int i = (int)lua_tonumber(L, -1);
    lua_getglobal(L, "b");
    double j = (double)lua_tonumber(L, -1);
    lua_getglobal(L, "c");
    int ij = lua_tonumber(L, -1);
    printf("%d * %lf = %d\n", i, j, ij);

    lua_close(L);
*/

    cout << lua_version << '\n';

    return 0;
}
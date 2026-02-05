#include <iostream>
//#include <cassert>
//#include <string>
//#include <vector>
//#include <memory>
//#include <chrono>
using namespace std;

extern "C"
{
    #include "../lua_548/lua.h"
    #include "../lua_548/lualib.h"
    #include "../lua_548/lauxlib.h"
}

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dostring(L, "local demo = 6 + 9 + 6 * 9 ; io.write(demo, '\\n')");

    char literal[] = "a = 4 ; b = 3.0 ; c = a * b";
    luaL_dostring(L, literal);
    lua_getglobal(L, "a");
    int var_L = (int)lua_tonumber(L, -1);
    lua_getglobal(L, "b");
    double var_R = (double)lua_tonumber(L, -1);
    lua_getglobal(L, "c");
    int vars = lua_tonumber(L, -1);
    printf("%d * %lf = %d\n", var_L, var_R, vars);

    lua_close(L);

    return 0;
}
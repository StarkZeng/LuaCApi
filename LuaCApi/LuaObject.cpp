//
//  LuaObject.c
//  LuaCApi
//
//  Created by zengqiang on 15/5/18.
//
//

extern "C" {
 #include "lua.h"
 #include "lualib.h"
 #include "lauxlib.h"
}
#include "LuaObject.h"


lua_State *L;

//open for lua state for use
void initLuaState()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

void closeLuaState()
{
    lua_close(L);
}



// dump the stock in L
static void stackDump(lua_State* L)
{
    printf("---------------- dump lua value :\n");
    int i;
    int top = lua_gettop(L);
    for(i = 1; i <= top; ++i)
    {
        int t = lua_type(L, i);
        switch(t)
        {
            case LUA_TSTRING:
                printf("'%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf(lua_toboolean(L, i) ? "true": "false");
                break;
            case LUA_TNUMBER:
                printf("'%g'", lua_tonumber(L, i));
                break;
            default:
                printf("'%s'", lua_typename(L, t));
                break;
        }
    }
    printf("\n");
}


void error(lua_State* L, const char* fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}


//////////////////////////////////////////////////////////////////
///register the c function for lua call it
//////////////////////////////////////////////////////////////////

// method 1.

static int add(lua_State *L)
{
    double op1 = luaL_checknumber(L, 1);
    double op2 = luaL_checknumber(L, 2);
    
    lua_pushnumber(L, op1 + op2);
    return 1;
}


void registerFunction()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    
    lua_register(L, "add", add);
    
    lua_close(L);
}

//method 2.
static luaL_Reg myLibs[] =
{
    {"add",add},
    {NULL,NULL}
};

int registerLibs()
{
    const char* libName = "mytestlib";
    luaL_register(L,libName,myLibs);
    return 1;
}



//////////////////////////////////////////////////////////////////
///tying to excute a lua file
//////////////////////////////////////////////////////////////////

void doLuaFile(const char *filename)
{
    
    printf("doluaFile %s\n",filename);
    
    
    if(luaL_dofile(L, filename))
    {
        error(L, "cannot run configuration file: %s", lua_tostring(L, -1));
    }
    
    
    //获得lua的全局变量
    lua_getglobal(L, "width");
    lua_getglobal(L, "height");
    
    if(!lua_isnumber(L, -2))
        error(L, "'width' should be a number\n");
    if(!lua_isnumber(L, -1))
        error(L, "'height' should be a number\n");
    int width = (int)lua_tonumber(L, -2);
    int height = (int)lua_tonumber(L, -1);
    //stackDump(L);
    lua_pop(L,2);//lua_tonumber 会把参数压入堆栈 使用完毕后清除掉
    //stackDump(L);
    
    
    
    //设置lua全局变量的值 调用完成后相当于在lua里面赋值 a ＝ 60
    lua_pushnumber(L,60);
    lua_setglobal(L, "a");
    //注意 lua_getglobal 会自动回弹栈内的元素 相当于自动调用了lua_pop(L,2);
    
    
    //调用lua全局函数luaFunc 如下代码相当于lua代码：luaFunc("how",40)
    lua_getglobal(L, "luaFunc");
    lua_pushliteral(L,"how");
    lua_pushinteger(L,14);
    lua_call(L, 2, 1);
    //和lua_getglobal 一样会回弹栈内元素 并且会把返回值压入栈内 通过lua_to* 可以获得这些返回值
    int returnVar = lua_tonumber(L,-1);
    stackDump(L);
    lua_pop(L,1);
}

//#pragma once
//#include <functional>
//extern "C"{
//    #include <lua.h>
//    #include <lualib.h>
//    #include <lauxlib.h>
//}
////#include <luabind/luabind.hpp>
//#include "../render/LogManager.h"
//
/////Wrap lua using R.A.I.I.
//class Lua_State
//{
//public:
//    Lua_State()
//		: L(luaL_newstate())
//	{
//        //luabind::open(L);//Connect automatically luabind to all states
//    }
//    ~Lua_State() { lua_close(L); }
//    operator lua_State*() { return L; }//implicit conversion Lua_State ------> lua_state*
//private:
//    lua_State *L;
//};
//
//inline static void LuaOpenLibs(lua_State *L){ //convenience functions;
//        luaopen_io(L);
//        luaopen_base(L);
//        luaopen_table(L);
//        luaopen_string(L);
//        luaopen_math(L);
//        luaL_openlibs(L);
//}
//
//
//inline static void ReportLuaError(lua_State *L, int status){
//    if(status != 0){
//        LOG_ERROR_INFO("LuaLoader", "ERROR : LUA ERROR", lua_tostring(L, -1));
//        lua_pop(L, 1);
//    }
//}
//
//inline static int LuaExecuteScript(lua_State *L){
//    return lua_pcall(L, 0, LUA_MULTRET, 0);
//}
//
//
//class LuaScript
//{
//public:
//    LuaScript() : m_luaState(Lua_State()), m_status(0){}
//    ~LuaScript(){}
//
//    bool Load(const char* scriptName);
//    bool ExecuteScript();
//    Lua_State& LuaState() { return m_luaState; }
//
//    //User registers his own types / functions here...
//	/*
//	void ImplementApplicationInterface(std::function<void()> function){
//        try
//        {
//            function();
//        }catch(luabind::error& e){
//            LOG_ERROR_INFO("ERROR", "A LUA BIND ERROR OCCURED : ", e.what());
//        }catch(luabind::cast_failed& e){
//            LOG_ERROR_INFO("ERROR", "A LUA BIND ERROR OCCURED : ", e.what());
//        }
//    }
//	*/
//private:
//    Lua_State m_luaState;
//    int m_status;
//};

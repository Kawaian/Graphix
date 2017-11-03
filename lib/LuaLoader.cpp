//
//#include "../include/Graphics/LuaLoader.h"
//
//bool LuaScript::Load(const char* scriptName){
//        LuaOpenLibs(m_luaState);
//        LOG_INFO("Lua", "Loading file", scriptName);
//        m_status = luaL_loadfile(m_luaState, scriptName);
//        ReportLuaError(m_luaState, m_status);
//        return m_status;
//}
//
//bool LuaScript::ExecuteScript(){
//    if(m_status == LUA_OK)
//        LuaExecuteScript(m_luaState);
//    ReportLuaError(m_luaState, m_status);
//    return m_status;
//}

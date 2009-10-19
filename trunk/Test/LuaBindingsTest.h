#ifndef LUABINDINGSTEST_H_INCLUDED
#define LUABINDINGSTEST_H_INCLUDED


#include "XULWin/Lua/XULRunnerWithLua.h"
#include <string>


namespace XULWin
{


    class LuaBindingsTest
    {
    public:
        LuaBindingsTest();

        ~LuaBindingsTest();

        void runXULSample(const std::string & inAppname);

        void log(const std::string & inMessage);

    private:
        Lua::XULRunnerWithLua mLoggerRunner;
        ElementPtr mLoggerApp;
    };


} // namespace XULWin


#endif // LUABINDINGSTEST_H_INCLUDED

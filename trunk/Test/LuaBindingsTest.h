#ifndef LUABINDINGSTEST_H_INCLUDED
#define LUABINDINGSTEST_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/Lua/XULRunnerWithLua.h"


namespace XULWin
{


    class LuaBindingsTest
    {
    public:
        LuaBindingsTest();

        ~LuaBindingsTest();

        void run(const std::string & inAppname);

    private:
        Lua::XULRunnerWithLua mXULRunnerWithLua;
    };


} // namespace XULWin


#endif // LUABINDINGSTEST_H_INCLUDED

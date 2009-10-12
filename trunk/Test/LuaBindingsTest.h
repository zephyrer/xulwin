#ifndef LUABINDINGSTEST_H_INCLUDED
#define LUABINDINGSTEST_H_INCLUDED


#include "XULWin/Lua/LuaBindings.h"
#include <boost/scoped_ptr.hpp>
#include <string>


namespace XULWin
{

    namespace Lua
    {
        class LuaInitializer;
    }


    class LuaBindingsTest
    {
    public:
        LuaBindingsTest();

        ~LuaBindingsTest();

        void run();

    private:
        boost::scoped_ptr<Lua::LuaInitializer> mInitializer;
    };


} // namespace XULWin


#endif // LUABINDINGSTEST_H_INCLUDED

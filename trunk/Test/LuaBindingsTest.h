#ifndef LUABINDINGSTEST_H_INCLUDED
#define LUABINDINGSTEST_H_INCLUDED


#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Element.h"
#include "XULWin/Parser.h"
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
        LRESULT handleButtonPressed();
        boost::scoped_ptr<Lua::LuaInitializer> mInitializer;
        ElementPtr mRootElement;
    };


} // namespace XULWin


#endif // LUABINDINGSTEST_H_INCLUDED

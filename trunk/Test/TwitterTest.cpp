#include "TwitterTest.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>


namespace XULWin
{


    TwitterTest::TwitterTest()
    {
        Poco::AutoPtr<Poco::XML::Document> friendStatuses = mTwitter.getFriendStatuses();
        friendStatuses->getElementById("text");
    }



} // namespace XULWin

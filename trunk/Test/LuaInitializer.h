#ifndef LUAINITIALIZER_H_INCLUDED
#define LUAINITIALIZER_H_INCLUDED


#include <boost/function.hpp>
#include <string>


namespace XULWin
{

namespace Lua
{

    class LuaInitializer
    {
    public:
        LuaInitializer();

        ~LuaInitializer();

        bool loadFile(const std::string & inLuaFile);

        bool loadScript(const std::string & inScript);

        boost::function<void(const std::string &)> Logger;

    private:
        void log(const std::string & inMessage);

    };


} // namespace Lua

} // namespace XULWin


#endif // LUAINITIALIZER_H_INCLUDED

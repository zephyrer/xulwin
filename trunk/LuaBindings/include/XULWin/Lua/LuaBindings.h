#ifndef LUABINDINGS_H_INCLUDED
#define LUABINDINGS_H_INCLUDED


#include <string>


namespace XULWin
{

namespace Lua
{

    void ShowMessage(const std::string & inString);


    /**
     * LuaBindings is a swig-compatible class.
     */
    class Element
    {
    public:
        Element();

        int testSum(int a, int b);

        Element * getElementById();

    };


} // namespace Lua

} // namespace XULWin


#endif // LUABINDINGS_H_INCLUDED

#include "XULWin/ToolbarMenuItem.h"


namespace XULWin
{

    namespace WinAPI
    {


        ToolbarMenuItem::ToolbarMenuItem()
        {
        }


        ToolbarMenuItem::ToolbarMenuItem(int inId, const std::string & inText, const boost::function<void()> & inAction) :
            mAction(inAction)
        {
        }


    } // namespace WinAPI

} // namespace XULWin

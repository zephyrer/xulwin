#include "XULWin/ToolbarMenuItem.h"


namespace XULWin
{

namespace Windows
{


	ToolbarMenuItem::ToolbarMenuItem()
	{
	}


    ToolbarMenuItem::ToolbarMenuItem(int inId, const std::string & inText, const boost::function<void()> & inAction) :
        mAction(inAction)
	{
	}


} // namespace Windows

} // namespace XULWin

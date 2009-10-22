%module XULWin
%include stl.i
%naturalvar;


%{
#include "XULWin/AttributesMapping.h"
#include "XULWin/Element.h"
#include "XULWin/Lua/LuaBindings.h"
%}
 
%include "../XULWin/include/XULWin/AttributesMapping.h"
%include "../XULWin/include/XULWin/Element.h"
%include "include/XULWin/Lua/LuaBindings.h"


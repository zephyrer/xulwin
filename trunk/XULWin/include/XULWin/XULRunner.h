#ifndef XULRUNNER_H_INCLUDED
#define XULRUNNER_H_INCLUDED


#include "XULWin/Parser.h"
#include "XULWin/Windows.h"
#include <string>


namespace XULWin
{
    
    class XULRunner
    {
    public:
        XULRunner(HMODULE inModuleHandle);

        /**
         * Runs a XUL applications. Requires that the main XUL file is a
         * WindowElement root element.
         */
        void run(const std::string & inApplicationIniFile);

        ElementPtr loadApplication(const std::string & inApplicationIniFile);

        ElementPtr loadXUL(const std::string & inXULUrl);

        ElementPtr rootElement() const;

        HMODULE getModuleHandle() const;

    private:
        HMODULE mModuleHandle;
        Parser mParser;
        ElementPtr mRootElement;
    
    };

} // XULWin


#endif // XULRUNNER_H_INCLUDED

#ifndef XULRUNNER_H_INCLUDED
#define XULRUNNER_H_INCLUDED


#include "XULWin/Fallible.h"
#include "XULWin/Parser.h"
#include "XULWin/Windows.h"
#include <string>


namespace XULWin
{
    
    class XULRunner
    {
    public:
        // Gets the previously set locale.
        // If no locale has been set en-US is returned.
        static std::string GetLocale();

        // Sets the desired locale (en-US, fr, nl, ...)
        static void SetLocale(const std::string & inLocale);


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
        static Fallible<std::string> sLocale;

        HMODULE mModuleHandle;
        Parser mParser;
        ElementPtr mRootElement;    
    };

} // XULWin


#endif // XULRUNNER_H_INCLUDED

#ifndef XULRUNNER_H_INCLUDED
#define XULRUNNER_H_INCLUDED


#include "XULWin/Fallible.h"
#include "XULWin/XULParser.h"
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
        // NOTE: the locale statements in the chrome.manifest file
        // are ignored, instead we depend on the foldernames in the chrome/locale directory.
        // This means that the locale value should be identical to its foldername.
        static void SetLocale(const std::string & inLocale);


        XULRunner(HMODULE inModuleHandle);

        /**
         * Runs a XUL applications. Requires that the main XUL file is a
         * WindowElement root element.
         */
        void run(const std::string & inApplicationIniFile);

        ElementPtr loadApplication(const std::string & inApplicationIniFile);

        ElementPtr loadXUL(const std::string & inXULUrl);

        void loadOverlay(const std::string & inXULUrl);

        ElementPtr rootElement() const;

        HMODULE getModuleHandle() const;

    private:
        static Fallible<std::string> sLocale;

        static ElementPtr Parse(AbstractXULParser & inParser, const std::string & inXULURL);
        std::string getOverlayElementId(const std::string & inXULURL);

        HMODULE mModuleHandle;
        XULParser mParser;
        ElementPtr mRootElement;    
    };

} // XULWin


#endif // XULRUNNER_H_INCLUDED

#include "XULWin/XULRunner2.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/XULOverlayParser.h"
#include "XULWin/WindowElement.h"
#include "XULWin/WinUtils.h"
#include "Poco/DOM/DOMBuilder.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"


namespace XULWin
{

    typedef std::map<std::string, std::string> Prefs;


    bool parsePrefsLine(const std::string & inPrefsLine, std::pair<std::string, std::string> & outPref);
    bool getPrefs(const std::string & inPrefsFile, Prefs & outPrefs);
    std::string getMainXULFile(const Poco::Path & inTopLevelAppDir);


    Fallible<std::string> XULRunner2::sLocale;
    HMODULE XULRunner2::sModuleHandle(0);


    XULRunner2::XULRunner2()
    {
        if (sModuleHandle == 0)
        {
            throw std::runtime_error("No module handle set for XULRunner2 class. Did you forget to create the XULWin::Initializer object?");
        }
        mModuleHandle = GetModuleHandle();
    }


    XULRunner2::XULRunner2(HMODULE inModuleHandle) :
        mModuleHandle(inModuleHandle)
    {
    }


    std::string XULRunner2::GetLocale()
    {
        return sLocale.or(Defaults::defaultLocale());
    }
    
    
    void XULRunner2::SetLocale(const std::string & inLocale)
    {
        sLocale = inLocale;
    }
    

    HMODULE XULRunner2::GetModuleHandle()
    {
        return sModuleHandle;
    }

    
    void XULRunner2::SetModuleHandle(HMODULE inModuleHandle)
    {
        sModuleHandle = inModuleHandle;
    }


    void XULRunner2::run(const std::string & inApplicationIniFile)
    {
        XULParser parser;
        Poco::Path topLevelAppDir = Windows::getCurrentDirectory();
        std::string mainXULFile = getMainXULFile(topLevelAppDir);
        parser.parse(mainXULFile);
        if (WindowElement * window = parser.rootElement()->downcast<WindowElement>())
        {
            window->showModal(WindowElement::CenterInScreen);
        }
    }


    ElementPtr XULRunner2::ParseFile(AbstractXULParser & inParser, const std::string & inFilePath)
    {
        ElementPtr result;
        try
        {
            // Somewhat hackish solution to make both XULParser and DOMBuilder content handlers.
            Poco::XML::ContentHandler * myContentHandler = inParser.getContentHandler();
            Poco::XML::DOMBuilder domBuilder(inParser); // here the content handler gets overwritten
            Poco::XML::ContentHandler * pocoContentHandler = inParser.getContentHandler();
            inParser.setContentHandler(myContentHandler);
            inParser.setExtraContentHander(pocoContentHandler);
            domBuilder.parse(inFilePath);
            result = inParser.rootElement();
        }
        catch (Poco::Exception & exc)
        {
            ReportError(exc.displayText());
        }
        return result;
    }


    ElementPtr XULRunner2::ParseString(AbstractXULParser & inParser, const std::string & inXULString)
    {
        ElementPtr result;
        try
        {
            inParser.parseString(inXULString);
            result = inParser.rootElement();
        }
        catch (Poco::Exception & exc)
        {
            ReportError(exc.displayText());
        }
        return result;
    }


    ElementPtr XULRunner2::loadApplication(const std::string & inApplicationIniFile)
    {
        Poco::File file(inApplicationIniFile);
        if (!file.exists())
        {
            std::string msg = "The current working directory does not contain a file named ";
            msg += "'" + inApplicationIniFile + "'.";
            msg += " Please set the current directory to the one that contains this file.";
            throw std::runtime_error(msg.c_str());
        }

        XULParser parser;
        mRootElement = ParseFile(parser, getMainXULFile(Windows::getCurrentDirectory()));
        return mRootElement;
    }


    ElementPtr XULRunner2::loadXULFromFile(const std::string & inXULURL)
    {
        XULParser parser;
        ChromeURL url(inXULURL);
        mRootElement = ParseFile(parser, url.convertToLocalPath());
        return mRootElement;
    }


    ElementPtr XULRunner2::loadXULFromString(const std::string & inXULString)
    {
        XULParser parser;
        mRootElement = ParseString(parser, inXULString);
        return mRootElement;
    }


    std::string XULRunner2::getOverlayElementId(const std::string & inXULURL)
    {
        XULRunner2 temp(::GetModuleHandle(0));
        temp.loadXULFromFile(inXULURL);
        if (!temp.rootElement() || temp.rootElement()->children().empty())
        {
            return "";
        }
        Element * child = temp.rootElement()->children()[0].get();
        if (child)
        {
            return child->getAttribute("id");
        }
        return "";
    }


    void XULRunner2::loadOverlay(const std::string & inXULURL)
    {
        Element * targetElement = mRootElement->getElementById(getOverlayElementId(inXULURL));
        if (targetElement)
        {
            XULOverlayParser parser(targetElement);
            ChromeURL url(inXULURL);
            ParseFile(parser, url.convertToLocalPath());
        }
        else
        {
            ReportError("Failed to parse the overlay document.");
        }
    }


    ElementPtr XULRunner2::rootElement() const
    {
        return mRootElement;
    }


    HMODULE XULRunner2::getModuleHandle() const
    {
        return mModuleHandle;
    }


} // namespace XULWin

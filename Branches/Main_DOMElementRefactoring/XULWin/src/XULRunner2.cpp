#include "XULWin/XULRunner2.h"
#include "XULWin/XULRunner.h"
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
        if (XULRunner::GetModuleHandle() == 0)
        {
            throw std::runtime_error("No module handle set for XULRunner2 class. Did you forget to create the XULWin::Initializer object?");
        }
        mModuleHandle = XULRunner::GetModuleHandle();
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


    Poco::XML::Document * XULRunner2::document()
    {
        return mDocument.get();
    }


    ElementPtr XULRunner2::parseFile(AbstractXULParser & ioParser, const std::string & inFilePath)
    {
        ElementPtr result;
        try
        {
            // Somewhat hackish solution to make both XULParser and DOMBuilder content handlers.
            Poco::XML::ContentHandler * myContentHandler = ioParser.getContentHandler();
            Poco::XML::DOMBuilder domBuilder(ioParser); // here the content handler gets overwritten
            Poco::XML::ContentHandler * pocoContentHandler = ioParser.getContentHandler();
            ioParser.setContentHandler(myContentHandler);
            ioParser.setExtraContentHander(pocoContentHandler);
            mDocument = domBuilder.parse(inFilePath);
            result = ioParser.rootElement();
            mDocument->firstChild();
        }
        catch (Poco::Exception & exc)
        {
            ReportError(exc.displayText());
        }
        return result;
    }


    ElementPtr XULRunner2::parseString(AbstractXULParser & ioParser, const std::string & inXULString)
    {
        ElementPtr result;
        try
        {
            // Somewhat hackish solution to make both XULParser and DOMBuilder content handlers.
            Poco::XML::ContentHandler * myContentHandler = ioParser.getContentHandler();
            Poco::XML::DOMBuilder domBuilder(ioParser); // here the content handler gets overwritten
            Poco::XML::ContentHandler * pocoContentHandler = ioParser.getContentHandler();
            ioParser.setContentHandler(myContentHandler);
            ioParser.setExtraContentHander(pocoContentHandler);
            mDocument = domBuilder.parseMemoryNP(inXULString.c_str(), inXULString.size());
            result = ioParser.rootElement();
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
        mRootElement = parseFile(parser, getMainXULFile(Windows::getCurrentDirectory()));
        return mRootElement;
    }


    ElementPtr XULRunner2::loadXULFromFile(const std::string & inXULURL)
    {
        XULParser parser;
        ChromeURL url(inXULURL);
        mRootElement = parseFile(parser, url.convertToLocalPath());
        return mRootElement;
    }


    ElementPtr XULRunner2::loadXULFromString(const std::string & inXULString)
    {
        XULParser parser;
        mRootElement = parseString(parser, inXULString);
        return mRootElement;
    }


    std::string XULRunner2::getOverlayElementId(const std::string & inXULURL)
    {
        XULRunner2 temp;
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
            parseFile(parser, url.convertToLocalPath());
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


} // namespace XULWin

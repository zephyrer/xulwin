#include "XULWin/XULRunner.h"
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


    bool parsePrefsLine(const std::string & inPrefsLine, std::pair<std::string, std::string> & outPref)
    {
        try
        {
            //pref("toolkit.defaultChromeURI", "chrome://myapp/content/myapp.xul");
            std::string::size_type begin = inPrefsLine.find_first_of("\"");
            if (begin == std::string::npos)
            {
                begin = inPrefsLine.find_first_of("'");
            }
            if (begin == std::string::npos)
            {
                return false;
            }
            begin++;

            std::string::size_type end = inPrefsLine.find_first_of("\"", begin);
            if (end == std::string::npos)
            {
                end = inPrefsLine.find_first_of("'", begin);
            }

            outPref.first = inPrefsLine.substr(begin, end - begin);
            end++;

            begin = inPrefsLine.find_first_of("\"", end);
            if (begin == std::string::npos)
            {
                begin = inPrefsLine.find_first_of("'", end);
            }
            begin++;

            end = inPrefsLine.find_first_of("\"", begin);
            if (end == std::string::npos)
            {
                end = inPrefsLine.find_first_of("'", begin);
            }

            outPref.second = inPrefsLine.substr(begin, end - begin);
            end++;
        }
        catch (const std::exception & inExc)
        {
            ReportError(inExc.what());
            return false;
        }
        return true;
    }


    bool getPrefs(const std::string & inPrefsFile, Prefs & outPrefs)
    {
        static char str[1024];
        FILE * fp;
        fp = fopen(inPrefsFile.c_str(), "r");
        if (!fp)
        {
            ReportError("Failed to open prefs file: " + inPrefsFile);
            return false;
        }

        while (fgets(str, sizeof(str), fp) != NULL)
        {
            // strip trailing '\n' if it exists
            int len = strlen(str)-1;
            if (str[len] == '\n')
            {
                str[len] = 0;
            }
            std::pair<std::string, std::string> pair;
            if (parsePrefsLine(str, pair))
            {
                outPrefs.insert(pair);
            }
            else
            {
                ReportError("Could not parse pref: " + std::string(str));
            }
        }
        fclose(fp);
        return true;
    }


    std::string getMainXULFile(const Poco::Path & inTopLevelAppDir)
    {
        static const std::string cPrefsFile = "defaults/preferences/prefs.js";

        std::string appName = inTopLevelAppDir.toString();
        if (inTopLevelAppDir.depth() > 0)
        {
            appName = inTopLevelAppDir.directory(inTopLevelAppDir.depth() - 1);
        }

        Prefs prefs;
        if (!getPrefs(cPrefsFile, prefs))
        {
            throw std::exception("Could not parse prefs file. This implicates that the main XUL file path remains unknown and that the application can't be started.");
        }

        Prefs::iterator it = prefs.find("toolkit.defaultChromeURI");
        if (it != prefs.end())
        {
            // "chrome://myapp/content/myapp.xul"
            ChromeURL url(it->second);
            std::string xulFile = url.convertToLocalPath();
            return xulFile;
        }
        return "";
    }


    Fallible<std::string> XULRunner::sLocale;
    HMODULE XULRunner::sModuleHandle(0);


    XULRunner::XULRunner()
    {
        if (XULRunner::GetModuleHandle() == 0)
        {
            throw std::runtime_error("No module handle set for XULRunner class. Did you forget to create the XULWin::Initializer object?");
        }
        mModuleHandle = XULRunner::GetModuleHandle();
    }


    std::string XULRunner::GetLocale()
    {
        return sLocale.or(Defaults::defaultLocale());
    }
    
    
    void XULRunner::SetLocale(const std::string & inLocale)
    {
        sLocale = inLocale;
    }
    

    HMODULE XULRunner::GetModuleHandle()
    {
        return sModuleHandle;
    }

    
    void XULRunner::SetModuleHandle(HMODULE inModuleHandle)
    {
        sModuleHandle = inModuleHandle;
    }


    Poco::XML::Document * XULRunner::document()
    {
        return mDocument.get();
    }


    ElementPtr XULRunner::parseFile(AbstractXULParser & ioParser, const std::string & inFilePath)
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


    ElementPtr XULRunner::parseString(AbstractXULParser & ioParser, const std::string & inXULString)
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


    ElementPtr XULRunner::loadApplication(const std::string & inApplicationIniFile)
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


    ElementPtr XULRunner::loadXULFromFile(const std::string & inXULURL)
    {
        XULParser parser;
        ChromeURL url(inXULURL);
        mRootElement = parseFile(parser, url.convertToLocalPath());
        return mRootElement;
    }


    ElementPtr XULRunner::loadXULFromString(const std::string & inXULString)
    {
        XULParser parser;
        mRootElement = parseString(parser, inXULString);
        return mRootElement;
    }


    std::string XULRunner::getOverlayElementId(const std::string & inXULURL)
    {
        XULRunner temp;
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


    void XULRunner::loadOverlay(const std::string & inXULURL)
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


    ElementPtr XULRunner::rootElement() const
    {
        return mRootElement;
    }


} // namespace XULWin

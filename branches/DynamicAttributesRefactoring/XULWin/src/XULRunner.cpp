#include "XULWin/XULRunner.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/XULOverlayParser.h"
#include "XULWin/WinUtils.h"
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
        FILE *fp;
        fp = fopen(inPrefsFile.c_str(), "r");
        if(!fp)
        {
            ReportError("Failed to open prefs file: " + inPrefsFile);
            return false;
        }
        
        while (fgets(str, sizeof(str), fp) != NULL)
        {
            // strip trailing '\n' if it exists
            int len = strlen(str)-1;
            if(str[len] == '\n')
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
        std::string appName = inTopLevelAppDir.directory(inTopLevelAppDir.depth() - 1);

        Prefs prefs;
        if (!getPrefs(cPrefsFile, prefs))
        {
            ReportError("Could not parse prefs file.");
            return "";
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


    XULRunner::XULRunner(HMODULE inModuleHandle) :
        mModuleHandle(inModuleHandle)
    {
    }


    std::string XULRunner::GetLocale()
    {
        return sLocale.or(Defaults::defaultLocale());
    }


    void XULRunner::SetLocale(const std::string & inLocale)
    {
        sLocale = inLocale;
    }


    void XULRunner::run(const std::string & inApplicationIniFile)
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


    ElementPtr XULRunner::Parse(AbstractXULParser & inParser, const std::string & inXULURL)
    {
        ElementPtr result;
        try
        {
            inParser.parse(inXULURL);
            result = inParser.rootElement();
        }
        catch (Poco::Exception & exc)
        {
            ReportError(exc.displayText());
        }
        return result;
    }

    
    ElementPtr XULRunner::loadApplication(const std::string & inApplicationIniFile)
    {
        XULParser parser;
        mRootElement = Parse(parser, getMainXULFile(Windows::getCurrentDirectory()));
        return mRootElement;
    }

    
    ElementPtr XULRunner::loadXUL(const std::string & inXULURL)
    {
        XULParser parser;
        ChromeURL url(inXULURL);
        mRootElement = Parse(parser, url.convertToLocalPath());
        return mRootElement;
    }


    std::string XULRunner::getOverlayElementId(const std::string & inXULURL)
    {
        XULRunner temp(::GetModuleHandle(0));
        temp.loadXUL(inXULURL);
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
            Parse(parser, url.convertToLocalPath());
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
    
    
    HMODULE XULRunner::getModuleHandle() const
    {
        return mModuleHandle;
    }


} // namespace XULWin
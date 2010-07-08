#include "XULWin/XULRunner.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/XULOverlayParser.h"
#include "XULWin/XMLWindow.h"
#include "XULWin/WinUtils.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include <boost/noncopyable.hpp>


namespace XULWin
{

    typedef std::map<std::string, std::string> Prefs;


    void parsePrefsLine(const std::string & inPrefsLine, std::pair<std::string, std::string> & outPref)
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
                throw std::runtime_error("Couldn't not parse this preference line: " + inPrefsLine);
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
            throw std::runtime_error(std::string("Failed to parse the prefs file. Error: ") + inExc.what());
        }
    }

    class FileHelper : boost::noncopyable
    {
    public:
        FileHelper(const std::string & inFileName) :
            mFile(fopen(inFileName.c_str(), "r"))
        {
        }

        ~FileHelper()
        {
            if (mFile)
            {
                fclose(mFile);
                mFile = 0;
            }
        }

        FILE * get() { return mFile; }

    private:
        FILE * mFile;
    };


    void getPrefs(const std::string & inPrefsFile, Prefs & outPrefs)
    {
        FileHelper scopedFile(inPrefsFile);
        if (!scopedFile.get())
        {
            throw std::runtime_error("Failed to open prefs file: " + inPrefsFile);
        }

        std::vector<char> str(1024, 0);
        while (fgets(&str[0], str.size(), scopedFile.get()) != NULL)
        {
            // strip trailing '\n' if it exists
            int len = strnlen(&str[0], str.size());
            if (str[len - 1] == '\n')
            {
                str[len - 1] = 0;
            }
            std::pair<std::string, std::string> pair;
            parsePrefsLine(&str[0], pair);
            outPrefs.insert(pair);
        }
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
        getPrefs(cPrefsFile, prefs);

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
        if (sModuleHandle == 0)
        {
            throw std::runtime_error("No module handle set for XULRunner class. Did you forget to create the XULWin::Initializer object?");
        }
        mModuleHandle = GetModuleHandle();
    }


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


    HMODULE XULRunner::GetModuleHandle()
    {
        return sModuleHandle;
    }


    void XULRunner::SetModuleHandle(HMODULE inModuleHandle)
    {
        sModuleHandle = inModuleHandle;
    }


    void XULRunner::run(const std::string & inApplicationIniFile)
    {
        XULParser parser;
        Poco::Path topLevelAppDir = WinAPI::getCurrentDirectory();
        std::string mainXULFile = getMainXULFile(topLevelAppDir);
        parser.parse(mainXULFile);
        if (XMLWindow * window = parser.rootElement()->downcast<XMLWindow>())
        {
            window->showModal(WindowPos_CenterInScreen);
        }
    }


    ElementPtr XULRunner::ParseFile(AbstractXULParser & inParser, const std::string & inXULURL)
    {
        inParser.parse(inXULURL);
        return inParser.rootElement();
    }


    ElementPtr XULRunner::ParseString(AbstractXULParser & inParser, const std::string & inXULString)
    {
        inParser.parseString(inXULString);
        return inParser.rootElement();
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
        mRootElement = ParseFile(parser, getMainXULFile(WinAPI::getCurrentDirectory()));
        return mRootElement;
    }


    ElementPtr XULRunner::loadXULFromFile(const std::string & inXULURL)
    {
        XULParser parser;
        ChromeURL url(inXULURL);
        mRootElement = ParseFile(parser, url.convertToLocalPath());
        return mRootElement;
    }


    ElementPtr XULRunner::loadXULFromString(const std::string & inXULString)
    {
        XULParser parser;
        mRootElement = ParseString(parser, inXULString);
        return mRootElement;
    }


    std::string XULRunner::getOverlayElementId(const std::string & inXULURL)
    {
        XULRunner temp(::GetModuleHandle(0));
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
            ParseFile(parser, url.convertToLocalPath());
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

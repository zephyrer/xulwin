#include "XULWin/XULRunner.h"
#include "XULWin/XULRunner.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Defaults.h"
#include "XULWin/ElementUtilities.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/XML/XMLStream.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include <sstream>


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


    Poco::XML::Document * XULRunner::ParseFile(const std::string & inFilePath)
    {
        Poco::XML::Document * result(0);
        Poco::XML::InputSource src(inFilePath);
	    try
	    {
		    Poco::XML::DOMParser parser;
		    result = parser.parse(&src);
        }
        catch (const Poco::Exception & exc)
	    {
            ReportError(exc.displayText());
	    }
        return result;
    }


    Poco::XML::Document * XULRunner::ParseString(const std::string & inXULString)
    {
        Poco::XML::Document * result(0);
        std::istringstream inputStream(inXULString);
        Poco::XML::InputSource src(inputStream);
	    try
	    {
		    Poco::XML::DOMParser parser;
		    result = parser.parse(&src);
	    }
        catch (const Poco::Exception & exc)
	    {
            ReportError(exc.displayText());
	    }
        return result;
    }


    Poco::XML::Document * XULRunner::loadApplication(const std::string & inApplicationIniFile)
    {
        Poco::File file(inApplicationIniFile);
        if (!file.exists())
        {
            std::string msg = "The current working directory does not contain a file named ";
            msg += "'" + inApplicationIniFile + "'.";
            msg += " Please set the current directory to the one that contains this file.";
            throw std::runtime_error(msg.c_str());
        }
        mDocument.assign(ParseFile(getMainXULFile(Windows::getCurrentDirectory())));
        mRootComponent = CreateRootComponent(mDocument->documentElement());
        return mDocument.get();
    }


    Poco::XML::Document * XULRunner::loadXULFromFile(const std::string & inXULURL)
    {
        ChromeURL url(inXULURL);
        mDocument.assign(ParseFile(url.convertToLocalPath()));
        mRootComponent = CreateRootComponent(mDocument->documentElement());
        return mDocument.get();
    }


    Poco::XML::Document * XULRunner::loadXULFromString(const std::string & inXULString)
    {
        mDocument.assign(ParseString(inXULString));
        mRootComponent = CreateRootComponent(mDocument->documentElement());
        return mDocument.get();
    }


    std::string XULRunner::getOverlayElementId(const std::string & inXULURL)
    {
        //XULRunner temp;
        //temp.loadXULFromFile(inXULURL);
        //if (!temp.() || temp.()->childNodes()->length() == 0)
        //{
        //    return "";
        //}
        //Poco::XML::Node * child = temp.()->childNodes()->item(0);
        //if (child)
        //{
        //    return child->getAttribute("id");
        //}
        return "";
    }


    void XULRunner::loadOverlay(const std::string & inXULURL)
    {
        assert(!"Not yet implemented");
        //Element *  = ->getElementById(getOverlayElementId(inXULURL));
        //if ()
        //{
        //    XULOverlayParser parser();
        //    ChromeURL url(inXULURL);
        //    ParseFile(url.convertToLocalPath());
        //}
        //else
        //{
        //    ReportError("Failed to parse the overlay document.");
        //}
    }


    Poco::XML::Element * XULRunner::rootElement() const
    {
        return mDocument->documentElement();
    }
    
    
    ComponentPtr XULRunner::CreateRootComponent(Poco::XML::Element * inElement)
    {
        ComponentPtr result = CreateComponent(0, inElement);
        if (!result)
        {
            throw std::runtime_error("Failed to create a native component from the root component.");
        }
        return result;
    }


    ComponentPtr XULRunner::CreateComponent(Component * inParent, Poco::XML::Element * inElement)
    {
        ComponentPtr result;
        result = ComponentFactory::Instance().create(inParent, inElement);
        if (!result)
        {
            return result;
        }

        // Create child components        
        Poco::XML::NodeList * nodeList = inElement->childNodes();
        if (nodeList->length() > 0)
        {
            Poco::XML::Node * node = nodeList->item(0);
            while (node)
            {
                Poco::XML::Element * element = dynamic_cast<Poco::XML::Element *>(node);
                if (element)
                {
                    ComponentPtr comp = CreateComponent(result.get(), element);
                    if (comp)
                    {
                        result->addChild(comp);
                    }
                }
                node = node->nextSibling();
            }
        }
        return result;
    }


} // namespace XULWin

#include "DesktopOrganizer.h"
#include "ForeignWindow.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/String.h"
#include "Poco/StringTokenizer.h"
#include <fstream>
#include <sstream>


namespace XULWin
{

    using namespace WinAPI;


    WindowEnumerator::WindowEnumerator()
    {
    }

    
    std::vector<HWND> WindowEnumerator::getDesktopWindows() const
    {
        mFoundWindows.clear();
        ::EnumChildWindows(::GetDesktopWindow(), &WindowEnumerator::OnFoundWindow, (LPARAM)this);
        return mFoundWindows;
    }


    void WindowEnumerator::onFoundWindow(HWND inHWND)
    {
        // The Window must have a caption (title bar) or a sysmenu.
        if (!Window_HasStyle(inHWND, WS_CAPTION) &&
            !Window_HasStyle(inHWND, WS_SYSMENU))
        {
            return;
        }

        // Only consider visible Windows.
        if (!Window_IsVisible(inHWND))
        {
            return;
        }

        mFoundWindows.push_back(inHWND);
    }


    BOOL CALLBACK WindowEnumerator::OnFoundWindow(HWND inHWND, LPARAM lParam)
	{
        WindowEnumerator * windowEnumerator = reinterpret_cast<WindowEnumerator*>(lParam);
        windowEnumerator->onFoundWindow(inHWND);
		return true;
	}

    
    DesktopOrganizer::DesktopOrganizer()
    {
    }


    static std::string EncodeWindowPlacement(const WINDOWPLACEMENT & inWindowPlacement)
    {
        std::stringstream ss;
        {
            Poco::Base64Encoder encoder(ss);
            const char * offset = reinterpret_cast<const char*>(&inWindowPlacement);
            std::vector<char> buffer(offset, offset + sizeof(inWindowPlacement));
            for (size_t idx = 0; idx != buffer.size(); ++idx)
            {
                encoder << buffer[idx];
            }
            encoder.close();
        }
        return ss.str();
    }


    static WINDOWPLACEMENT DecodeWindowPlacement(const std::string & inEncoded)
    {
        std::string decodedString;
        {
            std::istringstream istr(inEncoded);
            Poco::Base64Decoder decoder(istr);
            decoder >> decodedString;
            assert(decoder.eof());
            if (decoder.fail())
            {
                throw std::runtime_error("Failed to parse Window placement data from the configuration file.");
            }
        }

        if (decodedString.size() != sizeof(WINDOWPLACEMENT))
        {
            throw std::runtime_error("Configuration file seems to contain invalid data.");
        }
        
        WINDOWPLACEMENT windowPlacement;
        memcpy(&windowPlacement, &decodedString[0], decodedString.size());
        return windowPlacement;
    }


    static void TestWindowPlacementSerialization(HWND hWnd)
    {        
        WINDOWPLACEMENT windowPlacement;
        ::ZeroMemory(&windowPlacement, sizeof(windowPlacement));
        windowPlacement.length = sizeof(WINDOWPLACEMENT);
        ::GetWindowPlacement(hWnd, &windowPlacement);

        std::string encoded = EncodeWindowPlacement(windowPlacement);
        WINDOWPLACEMENT decodedWindowPlacement = DecodeWindowPlacement(encoded);

        assert(!memcmp(&windowPlacement, &decodedWindowPlacement, sizeof(WINDOWPLACEMENT)));
        assert(EncodeWindowPlacement(decodedWindowPlacement) == encoded);
    }


    static std::string EncodeRect(const Rect & inRect)
    {
        std::stringstream ss;
        ss << inRect.x() << ", " << inRect.y() << ", " << inRect.width() << ", " << inRect.height();
        return ss.str();
    }


    static Rect DecodeRect(const std::string & inEncodedRect)
    {
        std::vector<int> values;
        Poco::StringTokenizer tokenizer(inEncodedRect, ",", Poco::StringTokenizer::TOK_TRIM);
        Poco::StringTokenizer::Iterator it = tokenizer.begin(), end = tokenizer.end();
        for (; it != end; ++it)
        {
            int value(-1);
            std::istringstream iss(*it);
            iss >> value;
            if (value == -1)
            {
                throw std::runtime_error("Error while parsing Rect");
            }
            values.push_back(value);
        }
        return Rect(values[0], values[1], values[2], values[3]);
    }


    void DesktopOrganizer::saveDesktop(const std::string & inConfigFile)
    {
        WindowEnumerator enumerator;
        std::vector<HWND> desktopWindows = enumerator.getDesktopWindows();
        std::ofstream outFile(inConfigFile.c_str());
        size_t numWindows = desktopWindows.size();
        for (size_t idx = 0; idx != numWindows; ++idx)
        {
            HWND & hWnd(desktopWindows[idx]);

            // Test Window Placement serialization
            TestWindowPlacementSerialization(hWnd);
            
            // Classname
            outFile << Window_GetClassName(hWnd) << std::endl;

            // Title
            outFile << Window_GetText(hWnd) << std::endl;

            // Window Rect
            Rect rect = Window_GetWindowRect(hWnd);
            outFile << EncodeRect(rect) << std::endl;

            // Window placement
            WINDOWPLACEMENT windowPlacement;
            windowPlacement.length = 0;
            ::GetWindowPlacement(hWnd, &windowPlacement);
            outFile << EncodeWindowPlacement(windowPlacement);
            outFile << std::endl;
        }
    }


    static std::vector<std::string> GetLines(const std::string & inText)
    {
        std::vector<std::string> result;
        Poco::StringTokenizer tokenizer(inText, "\n");
        Poco::StringTokenizer::Iterator it = tokenizer.begin(), end = tokenizer.end();
        for (; it != end; ++it)
        {
            result.push_back(*it);
        }
        return result;
    }


    static std::string ReadFile(const std::string & inPath)
    {
        std::string result;
        std::ifstream t(inPath.c_str());

        t.seekg(0, std::ios::end);   
        result.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        result.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        return result;
    }


    void DesktopOrganizer::loadDesktop(const std::string & inConfigFile)
    {
        std::vector<ForeignWindowInfo> parsedInfos;
        std::vector<std::string> lines(GetLines(ReadFile(inConfigFile)));
        size_t numLines = lines.size();
        assert((lines.size() % 4) == 0);

        std::vector<std::string>::iterator it = lines.begin();
        while (it != lines.end())
        {
            std::string className(*it++);
            std::string title(*it++);
            Rect decodedRect(DecodeRect(*it++));
            WINDOWPLACEMENT windowPlacement(DecodeWindowPlacement(*it++));
            parsedInfos.push_back(ForeignWindowInfo(className, title, decodedRect, windowPlacement));
        }


        WindowEnumerator windowEnumerator;
        std::vector<HWND> desktopWindows = windowEnumerator.getDesktopWindows();
        for (size_t idx = 0; idx != desktopWindows.size(); ++idx)
        {
            HWND hwnd = desktopWindows[idx];
            ForeignWindowInfo hwndInfo(hwnd);
            for (size_t idx = 0; idx != parsedInfos.size(); ++idx)
            {
                ForeignWindowInfo & parsedInfo(parsedInfos[idx]);
                if (parsedInfo.className() == hwndInfo.className() && parsedInfo.title() == hwndInfo.title())
                {
                    const Rect & rect = parsedInfo.rect();
                    ::SetWindowPlacement(hwnd, &parsedInfo.windowPlacement());
                    ::MoveWindow(hwnd, rect.x(), rect.y(), rect.width(), rect.height(), TRUE);
                    ::InvalidateRect(hwnd, NULL, TRUE);
                    //::RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
                }
            }
        }
    }

} // namespace XULWin

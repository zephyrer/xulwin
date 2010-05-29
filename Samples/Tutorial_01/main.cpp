#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "XULWin/XMLWindow.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include <sstream>
#include <string>


/**
 * Remove single or double quotes around a string.
 * Windows wraps it's arguments in quotes and we don't want that.
 */
std::string unquote(const std::string & inString)
{
    if (inString.size() <= 2)
    {
        return inString;
    }

    if ((inString[0] == '"' && inString[inString.size() - 1] == '"') ||
            (inString[0] == '\'' && inString[inString.size() - 1] == '\''))
    {
        return inString.substr(1, inString.size() - 2);
    }
    return inString;
}


/**
 * Tutorial_01 illustrates a simple XULWin application.
 *
 * Usage: Tutorial_01.exe XULApplicationPath
 *
 *  Checklist:
 *  1. Set code generation settings:
 *      - "Multi-threaded Debug" (/MTd) for Debug builds
 *      - "Multi-threaded" (/MT) for Release builds
 *  2. Link with: comctl32.lib, GdiPlus.lib.
 *  3. The project must have a manifest file.
 *      - See "Tutorial_01.exe.manifest" for example.
 *  4. Set the debugging settings correctly
 *      Project settings -> Debugging -> Command Arguments: $(ProjectDir)HelloWorld
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize the XULWin library
    XULWin::Initializer initializer(hInstance);

    // Obtain the path to the directory containing your XUL application.ini file.
    // In this example the path will be passed as a command line argument.
    // See Project settings -> Debugging -> Command Arguments: $(ProjectDir)HelloWorld
    std::string commandLine(unquote(lpCmdLine));
    if (commandLine.empty())
    {
        ::MessageBox(0, TEXT("Please set the command arguments correctly:\nTutorial Project settings -> Debugging -> Command Arguments: $(ProjectDir)HelloWorld"), 0, MB_OK);
        return 1;
    }

    // To make things a bit easier, we change the current directory.
    // This allows us to use relative paths for our application.
    XULWin::Windows::CurrentDirectoryChanger cd(commandLine);

    // Now that we have the path to the app, let's try to run it.
    XULWin::XULRunner xulRunner(hInstance);

    // We want to anticipate potential errors occuring during
    // the parsing of the XUL file. A local ErrorCatcher object
    // collects any reported errors during its lifetime. See the
    // XULWin/ErrorReporter.h file for more info.
    XULWin::ErrorCatcher errorCatcher;

    // We pass the "application.ini" file as argument to XULRunner.
    // XUL applications should always have an application.ini file,
    // see the Mozilla XULRunner documentation for more about the
    // application directory structure.
    // The parsing of the XUL file also happens here.
    xulRunner.loadApplication("application.ini");

    // If the main XUL file was successfully found and parsed we
    // should now get a valid root element.
    XULWin::ElementPtr root = xulRunner.rootElement();

    // Check if the root element is not NULL
    if (!root)
    {
        // Let's see if our ErrorCatcher has caught any errors.
        if (errorCatcher.hasCaught())
        {
            std::stringstream ss;
            errorCatcher.getErrorMessage(ss);

            // Since we are compiling with Unicode set on, we must convert
            // the message to UTF16.
            std::wstring utf16Message = XULWin::ToUTF16(ss.str());

            // Report the error using a message box.
            ::MessageBox(0, utf16Message.c_str(), 0, MB_OK);

            // And quit the application.
            return 1;
        }
        // The ErrorCatcher didn't catch any errors for some mysterious reason.
        // Report a generic and unhelpful errormessage, and then quit :(
        // This should not happen.
        else
        {
            ::MessageBox(0, TEXT("XUL runner failed to parse the XUL file for an unknown reason."), 0, MB_OK);
        }
        return 1;
    }

    // If we get here then things are going well.
    // We know that the root element of 'HelloWorld' is a 'window' element.
    // Therefore we obtain the root's Component object, and cast it to a Window component.
    // For more information about the Element and Component classes see their corresponding
    // header files.
    if (XULWin::Window * window = root->component()->downcast<XULWin::Window>())
    {
        // If we get here, then the downcast succeeded (it really is a window)
        // Let's display the window now.

        // We use the 'showModal' method because it halts the program flow until the window
        // is closed. (Or more specifically, it starts a message loop.)
        // The 'show' method on the other hand does not halt program flow. If we used it here
        // then the Window would only be visible for a fraction of a second, and then our
        // application would quit because the end of the WinMain function has been reached.

        // We pass the 'CenterInScreen' parameter to have the window centered in the screen.
        // You could also use the DefaultPosition parameter and define the 'screenX', 'screenY',
        // 'width' and 'height' attributes of the window element in the XUL file.
        window->showModal(XULWin::WindowPos_CenterInScreen);
    }

    // This is the end of our very quick XULWin tutorial.
    //
    // See Tutorial 02 for:
    // - A *real* GUI example
    // - Event Handling
    // - Multilingual support by putting the translations in separate DTD documents.
    //
    // You may also want to have a look at the Test project.
    //
    return 0;
}

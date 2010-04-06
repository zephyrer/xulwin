#ifndef JSPROMPT_H_INCLUDED
#define JSPROMPT_H_INCLUDED


#include "XULWin/Js/V8Includes.h"
#include "XULWin/Element.h"
#include "XULWin/Window.h"
#include "XULWin/EventListener.h"
#include "XULWin/XULRunner.h"


namespace XULWin
{

namespace Js
{

    class JsPrompt
    {
    public:
        static char * GetPromptXUL()
        {
            return
                "<?xml version=\"1.0\"?>																\n"
                "<?xml-stylesheet href=\"chrome://global/skin/\" type=\"text/css\"?>					\n"
                "<dialog xmlns=\"http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul\"		\n"
                "        xmlns:svg=\"http://www.w3.org/2000/svg\"										\n"
                "        orient=\"vertical\"															\n"
                "        align=\"stretch\"																\n"
                "        title=\"JavaScript Prompt\">													\n"
                "    <vbox>																				\n"
                "      <hbox>																			\n"
                "        <vbox>																			\n"
                "          <spacer flex=\"1\"></spacer>													\n"
                "          <label value=\"{{Text}}:\" id=\"textLabel\"></label>							\n"
                "          <spacer flex=\"1\"></spacer>													\n"
                "        </vbox>																		\n"
                "        <textbox width=\"120\" id=\"textInput\" value=\"{{Value}}\"></textbox>			\n"
                "      </hbox>																			\n"
                "      <hbox>																			\n"
                "        <spacer flex=\"1\"></spacer>													\n"
                "        <button label=\"OK\" id=\"okButton\" />                                        \n"
                "        <button label=\"Cancel\" id=\"cancelButton\" />                                \n"
                "    </hbox>																			\n"
                "  </vbox>																				\n"
                "</dialog>																				\n";
        }

        /**
         * JsPrompt Constructor
         *
         * @param inParentWindow    The parent window. May be nil, but makes a non-modal dialog.
         */
        JsPrompt(Window * inParentWindow);
        
        /**
         * show
         *
         * @param inText    Text shown before the input field.
         * @param inValue   The default value in the textbox.
         */
        Fallible<std::string> show(const std::string & inText, const std::string & inValue);

    private:
        void setTextValues(const std::string & inText, const std::string & inValue);

        void registerEventListeners();

        LRESULT onOkButtonPressed(WPARAM wParam, LPARAM lParam);

        LRESULT onCancelButtonPressed(WPARAM wParam, LPARAM lParam);

        XULRunner mXULRunner;
        Poco::XML::Element * mTextField;
        Dialog * mDialog;
        Window * mParentWindow;
        ScopedEventListener mEvents;
    };


} // namespace Js

} // namespace XULWin


#endif // JSPROMPT_H_INCLUDED

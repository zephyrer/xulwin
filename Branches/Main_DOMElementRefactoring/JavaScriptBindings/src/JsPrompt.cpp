#include "XULWin/Js/JsPrompt.h"
#include "XULWin/ComponentManager.h"
#include <boost/bind.hpp>


using namespace v8;


namespace XULWin
{

namespace Js
{

    JsPrompt::JsPrompt(Window * inParentWindow) :
        mParentWindow(inParentWindow),
        mDialog(0),
        mTextField(0)
    {
    }


    Fallible<std::string> JsPrompt::show(const std::string & inText, const std::string & inValue)
    {
        Fallible<std::string> result;
        mXULRunner.loadXULFromString(JsPrompt::GetPromptXUL());
        mDialog = GetComponent<Dialog>(mXULRunner.rootElement());
        if (!mDialog)
        {
            throw std::runtime_error("Prompt root element is not of type 'dialog'.");
        }

        setTextValues(inText, inValue);
        registerEventListeners();

        DialogResult dlgResult = mDialog->showModal(mParentWindow);
        if (dlgResult == DialogResult_Ok)
        {
            result = mTextField->getAttribute("value");
        }
        return result;
    }


    void JsPrompt::setTextValues(const std::string & inText, const std::string & inValue)
    {
        if (!mDialog)
        {
            throw std::logic_error("Dialog has not yet been initialized.");
        }

        Poco::XML::Element * textLabel = mXULRunner.document()->getElementById("textLabel");
        if (!textLabel)
        {
            throw std::runtime_error("Promt dialog does not have an element with id 'textLabel'");
        }
        textLabel->setAttribute("value", inText);
        
        mTextField = mXULRunner.document()->getElementById("textInput");
        if (!mTextField)
        {
            throw std::runtime_error("Promt dialog does not have an element with id 'textInput'.");
        }
        mTextField->setAttribute("value", inValue);
    }


    void JsPrompt::registerEventListeners()
    {        
        Poco::XML::Element * okButton = mXULRunner.document()->getElementById("okButton");
        if (!okButton)
        {
            throw std::runtime_error("Prompt dialog does not contain an element with id 'okButton'.");
        }

        Poco::XML::Element * cancelButton = mXULRunner.document()->getElementById("cancelButton");
        if (!cancelButton)
        {
            throw std::runtime_error("Prompt dialog does not contain an element with id 'cancelButton'.");
        }

        mEvents.connect(GetComponent<Component>(okButton), boost::bind(&JsPrompt::onOkButtonPressed, this, _1, _2));
        mEvents.connect(GetComponent<Component>(cancelButton), boost::bind(&JsPrompt::onCancelButtonPressed, this, _1, _2));
    }


    LRESULT JsPrompt::onOkButtonPressed(WPARAM wParam, LPARAM lParam)
    {
        if (!mDialog)
        {
            throw std::logic_error("Dialog is not initialized.");
        }
        mDialog->endModal(DialogResult_Ok);
        return XULWin::EventResult_Handled;
    }

    
    LRESULT JsPrompt::onCancelButtonPressed(WPARAM wParam, LPARAM lParam)
    {
        if (!mDialog)
        {
            throw std::logic_error("Dialog is not initialized.");
        }
        mDialog->endModal(DialogResult_Cancel);
        return XULWin::EventResult_Handled;
    }

} // namespace Js

} // namespace XULWin

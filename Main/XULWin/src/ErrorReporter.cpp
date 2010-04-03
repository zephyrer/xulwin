#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include <boost/bind.hpp>
#include <sstream>
#include <assert.h>

#define MESSAGEBOXLOGGING 1

#if MESSAGEBOXLOGGING
#include <windows.h>
#endif


namespace XULWin
{


    ErrorReporter * ErrorReporter::sInstance = 0;


    Error::Error() :
        mErrorCode(Error::SUCCEEDED)
    {
    }


    Error::Error(int inErrorCode) :
        mErrorCode(inErrorCode)
    {
    }


    Error::Error(const std::string & inErrorMessage) :
        mErrorCode(Error::FAILED),
        mErrorMessage(inErrorMessage)
    {
    }


    Error::Error(int inErrorCode, const std::string & inErrorMessage) :
        mErrorCode(inErrorCode),
        mErrorMessage(inErrorMessage)
    {
    }


    int Error::code() const
    {
        return mErrorCode;
    }


    const std::string & Error::message() const
    {
        return mErrorMessage;
    }


    ErrorCatcher::ErrorCatcher() :
        mPropagate(false),
        mOwns(true), // the original object, created on the stack, must do the cleanup
        mDisableLogging(false)
    {
        ErrorReporter::Instance().push(this);
    }


    ErrorCatcher::~ErrorCatcher()
    {
        if (mOwns)
        {
            ErrorReporter::Instance().pop(this);
            if (mPropagate && !ErrorReporter::Instance().mStack.empty())
            {
                ErrorReporter::Instance().mStack.top()->setChild(this);
            }
            else
            {
                if (!mDisableLogging)
                {
                    log();
                }
            }
        }
    }


    ErrorCatcher::ErrorCatcher(const ErrorCatcher & rhs) :
        mOwns(false), // copy is not responsible for cleanup
        mErrors(rhs.mErrors),
        mChild(rhs.mChild),
        mPropagate(rhs.mPropagate)
    {
    }


    void ErrorCatcher::disableLogging(bool inDisableLogging)
    {
        mDisableLogging = inDisableLogging;
    }


    bool ErrorCatcher::hasCaught() const
    {
        return mChild || !mErrors.empty();
    }


    void ErrorCatcher::log()
    {
        ErrorReporter::Instance().log(this);
    }


    void ErrorCatcher::propagate()
    {
        mPropagate = true;
    }


    void ErrorCatcher::getErrorMessage(std::stringstream & ss) const
    {
        for (size_t idx = 0; idx != errors().size(); ++idx)
        {
            const Error & error = errors()[idx];
            if (idx > 0)
            {
                ss << " Followed by: ";
            }
            ss << error.message();

            // show the error code, unless if it is the default
            if (error.code() != Error::FAILED)
            {
                ss << " (Code: " << error.code() << ")";
            }
            ss << "\n";
        }
        if (child())
        {
            getErrorMessage(ss);
        }
    }


    void ErrorCatcher::push(const Error & inError)
    {
        mErrors.push_back(inError);
    }


    void ErrorCatcher::setChild(const ErrorCatcher * inErrorCatcher)
    {
        mChild.reset(new ErrorCatcher(*inErrorCatcher));
    }


    void ErrorReporter::Initialize()
    {
        assert(!sInstance);
        if (!sInstance)
        {
            sInstance = new ErrorReporter();
        }
    }


    ErrorReporter & ErrorReporter::Instance()
    {
        //assert(sInstance);
        if (!sInstance)
        {
            Initialize();
        }
        return *sInstance;
    }


    void ErrorReporter::Finalize()
    {
        assert(sInstance);
        if (sInstance)
        {
            delete sInstance;
            sInstance = 0;
        }
    }


    ErrorReporter::ErrorReporter()
    {
    }


    ErrorReporter::~ErrorReporter()
    {
        assert(mStack.empty());
    }


    void ErrorReporter::setLogger(const LogFunction & inLogFunction)
    {
        mLogFunction = inLogFunction;
    }


    void ErrorReporter::reportError(const Error & inError)
    {
        if (!mStack.empty())
        {
            mStack.top()->push(inError);
        }
        else
        {
            log(inError.message());
        }
    }


    void ErrorReporter::push(ErrorCatcher * inError)
    {
        mStack.push(inError);
    }


    void ErrorReporter::pop(ErrorCatcher * inError)
    {
        assert(!mStack.empty());
        if (!mStack.empty())
        {
            bool foundOnTop = mStack.top() == inError;
            assert(foundOnTop);
            if (foundOnTop)
            {
                mStack.pop();
            }
        }
    }


    void ErrorReporter::log(ErrorCatcher * inErrorCatcher)
    {
        std::stringstream ss;
        inErrorCatcher->getErrorMessage(ss);
        log(ss.str());
    }


    void ErrorReporter::log(const std::string & inErrorMessage)
    {
        if (!inErrorMessage.empty())
        {
            if (mLogFunction)
            {
                mLogFunction(inErrorMessage);
            }
#if MESSAGEBOXLOGGING
            else
            {
                std::wstring utf16Message = XULWin::ToUTF16(inErrorMessage);
                ::MessageBox(0, utf16Message.c_str(), 0, MB_OK);
            }
#endif
        }
    }


    void ReportError(int inErrorCode, const std::string & inErrorMessage)
    {
        ErrorReporter::Instance().reportError(Error(inErrorCode, inErrorMessage));
    }


    void ReportError(const std::string & inErrorMessage)
    {
        ErrorReporter::Instance().reportError(Error(inErrorMessage));
    }


    void ReportError(int inErrorCode)
    {
        ErrorReporter::Instance().reportError(Error(inErrorCode));
    }


    bool TryCatch(const TryAction & inTryAction, const CatchAction & inCatchAction)
    {
        try
        {
            inTryAction();
            return true;
        }
        catch (const std::exception & inExc)
        {
            inCatchAction(inExc);
        }
        return false;
    }


    bool TryOrReportError(const TryAction & inAction)
    {
        struct Helper
        {
            static void ReportError_(const std::exception & inException)
            {
                XULWin::ReportError(inException.what());
            }
        };
        return TryCatch(inAction, boost::bind(&Helper::ReportError_, _1));
    }


} // namespace XULWin

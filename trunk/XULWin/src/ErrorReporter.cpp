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


    Error::Error(const std::string & inError) :
        mError(inError),
        mLine(0)
    {
    }


    Error::Error(const std::string & inError, const std::string & inFile, int inLine) :
        mError(inError),
        mFile(inFile),
        mLine(inLine)
    {
    }


    const std::string & Error::message() const
    {
        return mError;
    }


    const std::string & Error::file() const
    {
        return mFile;
    }


    int Error::line() const
    {
        return mLine;
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
        if (hasCaught())
        {
            if (mPropagate)
            {
                if (!ErrorReporter::Instance().mStack.empty()) // safety check
                {
                    throw std::logic_error("The error stack is empty. This should never happen.");
                }
                ErrorReporter::Instance().mStack.top()->setChild(this);
            }
            else if (!mDisableLogging)
            {
                // Fall back to the default logger.
                log();
            }
        }

        if (mOwns)
        {
            ErrorReporter::Instance().pop(this);
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


    void ErrorCatcher::getError(std::stringstream & ss) const
    {
        ss << "The following errors have occured:\n"; 
        for (size_t idx = 0; idx != errors().size(); ++idx)
        {
            const Error & error = errors()[idx];
            if (!error.file().empty())
            {
                ss << error.file() << ":" << error.line() << " ";
            }
            ss << error.message() << "\n";
        }
        if (child())
        {
            getError(ss);
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


    ErrorReporter::ErrorReporter() :
        mEnableMessageBoxLogging(true)
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
        inErrorCatcher->getError(ss);
        log(ss.str());
    }


    void ErrorReporter::log(const std::string & inError)
    {
        if (!inError.empty())
        {
            if (mLogFunction)
            {
                mLogFunction(inError);
            }
            else if (mEnableMessageBoxLogging)
            {
                std::wstring utf16Message = XULWin::ToUTF16(inError);
                ::MessageBox(0, utf16Message.c_str(), 0, MB_OK);
            }
            else
            {
                // The user disabled message box logging and did not provide a log functor.
                // This log message is lost.
            }
        }
    }


    void ReportError(const std::string & inError)
    {
        ErrorReporter::Instance().reportError(Error(inError));
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

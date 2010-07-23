#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "XULWin/Windows.h"
#include <boost/bind.hpp>
#include <sstream>
#include <assert.h>


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
        }

        if (hasCaught() && !mDisableLogging)
        {
            log();
        }
    }


    ErrorCatcher::ErrorCatcher(const ErrorCatcher & rhs) :
        mOwns(false), // copy is not responsible for cleanup
        mErrors(rhs.mErrors)
    {
    }


    void ErrorCatcher::disableLogging(bool inDisableLogging)
    {
        mDisableLogging = inDisableLogging;
    }


    bool ErrorCatcher::hasCaught() const
    {
        return !mErrors.empty();
    }


    void ErrorCatcher::log()
    {
        ErrorReporter::Instance().log(this);
    }


    const std::vector<Error> & ErrorCatcher::errors() const
    {
        return mErrors;
    }


    void ErrorCatcher::getError(std::stringstream & ss) const
    {       
        ss << "The following errors have occured:\n"; 
        getErrorImpl(ss);
    }


    void ErrorCatcher::getErrorImpl(std::stringstream & ss) const
    {
        for (size_t idx = 0; idx != errors().size(); ++idx)
        {
            const Error & error = errors()[idx];
            if (!error.file().empty())
            {
                ss << error.file() << ":" << error.line() << " ";
            }
            ss << error.message() << "\n";
        }
    }


    void ErrorCatcher::push(const Error & inError)
    {
        mErrors.push_back(inError);
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
        assert(mErrorCatchers.empty());
    }


    void ErrorReporter::setLogger(const LogFunction & inLogFunction)
    {
        mLogFunction = inLogFunction;
    }


    void ErrorReporter::reportError(const Error & inError)
    {
        // Let the most recent error catcher handle it.
        if (!mErrorCatchers.empty())
        {
            mErrorCatchers.top()->push(inError);
        }
        // If there are no error catchers then just log the message.
        else
        {
            log(inError.message());
        }
    }


    void ErrorReporter::push(ErrorCatcher * inErrorCatcher)
    {
        mErrorCatchers.push(inErrorCatcher);
    }


    void ErrorReporter::pop(ErrorCatcher * inErrorCatcher)
    {
        if (mErrorCatchers.empty())
        {
            throw std::logic_error("Error stack is empty on pop. This should not happen.");
        }

        if (mErrorCatchers.top() != inErrorCatcher)
        {
            throw std::logic_error("Error being popped isn't on top of the stack. This does not make sense.");
        }
        
        mErrorCatchers.pop();
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


    //bool TryCatch(const TryAction & inTryAction, const CatchAction & inCatchAction)
    //{
    //    try
    //    {
    //        inTryAction();
    //        return true;
    //    }
    //    catch (const std::exception & inExc)
    //    {
    //        inCatchAction(inExc);
    //    }
    //    return false;
    //}


    //bool TryOrReportError(const TryAction & inAction)
    //{
    //    struct Helper
    //    {
    //        static void ReportError_(const std::exception & inException)
    //        {
    //            XULWin::ReportError(inException.what());
    //        }
    //    };
    //    return TryCatch(inAction, boost::bind(&Helper::ReportError_, _1));
    //}


} // namespace XULWin

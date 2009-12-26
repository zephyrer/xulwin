#include "XULWin/EventListener.h"
#include "XULWin/Decorator.h"
#include "XULWin/ConditionalState.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>


namespace XULWin
{
    
    ConditionalState * ConditionalState::sInstance(0);

    
    ScopedConditional::ScopedConditional() :
        mImpl(0)
    {
    }
    
    
    ScopedConditional::ScopedConditional(Element * inElement)
        : mImpl(new Impl(inElement))
    {
    }

    
    ScopedConditional::ScopedConditional(const ScopedConditional & rhs)
    {
        copy(rhs);
    }


    ScopedConditional & ScopedConditional::operator=(const ScopedConditional & rhs)
    {
        if (this != &rhs)
        {
            destroy();
            copy(rhs);
        }
        return *this;
    }
    
    
    ScopedConditional::~ScopedConditional()
    {
        destroy();
    }

    
    void ScopedConditional::copy(const ScopedConditional & rhs)
    {
        mImpl = rhs.mImpl;
        mImpl->mRefCount++;
    }

    
    void ScopedConditional::destroy()
    {
        if (mImpl)
        {
            mImpl->mRefCount--;
            if (mImpl->mRefCount == 0)
            {
                delete mImpl;
                mImpl = 0;
            }
        }
    }

    
    ScopedConditional::Impl::Impl(Element * inElement) :
        mElement(inElement),
        mRefCount(1)
    {
    }


    ScopedConditional::Impl::~Impl()
    {
        ConditionalState::Instance().remove(mElement);
    }


    void ConditionalState::Initialize(HINSTANCE hInstance)
    {
        assert(!sInstance);
        sInstance = new ConditionalState(hInstance);
    }


    ConditionalState & ConditionalState::Instance()
    {
        assert(sInstance);
        return *sInstance;
    }


    void ConditionalState::Finalize()
    {
        assert(sInstance);
        delete sInstance;
        sInstance = 0;
    }


    ConditionalState::ConditionalState(HINSTANCE hInstance) :
        mForegroundIdleProc(0)
    {
        mForegroundIdleProc = ::SetWindowsHookEx(WH_FOREGROUNDIDLE, 
                                                 (HOOKPROC)&ConditionalState::ForegroundIdleProc,
                                                 hInstance,
                                                 ::GetCurrentThreadId());
    }
    
    
    ConditionalState::~ConditionalState()
    {
        ::UnhookWindowsHookEx(mForegroundIdleProc);
    }


    ScopedConditional ConditionalState::associate(const Condition & inCondition,                 
                                                  Element * inElement,
                                                  const std::string & inAttributeName,
                                                  const std::string & inValueIfConditionIsTrue,
                                                  const std::string & inValueIfConditionIsFalse)
    {
        mMapping[inElement].push_back(Conditional(inCondition,
                                                  inAttributeName,
                                                  inValueIfConditionIsTrue,
                                                  inValueIfConditionIsFalse));
        return ScopedConditional(inElement);
    }
    
    
    void ConditionalState::remove(Element * inElement)
    {
        Mapping::iterator it = mMapping.find(inElement);
        assert(it != mMapping.end());
        if (it != mMapping.end())
        {
            mMapping.erase(it);
        }
    }

    
    void ConditionalState::updateStates()
    {
        Mapping::iterator it = mMapping.begin(), end = mMapping.end();
        for (; it != end; ++it)
        {
            Element * element = it->first;
            const Conditionals & conditionals = it->second;
            for (size_t idx = 0; idx != conditionals.size(); ++idx)
            {
                const Conditional & cond = conditionals[idx];
                if (cond.aCondition())
                {
                    element->setAttribute(cond.AttributeName, cond.ValueIfConditionIsTrue);
                }
                else
                {
                    element->setAttribute(cond.AttributeName, cond.ValueIfConditionIsFalse);
                }
            }
        }
    }
	
    
    LRESULT CALLBACK ConditionalState::ForegroundIdleProc(int nCode, DWORD wParam, LONG lParam)
	{
		if (nCode < 0) // do not process message 
		{
			return ::CallNextHookEx(sInstance->mForegroundIdleProc, nCode, wParam, lParam); 
		}

        sInstance->updateStates();
        return ::CallNextHookEx(sInstance->mForegroundIdleProc, nCode, wParam, lParam);
    }


    ScopedConditional SetEnabledCondition(const ConditionalState::Condition & inCondition, Element * inElement)
    {
        return ConditionalState::Instance().associate(inCondition, inElement, "disabled", "false", "true");
    }


    ScopedConditional SetVisibleCondition(const ConditionalState::Condition & inCondition, Element * inElement)
    {
        return ConditionalState::Instance().associate(inCondition, inElement, "hidden", "false", "true");
    }

} // namespace XULWin

#ifndef CONDITIONALSTATE_H_INCLUDED
#define CONDITIONALSTATE_H_INCLUDED


#include "XULWin/Windows.h"
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>


namespace XULWin
{
    class Initializer;
    class ConditionalState;

    /**
     * ScopedConditional is a helper class for ConditionalState.
     * On destruction it removes the association between an element and its
     * conditional states.
     */
    class ScopedConditional
    {
    public:
        ScopedConditional();
        ScopedConditional(Poco::XML::Element * inElement);
        ScopedConditional(const ScopedConditional & rhs);
        ScopedConditional & operator=(const ScopedConditional & rhs);
        ~ScopedConditional();

    private:
        friend class ConditionalState;
        void copy(const ScopedConditional & rhs);
        void destroy();
        class Impl : boost::noncopyable
        {
        public:
            Impl(Poco::XML::Element * inElement);
            ~Impl();
            int mRefCount;
            Poco::XML::Element * mElement;
        };
        Impl * mImpl;
    };

    /**
     * Allows you to associate an element's attribute value with a conditional expression.
     * The updating occurs during each idle event.
     */
    class ConditionalState : boost::noncopyable
    {
    public:

        /**
         * Get the instance.
         */
        static ConditionalState & Instance();

        /**
         * Condition function object.
         */
        typedef boost::function<bool()> Condition;

        /**
         * Create an association between a condition and corresponding element states.
         * The association will remain as long as the returned ScopedConditional object
         * stays alive.
         *
         * WARNING: The condition function will be called many times so it would better
         *          not be an expensive one.
         */
        ScopedConditional associate(const Condition & inCondition,
                                    Poco::XML::Element * inElement,
                                    const std::string & inAttributeName,
                                    const std::string & inValueIfConditionIsTrue,
                                    const std::string & inValueIfConditionIsFalse);

    private:
        friend class Initializer;
        friend class ScopedConditional::Impl;
        static void Initialize(HINSTANCE hInstance);
        static void Finalize();

        ConditionalState(HINSTANCE hInstance);
        ~ConditionalState();

        void remove(Poco::XML::Element * inElement);

        void updateStates();

        struct Conditional
        {
            Conditional(Condition inCondition,
                        const std::string & inAttributeName,
                        const std::string & inValueIfTrue,
                        const std::string & inValueIfFalse) :
                aCondition(inCondition),
                AttributeName(inAttributeName),
                ValueIfConditionIsTrue(inValueIfTrue),
                ValueIfConditionIsFalse(inValueIfFalse)
            {
            }
            Condition aCondition;
            std::string AttributeName;
            std::string ValueIfConditionIsTrue;
            std::string ValueIfConditionIsFalse;
        };
        typedef std::vector<Conditional> Conditionals;
        typedef std::map<Poco::XML::Element *, Conditionals> Mapping;
        Mapping mMapping;
        HHOOK mForegroundIdleProc;

        static LRESULT CALLBACK ForegroundIdleProc(int nCode, DWORD wParam, LONG lParam);
        static ConditionalState * sInstance;
    };


    /**
     * Shorter syntax for associating a conditional with the (negated) "disabled" attribute.
     */
    ScopedConditional SetEnabledCondition(const ConditionalState::Condition & inCondition, Poco::XML::Element * inElement);

    /**
     * Shorter syntax for associating a conditional with the (negated) "hidden" attribute.
     */
    ScopedConditional SetVisibleCondition(const ConditionalState::Condition & inCondition, Poco::XML::Element * inElement);


} // namespace XULWin

#endif // CONDITIONALSTATE_H_INCLUDED

#ifndef ATTRIBUTECONTROLLERPOLICY_H_INCLUDED
#define ATTRIBUTECONTROLLERPOLICY_H_INCLUDED


#include "XULWin/AttributeController.h"
#include "XULWin/Fallible.h"
#include <map>
#include <string>


namespace XULWin
{

    template<class MemberType>
    class MemberController
    {
    public:
        const MemberType & getMember() const
        { return mMemberType; }

        void setMember(const MemberType & inMemberType)
        { mMemberType = inMemberType; }

    private:
        MemberType mMemberType;
    };

    template<class MemberType>
    class FallibleMemberController
    {
    public:
        FallibleMemberController(const MemberType & inDefaultValue) :
            mDefaultValue(inDefaultValue)
        { }

        const MemberType & getMember() const
        { return mMemberType.or(mDefaultValue); }

        void setMember(const MemberType & inMemberType)
        { mMemberType = inMemberType; }

    private:
        Fallible<MemberType> mMemberType;
        MemberType mDefaultValue;
    };


    class LabelController_AsMember : public LabelController,
                                     private MemberController<std::string>
    {
    public:
        virtual std::string getLabel() const
        { return MemberController<std::string>::getMember(); }

        virtual void setLabel(const std::string & inLabel)
        { MemberController<std::string>::setMember(inLabel); }
    };


    class LabelController_AsFallibleMember : public LabelController,
                                             private FallibleMemberController<std::string>
    {
    public:
        LabelController_AsFallibleMember(const std::string & inDefaultValue)
            : FallibleMemberController(inDefaultValue)
        {}

        virtual std::string getLabel() const
        { return FallibleMemberController<std::string>::getMember(); }

        virtual void setLabel(const std::string & inLabel)
        { FallibleMemberController<std::string>::setMember(inLabel); }
    };


    class RowsController_AsMember : public RowsController,
                                    private MemberController<int>
    {
    public:
        virtual int getRows() const
        { return MemberController<int>::getMember(); }

        virtual void setRows(int inRows)
        { MemberController<int>::setMember(inRows); }
    };


    class Component;
    class AttributesMapping;
    template<class SuperClass, class ComponentType, class AttributeControllerType>
    class AttributeInitializer : public SuperClass,
                                 public AttributeControllerType
    {
    public:
        AttributeInitializer(Component * inParent, const AttributesMapping & inAttributesMapping) :
            SuperClass(inParent, inAttributesMapping)
        {
        }

        virtual bool initAttributeControllers()
        {
            ComponentType * pThis(static_cast<ComponentType*>(this));
            pThis->setAttributeController(AttributeControllerType::PropertyName(),
                                          static_cast<AttributeControllerType*>(pThis));
            return pThis->SuperClass::initAttributeControllers();
        }        
    };

} // namespace XULWin


#endif // ATTRIBUTECONTROLLERPOLICY_H_INCLUDED

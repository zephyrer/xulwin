#ifndef ATTRIBUTECONTROLLERPOLICY_H_INCLUDED
#define ATTRIBUTECONTROLLERPOLICY_H_INCLUDED


#include "XULWin/AttributeController.h"
#include "XULWin/Fallible.h"
#include <boost/function.hpp>
#include <map>
#include <string>


namespace XULWin
{
    
    //class LabelController_AsMember : public LabelController
    //{
    //public:
    //    virtual std::string getLabel() const
    //    { return mLabel; }

    //    virtual void setLabel(const std::string & inLabel)
    //    { mLabel = inLabel; }

    //private:
    //    std::string mLabel;
    //};


    //class LabelController_AsFallibleMember : public LabelController
    //{
    //public:
    //    LabelController_AsFallibleMember(const std::string & inDefaultLabel) :
    //        mDefaultLabel(inDefaultLabel)
    //    {}

    //    virtual std::string getLabel() const
    //    { return mFallibleLabel.or(mDefaultLabel); }

    //    virtual void setLabel(const std::string & inLabel)
    //    { mFallibleLabel = inLabel; }

    //private:
    //    std::string mDefaultLabel;
    //    Fallible<std::string> mFallibleLabel;
    //};


    //class LabelController_WithFunctions : public LabelController
    //{
    //public:
    //    typedef boost::function<void(const std::string & )> Setter;
    //    typedef boost::function<std::string()> Getter;
    //    LabelController_WithFunctions(const Getter & inGetter, const Setter & inSetter) :
    //        mGetter(inGetter),
    //        mSetter(inSetter)
    //    {
    //    }

    //    virtual std::string getLabel() const
    //    { return mGetter(); }

    //    virtual void setLabel(const std::string & inLabel)
    //    { mSetter(inLabel); }

    //private:
    //    Getter mGetter;
    //    Setter mSetter;
    //};


    //class Component;
    //class AttributesMapping;
    //template<class SuperClass, class ComponentType, class AttributeControllerType>
    //class AttributeInitializer : public SuperClass,
    //                             public AttributeControllerType
    //{
    //public:
    //    AttributeInitializer(Component * inParent, const AttributesMapping & inAttributesMapping) :
    //        SuperClass(inParent, inAttributesMapping)
    //    {
    //    }

    //    virtual bool initAttributeControllers()
    //    {
    //        ComponentType * pThis(static_cast<ComponentType*>(this));
    //        pThis->setAttributeController(AttributeControllerType::PropertyName(),
    //                                      static_cast<AttributeControllerType*>(pThis));
    //        return pThis->SuperClass::initAttributeControllers();
    //    }        
    //};

} // namespace XULWin


#endif // ATTRIBUTECONTROLLERPOLICY_H_INCLUDED

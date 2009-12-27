#ifndef DYNAMICATTRIBUTE_H_INCLUDED
#define DYNAMICATTRIBUTE_H_INCLUDED


#include "XULWin/Enums.h"
#include "XULWin/Point.h"
#include "XULWin/SVGPathInstructions.h"
#include "XULWin/RGBColor.h"
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>


/*
                                    Attribute Controllers          Dynamic Attributes
                                    ---------------------          ------------------
        Example usage:              comp->getTitle();              comp->get<Title>();
                                    comp->setWidth(80);            comp->set<Width>(80);
        Technology:                 multiple inheritance           visitor-like pattern
        Attributes are defined:     per class                      per object
        Attributes are added:       at compile-time                at run-time
        Defining new attributes:    intrusive                      non-intrusive
        Attribute lookup:           string to object mapping       string to object mapping

        Dynamic attributes allow to create a small core API that can be extended externally.
        Attribute controllers require you to modify the core code each time a new attribute is defined.
        So dynamic attributes follow the principle of "strong cohesion and weak coupling" more than
        attribute controllers do.
*/


namespace XULWin
{

    class Attribute
    {
    public:
        Attribute(const std::string & inName);

        virtual ~Attribute();

        virtual void getStringValue(std::string & outValue) = 0;

        virtual void setStringValue(const std::string & inValue) = 0;

        const std::string & name() const;

    private:
        std::string mName;
    };


    template<class ValueT, class RefT>
    class TypedAttribute : public Attribute
    {
    public:
        typedef RefT RefType;
        typedef ValueT ValueType;
        
        // We have to return a value type for the getter because we don't know how the variable
        // will be stored and retrieved.
        typedef boost::function<ValueType()> Getter;
        typedef boost::function<void(ValueType)> Setter;

        TypedAttribute(const std::string & inName, Getter inGetter, Setter inSetter) :
            Attribute(inName),
            mGetter(inGetter),
            mSetter(inSetter)
        {
        }

        RefType get() const
        {
            return mGetter();
        }

        void set(RefType inValue)
        {
            mSetter(inValue);
        }

        virtual void getStringValue(std::string & outValue)
        {
            outValue = boost::lexical_cast<std::string>(get());
        }

        virtual void setStringValue(const std::string & inValue)
        {
            set(boost::lexical_cast<ValueType>(inValue));
        }

    private:
        boost::function<RefType()> mGetter;
        boost::function<void(RefType)> mSetter;
    };


    class AttributeContainer
    {
    public:
        template<class T>
        void registerAttribute(typename T::Getter inGetter,
                               typename T::Setter inSetter)
        {
            if (mAttributes.find(T::Name()) == mAttributes.end())
            {
                mAttributes.insert(std::make_pair(T::Name(), new T(inGetter, inSetter)));
            }
        }

        template<class T>
        bool hasAttribute() const
        {
            return hasAttribute(T::Name());
        }

        
        bool hasAttribute(const std::string & inAttributeName) const
        {
            Attributes::const_iterator it = mAttributes.find(inAttributeName);
            return it != mAttributes.end();
        }

        
        bool getAttr(const std::string & inAttributeName, Attribute *& outAttribute) const
        {
            Attributes::const_iterator it = mAttributes.find(inAttributeName);            
            if (it != mAttributes.end())
            {
                outAttribute = it->second;
                return true;
            }
            return false;
        }


        template<class T>
        const T & attribute() const
        {
            Attributes::const_iterator it = mAttributes.find(T::Name());
            if (it == mAttributes.end())
            {
                std::string name = T::Name();
                std::string msg = "No attribute found with name '" + name + "'.";
                throw std::runtime_error(msg.c_str());
            }
            T * attr = dynamic_cast<T*>(it->second);
            if (!attr)
            {
                throw std::runtime_error("Dynamic cast failed.");
            }
            return *attr;
        }

        template<class T>
        T & attribute()
        {
            return const_cast<T &>(static_cast<const AttributeContainer*>(this)->attribute<T>());
        }

        const Attribute * attribute(const std::string & inName) const
        {
            Attributes::const_iterator it = mAttributes.find(inName);
            if (it == mAttributes.end())
            {
                std::string msg = "No attribute found with name '" + inName + "'.";
                throw std::runtime_error(msg.c_str());
            }
            return it->second;
        }

        Attribute * attribute(const std::string & inName)
        {
            return const_cast<Attribute*>(static_cast<const AttributeContainer*>(this)->attribute(inName));
        }

        template<class T>
        typename T::RefType getAttr() const
        {
            const T & attr = attribute<T>();
            return attr.get();
        }

        template<class T>
        void setAttr(typename T::RefType inValue)
        {
            T & attr = attribute<T>();
            attr.set(inValue);
        }

    private:
        typedef std::map<std::string, Attribute*> Attributes;
        Attributes mAttributes;
    };

    
    /**
     * DECLARE_ATTRIBUTE
     * 
     * This macro declares a XUL attribute.
     */
    #define DECLARE_ATTRIBUTE(classname, valuetype, reftype, attributename)                           \
        class classname : public TypedAttribute<valuetype, reftype>                                   \
        {                                                                                             \
        public:                                                                                       \
            typedef TypedAttribute<valuetype, reftype> Super;                                         \
            classname(Super::Getter inGetter, Super::Setter inSetter) :                               \
                Super(Name(), inGetter, inSetter)                                                     \
            {                                                                                         \
            }                                                                                         \
                                                                                                      \
            static const char * Name() { return attributename; }                                      \
                                                                                                      \
        };

    /**
     * DECLARE_INT_ATTRIBUTE
     *
     * Declares a XUL attribute that has an int value.
     */
    #define DECLARE_INT_ATTRIBUTE(classname, attributename) \
        DECLARE_ATTRIBUTE(classname, int, int, attributename)

    /**
     * DECLARE_BOOL_ATTRIBUTE
     *
     * Declares a XUL attribute that has a boolean value.
     */
    #define DECLARE_BOOL_ATTRIBUTE(classname, attributename) \
        DECLARE_ATTRIBUTE(classname, bool, bool, attributename)

    /**
     * DECLARE_STRING_ATTRIBUTE
     *
     * Declares a XUL attribute that has a std::string value.
     */
    #define DECLARE_STRING_ATTRIBUTE(classname, attributename) \
        DECLARE_ATTRIBUTE(classname, std::string, const std::string &, attributename)

    
    DECLARE_INT_ATTRIBUTE(Width, "width")
    DECLARE_INT_ATTRIBUTE(Height, "width")
    DECLARE_BOOL_ATTRIBUTE(Hidden, "hidden")
    DECLARE_STRING_ATTRIBUTE(Title, "title")


} // namespace XULWin


#endif // DYNAMICATTRIBUTE_H_INCLUDED

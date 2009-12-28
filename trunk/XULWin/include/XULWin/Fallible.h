#ifndef FALLIBLE_H_INCLUDED
#define FALLIBLE_H_INCLUDED


namespace XULWin
{

    template<class T>
    class Fallible
    {
    public:
        Fallible() :
          mValid(false)
        {
        }

        Fallible(const T & inValue) :
            mValue(inValue),
            mValid(true)
        {
        }

        T & operator= (const T & inValue)
        {
            mValid = true;
            mValue = inValue;
            return mValue;
        }

        bool isValid() const
        {
            return mValid;
        }


        /**
         * setInvalid()
         *
         * This method allows marking an object as invalid even though
         * it has a value set. The motivation behind this is to have
         * the object return a default value instead of garbage when
         * queried in fault state.
         *
         * Fallible number<int>(0); // 0 as default value, object is valid.
         * number.setInvalid(); // Mark as invalid.
         * int n = number; // 0 returned instead of garbage
         */
        void setInvalid()
        {
            mValid = false;
        }


        operator bool() const
        {
            return mValid;
        }

        
        operator const T &() const
        {
            return mValue;
        }


        T & operator->()
        {
            return mValue;
        }


        const T & operator->() const
        {
            return mValue;
        }


        const T & getValue() const
        {
            return mValue;
        }


        const T & or(const T & inDefault) const
        {
            if (mValid)
            {
                return mValue;
            }
            return inDefault;
        }

    private:
        bool mValid;
        T mValue;
    };

} // namespace XULWin


#endif // FALLIBLE_H_INCLUDED

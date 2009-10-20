#ifndef IMAGEIMPL_H_INCLUDED
#define IMAGEIMPL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class Image : public HWNDControl,
                      public virtual SrcController,
                      public virtual KeepAspectRatioController,
                      public GdiplusLoader
    {
    public:
        typedef HWNDControl Super;

        Image(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual std::string getSrc() const;

        virtual void setSrc(const std::string & inSrc);
        
        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
        
        virtual int getWidth(SizeConstraint inSizeConstraint) const;
        
        virtual int getHeight(SizeConstraint inSizeConstraint) const;

        virtual void move(int x, int y, int w, int h);

        virtual bool getKeepAspectRatio() const;

        virtual void setKeepAspectRatio(bool inKeepAspectRatio);

        virtual bool initAttributeControllers();
        
        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        void paintImage(HDC inHDC, const RECT & rc);

        void getWidthAndHeight(int & outWidth, int & outHeight) const;

        boost::scoped_ptr<Gdiplus::Bitmap> mImage;
        boost::scoped_ptr<Gdiplus::Bitmap> mCachedImage;
        std::string mSrc;
        bool mKeepAspectRatio;
    };


} // namespace XULWin


#endif // IMAGEIMPL_H_INCLUDED

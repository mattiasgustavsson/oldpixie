/**
 * \class	StaticText
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __StaticText_H__
#define __StaticText_H__

// Includes
#include "StringId.h"
#include "Sprite.h"
#include "Array.h"
#include "Resource_Font.h"

// Forward declares
class SpriteManager;

// StaticText
class StaticText:public Sprite
	{
	public:
		StaticText();
		StaticText(SpriteManager* spriteManager);
		virtual ~StaticText();

		virtual const Resource_Font& GetFont() const;
		virtual void SetFont(const Resource_Font& font);

		virtual const char* GetText() const;
		virtual void SetText(const char* text);

		virtual int GetSpacing() const;
		virtual void SetSpacing(int spacing);

		virtual int GetWidth() const;
		virtual int GetVSpacing() const;
		virtual void SetWidth(int width, int vspacing);

		virtual void SetCentered( bool centered );
		virtual bool GetCentered() const;

		virtual int GetBlockHeight() const;

		virtual int GetLimit() const;
		virtual void SetLimit(int limit);
	protected:
		virtual void Render(Bitmap& bitmap);

	private: 
		Resource_Font font_;
		char* text_;
		int spacing_;
		int width_;
		int vspacing_;
		bool centered_;
		int limit_;
	};


#endif /* __StaticText_H__ */


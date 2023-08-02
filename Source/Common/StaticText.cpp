//*** StaticText.cpp **

#include "StaticText.h"
#include "BitmapStrip.h"
#include "Font.h"
#include "DebugFont.h"


//*** Constructor ***

StaticText::StaticText():
	spacing_(0),
	width_(0),
	vspacing_(0),
	text_(0),
	centered_(0),
	limit_( -1 )
	{
	}


//*** Constructor ***

StaticText::StaticText(SpriteManager* spriteManager):
	Sprite(spriteManager),
	spacing_(0),
	width_(0),
	vspacing_(0),
	text_(0),
	centered_(0),
	limit_( -1 )
	{
	}


//*** Destructor ***

StaticText::~StaticText()
	{
	if (text_)
		{
		Free(text_);
		}
	}


//*** GetFont ***

const Resource_Font& StaticText::GetFont() const
	{
	return font_;
	}


//*** SetFont ***

void StaticText::SetFont(const Resource_Font& font)
	{
	font_=font;
	}


//*** GetText ***

const char* StaticText::GetText() const
	{
	return text_;
	}


//*** SetText ***

void StaticText::SetText(const char* text)
	{
	if (text_)
		{
		Free(text_);
		text_=0;
		}

	if (text)
		{
		text_=StrDup(text);
		}
	}


//*** GetSpacing ***

int StaticText::GetSpacing() const
	{
	return spacing_;
	}


//*** SetSpacing ***

void StaticText::SetSpacing(int spacing)
	{
	spacing_=spacing;
	}


//*** GetWidth ***

int StaticText::GetWidth() const
	{
	return width_;
	}


//*** GetVSpacing ***

int StaticText::GetVSpacing() const
	{
	return vspacing_;
	}


//*** SetWidth ***

void StaticText::SetWidth(int width, int vspacing)
	{
	width_=width;
	vspacing_=vspacing;
	}


//*** SetCentered ***

void StaticText::SetCentered( bool centered )
	{
	centered_ = centered;
	}


//*** GetCentered ***

bool StaticText::GetCentered() const
	{
	return centered_;
	}


int StaticText::GetBlockHeight() const
	{
	return font_.GetFont().GetBoundsWrap( GetText(), GetWidth(), GetSpacing(), GetVSpacing() ).height;
	}

//*** SetLimit ***

void StaticText::SetLimit( int limit )
	{
	limit_ = limit;
	}


//*** GetLimit ***

int StaticText::GetLimit() const
	{
	return limit_;
	}

//*** Render ***

void StaticText::Render(Bitmap& bitmap)
	{
	Sprite::Render(bitmap);

	if (!IsVisible())
		{
		return;
		}

	if (GetAlpha()==0)
		{
		return;
		}
	
	if (!text_)
		{
		return;
		}

	if (font_.GetFont().GetSize()==0)
		{
		DebugFont font;
		font.Blit(bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetText(),GetColor());
		return;
		}
		
	if (width_==0)
		{
		font_.Blit(bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetText(),GetSpacing(),GetColor(),GetAlpha());
		}
	else
		{
		if( centered_ )
			{
			font_.BlitCentered(bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetText(),GetLimit(),GetWidth(),GetSpacing(),GetVSpacing(),GetColor(),GetAlpha());
			}
		else
			{
			font_.BlitWrap(bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetText(),GetLimit(), GetWidth(),GetSpacing(),GetVSpacing(),GetColor(),GetAlpha());
			}
		}
	}




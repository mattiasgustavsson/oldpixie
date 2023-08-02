//*** SpriteAction_BounceIn.cpp ***
 
#include "SpriteAction_BounceIn.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "SpriteController.h"


//*** Constructor ***

SpriteAction_BounceIn::SpriteAction_BounceIn(float targetX, float targetY, float time):
	targetX_(targetX),
	targetY_(targetY),
	time_(time),
	elapsed_(0.0f),
	sourceX_(0),
	sourceY_(0)
	{
	}

//*** Setup ***

void SpriteAction_BounceIn::Setup(SpriteController* controller, Sprite* sprite)
	{
	sourceX_ = sprite->GetX();
	sourceY_ = sprite->GetY();
	}

void SpriteAction_BounceIn::Finalize(SpriteController* controller, Sprite* sprite)
	{
	sprite->SetPosition(targetX_,targetY_);
	}


static inline float ease_bounce_out( float t ) { if( t < ( 1.0f / 2.75f ) ) return 7.5625f * t * t; else if( t < ( 2.0f / 2.75f ) ) return 7.5625f * ( t -= ( 1.50f / 2.75f ) ) * t + 0.75f; else if( t < ( 2.5f / 2.75f ) ) return 7.5625f * ( t -= ( 2.25f / 2.75f ) ) * t + 0.9375f; else return 7.5625f * ( t -= ( 2.625f / 2.75f ) ) * t + 0.984375f; }
static inline float ease_bounce_in( float t ) { return 1.0f - ease_bounce_out( 1.0f - t ); }

//*** Update ***

void SpriteAction_BounceIn::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	elapsed_ += deltaTime;

	if( elapsed_ >= time_ )
		{
		sprite->SetPosition(targetX_,targetY_);
		ActionCompleted();
		return;
		}

	float t = elapsed_ / time_;
	t = ease_bounce_out( t );
	float x = ( targetX_ - sourceX_ ) * t;
	float y = ( targetY_ - sourceY_ ) * t;
	controller->AddOffset( x, y );
	}

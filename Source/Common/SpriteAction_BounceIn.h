/**
 * \class	SpriteAction_BounceIn
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_BounceIn_H__
#define __SpriteAction_BounceIn_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_BounceIn
class SpriteAction_BounceIn:public SpriteAction
	{
	public:
		SpriteAction_BounceIn(float targetX, float targetY, float time);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Finalize(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		float targetX_;
		float targetY_;
		float time_;
		float elapsed_;

		float sourceX_;
		float sourceY_;
	};

#endif /* __SpriteAction_BounceIn_H__ */


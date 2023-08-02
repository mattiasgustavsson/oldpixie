/**
 * \class	FrameTime
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __FrameTime_H__
#define __FrameTime_H__

// Includes

// Forward declares

// FrameTime
class FrameTime
	{
	public:
		FrameTime();
		
		float Update();
		float GetDeltaTime();
		int GetFrameCounter();

		void LockFrameRate( int fps );

	private:
		bool initialized_;
		float previousFrameTime_;
		float deltaTime_;
		int frameCounter_;
		int frameRateLock_;
	};

#endif /* __FrameTime_H__ */


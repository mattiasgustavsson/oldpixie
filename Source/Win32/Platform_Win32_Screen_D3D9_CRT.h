/**
 * \class	Platform_Win32_Screen_D3D9_CRT
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Screen_D3D9_CRT_H__
#define __Platform_Win32_Screen_D3D9_CRT_H__

// Includes
#include "Platform_Win32_Screen_Technology.h"

// Forward declares
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DTexture9;
struct IDirect3DPixelShader9;

// Platform_Win32_Screen_D3D9_CRT
class Platform_Win32_Screen_D3D9_CRT:public Platform_Win32_Screen_Technology
	{

	public:
		Platform_Win32_Screen_D3D9_CRT(struct HWND__* windowHandle, bool fullscreen, int screenX, int screenY, int screenWidth, int screenHeight, bool interpolationMode, int monitor );
		virtual ~Platform_Win32_Screen_D3D9_CRT();

		virtual bool Setup();

		virtual bool Present(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate,
			unsigned short backgroundColor
			);
	
		virtual bool Present(
			unsigned int* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned int modulate,
			unsigned int backgroundColor
			) { return false;}

		virtual void SetInterpolationMode(
			bool enabled) {};


		virtual void TransformCursorCoordinates(float& x, float& y);

	private:
		void InvTransformCursorCoordinates(float& x, float& y);
		void DrawQuad( float srcX1, float srcY1, float srcX2, float srcY2, float dstX1, float dstY1, float dstX2, float dstY2, unsigned int modulate );
		void DrawQuadBlur( float srcX1, float srcY1, float srcX2, float srcY2, float dstX1, float dstY1, float dstX2, float dstY2, bool horiz, float r );
		void ToTexture( unsigned short* data, int width, int height );
		void ToScreen( IDirect3DTexture9* texture, IDirect3DTexture9* blur, int width, int height, unsigned int modulate );
		void TexToTex( IDirect3DTexture9* target, IDirect3DTexture9* source, int width, int height, unsigned int modulate);
		void Accumulate( IDirect3DTexture9* target, IDirect3DTexture9* sourceA, IDirect3DTexture9* sourceB, int width, int height, unsigned int modulate );
		void Blur( IDirect3DTexture9* source, IDirect3DTexture9* blendA, IDirect3DTexture9* blendB, int width, int height, float r );
		void Blend( IDirect3DTexture9* target, IDirect3DTexture9* sourceA, IDirect3DTexture9* sourceB, int width, int height, unsigned int modulate );
		void CreateFrame();

	private:
		struct HWND__* windowHandle_;
		bool fullscreen_;
		int screenX_;
		int screenY_;
		int screenWidth_;
		int screenHeight_;
		int monitor_;
		void* d3dDLL_;
		IDirect3D9* direct3D_;
		IDirect3DDevice9* device_;
		IDirect3DTexture9* texture_;
		IDirect3DTexture9* accumulationA_;
		IDirect3DTexture9* accumulationB_;
		IDirect3DTexture9* blurA_;
		IDirect3DTexture9* blurB_;
		IDirect3DTexture9* frame_;
		IDirect3DPixelShader9* accumulationShader_;
		IDirect3DPixelShader9* blendShader_;
		IDirect3DPixelShader9* blurShader_;
		IDirect3DPixelShader9* crtShader_;
		int lastPresentWidth_;
		int lastPresentHeight_;
		float time_;
	};

		

#endif /* __Platform_Win32_Screen_D3D9_CRT_H__ */

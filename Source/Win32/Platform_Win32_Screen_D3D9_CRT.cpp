//*** Platform_Win32_Screen_D3D9_CRT.cpp ***

//#define  DO_COMPILE

#include "Platform_Win32_Screen_D3D9_CRT.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "Platform_OS.h"
#define DebugPrint(x) Platform::GetPlatform_OS()->OutputDebugText x;

extern unsigned char CRT_frame_color[ 331668 ];
extern unsigned char CRT_frame_alpha[ 50172 ];

#include "ps_main.h"
#include "ps_blend_main.h"
#include "ps_acc_main.h"
#include "ps_blur_main.h"

bool crtCursorHidden_ = false;

//*** Constructor ***

Platform_Win32_Screen_D3D9_CRT::Platform_Win32_Screen_D3D9_CRT(struct HWND__* windowHandle, bool fullscreen, int screenX, int screenY, int screenWidth, int screenHeight, bool interpolationMode, int monitor):
	windowHandle_(windowHandle),
	fullscreen_(fullscreen),
	screenX_(screenX),
	screenY_(screenY),
	screenWidth_(screenWidth),
	screenHeight_(screenHeight),
	monitor_( monitor ),
	d3dDLL_(0),
	direct3D_(0),
	device_(0),
	texture_(0),
	accumulationA_(0),
	accumulationB_(0),
	blurA_(0),
	blurB_(0),
	frame_(0),
	accumulationShader_( 0 ),
	blendShader_( 0 ),
	blurShader_( 0 ),
	crtShader_( 0 ),
	lastPresentWidth_(0),
	lastPresentHeight_(0),
	time_( 0 )
	{
	DebugPrint(("Creating Platform_Win32_Screen_D3D9_CRT...\n\tfullscreen:%d\n\tscreenWidth:%d\n\tscreenHeight:%d\n\tinterpolationMode:%d\n",fullscreen,screenWidth,screenHeight,interpolationMode));
//	crtCursorHidden_ = false;
	}


//*** Setup ***

bool Platform_Win32_Screen_D3D9_CRT::Setup()
	{

	// Create D3D object
	d3dDLL_=LoadLibrary("d3d9.dll");
	if (!d3dDLL_)
		{
		DebugPrint(("Couldn't find d3d9.dll\n"));
		return false;
		}

	// Create DirectDraw object
	typedef IDirect3D9 * (WINAPI *Direct3DCreate9Definition)( unsigned int SDKVersion );
	Direct3DCreate9Definition Direct3DCreate9=(Direct3DCreate9Definition)GetProcAddress((HMODULE)d3dDLL_, "Direct3DCreate9"); 
	if (Direct3DCreate9==0)
		{
		DebugPrint(("Failed to find function entry point in d3d9.dll\n"));
		return false;
		}

	direct3D_=Direct3DCreate9(D3D_SDK_VERSION);
	if (!direct3D_)
		{
		DebugPrint(("Couldn't create Direct3D object\n"));
		return false;
		}


	// Get present parameters
	D3DPRESENT_PARAMETERS d3dpp;
	if (fullscreen_)
		{
		SetWindowLong(windowHandle_,GWL_STYLE, WS_MAXIMIZE | WS_POPUP | WS_VISIBLE);
		SetWindowLong(windowHandle_, GWL_EXSTYLE, 0);
		SetWindowPos(windowHandle_,HWND_TOP,screenX_,screenY_,screenWidth_,screenHeight_,/*SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | /*SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING*/ 0);
//		SetForegroundWindow( windowHandle_ );
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		//d3dpp.BackBufferWidth=screenWidth_;
		//d3dpp.BackBufferHeight=screenHeight_;
		//d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
		//d3dpp.BackBufferCount=0;
		//d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
		//d3dpp.MultiSampleQuality=0;
		//d3dpp.SwapEffect=D3DSWAPEFFECT_FLIP;
		//d3dpp.hDeviceWindow=windowHandle_;
		//d3dpp.Windowed=0;
		//d3dpp.EnableAutoDepthStencil=FALSE;
		//d3dpp.AutoDepthStencilFormat=D3DFMT_D24S8;
		//d3dpp.Flags=0;
		//d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		//d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat=D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = FALSE;
		d3dpp.BackBufferCount=0;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
//		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
		}
	else
		{
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat=D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = FALSE;
		d3dpp.BackBufferCount=0;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
//		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
		}

	// Set device adapter to use
	UINT AdapterToUse=D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL;
	
	// Create the D3DDevice
	HRESULT ret=direct3D_->CreateDevice(AdapterToUse,DeviceType,windowHandle_, D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp,&device_);
	if (FAILED(ret) || !device_)
		{
		DebugPrint(("Couldn't create Direct3D Device\n"));
		return false;
		}


	// Use the right type of filtering for the requested interpolation mode
	device_->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE );
	device_->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR );
	device_->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR );

	device_->SetSamplerState(0,D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device_->SetSamplerState(0,D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	device_->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE );
	device_->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR );
	device_->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR );

	device_->SetSamplerState(1,D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device_->SetSamplerState(1,D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	device_->SetSamplerState(2,D3DSAMP_MIPFILTER,D3DTEXF_NONE );
	device_->SetSamplerState(2,D3DSAMP_MINFILTER,D3DTEXF_LINEAR );
	device_->SetSamplerState(2,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR );

	device_->SetSamplerState(2,D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device_->SetSamplerState(2,D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	ret=device_->CreateTexture(screenWidth_,screenHeight_,1,0,D3DFMT_R5G6B5,D3DPOOL_MANAGED,&texture_,0);
	if (FAILED(ret)) { DebugPrint(("Couldn't create texture\n")); return false;	}			

	ret=device_->CreateTexture(screenWidth_,screenHeight_,1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&accumulationA_,0);
	if (FAILED(ret)) { DebugPrint(("Couldn't create accumulation\n")); return false; }			

	ret=device_->CreateTexture(screenWidth_,screenHeight_,1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&accumulationB_,0);
	if (FAILED(ret)) { DebugPrint(("Couldn't create accumulation\n")); return false; }			

	ret=device_->CreateTexture(screenWidth_,screenHeight_,1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&blurA_,0);
	if (FAILED(ret)) { DebugPrint(("Couldn't create blur\n")); return false; }			

	ret=device_->CreateTexture(screenWidth_,screenHeight_,1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&blurB_,0);
	if (FAILED(ret)) { DebugPrint(("Couldn't create blur\n")); return false; }			

	CreateFrame();
#ifndef DO_COMPILE
	ret = device_->CreatePixelShader((const DWORD*) g_ps30_acc_main, &accumulationShader_);
	if (FAILED(ret) || !accumulationShader_ ) { DebugPrint(("Couldn't create shader\n")); return false; }			

	ret = device_->CreatePixelShader((const DWORD*) g_ps30_blend_main, &blendShader_);
	if (FAILED(ret) || !blendShader_ ) { DebugPrint(("Couldn't create shader\n")); return false; }			

	ret = device_->CreatePixelShader((const DWORD*) g_ps30_blur_main, &blurShader_);
	if (FAILED(ret) || !blendShader_ ) { DebugPrint(("Couldn't create shader\n")); return false; }			

	ret = device_->CreatePixelShader((const DWORD*) g_ps30_main, &crtShader_);
	if (FAILED(ret) || !crtShader_ ) { DebugPrint(("Couldn't create shader\n")); return false; }			
#endif //  DO_COMPILE

#ifdef DO_COMPILE
	FILE* f = fopen( "crt.ps", "r" );
	fseek( f, 0, SEEK_END );
	int crtCodeLen = ftell( f );
	fseek( f, 0, SEEK_SET );
	char* crtCode = (char*) malloc( crtCodeLen + 1 );
	int s = fread( crtCode, 1, crtCodeLen, f );
	crtCode[ s ] = 0;
	printf( crtCode );
	fclose( f );
	
	HMODULE d3dx = 0;
	int version = 45;
	while( !d3dx && version >= 0 )
		{
		char str[256];
		sprintf( str, "d3dx9_%d.dll", version );
		d3dx = LoadLibrary(str);
		--version;
		}
	if (!d3dx)
		{
		DebugPrint(("Couldn't find any d3dx9 dll\n"));
		return false;
		}

	typedef HRESULT (WINAPI *CompileShaderFunc)(
        LPCSTR                          pSrcData,
        UINT                            SrcDataLen,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pFunctionName,
        LPCSTR                          pProfile,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs,
        LPD3DXCONSTANTTABLE*            ppConstantTable);


	CompileShaderFunc CompileShader = (CompileShaderFunc) GetProcAddress( d3dx, "D3DXCompileShader"); 
	if( CompileShader == 0 )
		{
		DebugPrint(("Failed to find function entry point for D3DXCompileShader\n"));
		return false;
		}


	LPD3DXBUFFER                 code = NULL;
	LPD3DXBUFFER                 errors = NULL;
	ret = CompileShader(		crtCode,
									crtCodeLen,
									NULL,          //macro's            
									NULL,          //includes           
									"acc_main",     //main function      
									"ps_3_0",      //shader profile     
									0,             //flags              
									&code,         //compiled operations
									&errors,          //errors
									NULL);         //constants

	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader:\n%s\n", errors ? (const char*)errors->GetBufferPointer() : "")); if( errors ) errors->Release(); return false; }			

	ret = device_->CreatePixelShader((DWORD*)code->GetBufferPointer(), &accumulationShader_);
	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader\n")); return false; }			
	code->Release();

	ret = CompileShader(		crtCode,
									crtCodeLen,
									NULL,          //macro's            
									NULL,          //includes           
									"blend_main",     //main function      
									"ps_3_0",      //shader profile     
									0,             //flags              
									&code,         //compiled operations
									&errors,          //errors
									NULL);         //constants

	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader:\n%s\n", errors ? (const char*)errors->GetBufferPointer() : "")); if( errors ) errors->Release(); return false; }			

	ret = device_->CreatePixelShader((DWORD*)code->GetBufferPointer(), &blendShader_);
	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader\n")); return false; }			
	code->Release();


	ret = CompileShader(		crtCode,
									crtCodeLen,
									NULL,          //macro's            
									NULL,          //includes           
									"blur_main",     //main function      
									"ps_3_0",      //shader profile     
									0,             //flags              
									&code,         //compiled operations
									&errors,          //errors
									NULL);         //constants

	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader:\n%s\n", errors ? (const char*)errors->GetBufferPointer() : "")); if( errors ) errors->Release(); return false; }			

	ret = device_->CreatePixelShader((DWORD*)code->GetBufferPointer(), &blurShader_);
	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader\n")); return false; }			
	code->Release();


	ret = CompileShader(		crtCode,
									strlen( crtCode ),
									NULL,          //macro's            
									NULL,          //includes           
									"main",     //main function      
									"ps_3_0",      //shader profile     
									0,             //flags              
									&code,         //compiled operations
									&errors,          //errors
									NULL);         //constants

	free( crtCode );
	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader:\n%s\n", errors ? (const char*)errors->GetBufferPointer() : "")); if( errors ) errors->Release(); return false; }			

	ret = device_->CreatePixelShader((DWORD*)code->GetBufferPointer(), &crtShader_);
	if (FAILED(ret) || !code ) { DebugPrint(("Couldn't compile shader\n")); return false; }			
	code->Release();

	FreeLibrary( d3dx );
#endif // DO_COMPILE

	DebugPrint(("...Platform_Win32_Screen_D3D9_CRT created\n"));
	return true;
	}

//*** Destructor ***

Platform_Win32_Screen_D3D9_CRT::~Platform_Win32_Screen_D3D9_CRT()
	{
	DebugPrint(("Destroying Platform_Win32_Screen_D3D9_CRT...\n"));

	if( crtShader_ ) crtShader_->Release();
	if( blurShader_ ) blurShader_->Release();
	if( blendShader_ ) blendShader_->Release();
	if( accumulationShader_ ) accumulationShader_->Release();
	if( frame_ ) frame_->Release();
	if (blurB_) blurB_->Release();
	if (blurA_)	blurA_->Release();
	if (accumulationB_) accumulationB_->Release();
	if (accumulationA_)	accumulationA_->Release();
	if (texture_) texture_->Release();
	if (device_) device_->Release();
	if (direct3D_) direct3D_->Release();
	if (d3dDLL_) FreeLibrary((HMODULE)d3dDLL_);

	//SetWindowPos(windowHandle_,HWND_NOTOPMOST,0,0,0,0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);
	if (fullscreen_) SetWindowLong(windowHandle_,GWL_STYLE,WS_OVERLAPPEDWINDOW | WS_VISIBLE );



	DebugPrint(("... Platform_Win32_Screen_D3D9_CRT destroyed\n"));
	}

__forceinline unsigned short RGB32TO16(
	unsigned int color
	)
	{
	return
		((unsigned short) (
			((color & 0x00f80000)>>8) |
			((color & 0x0000fc00)>>5) |
			((color & 0x000000f8)>>3)
		));
	}

void Platform_Win32_Screen_D3D9_CRT::ToTexture( unsigned short* data, int width, int height )
	{
	D3DLOCKED_RECT lockedRect;
	HRESULT result=S_OK;
	RECT rect;
	rect.left = 0; 
	rect.top = 0;
	rect.right = width == 320 ? 384 : width;
	rect.bottom = height == 200 ? 288 : height;
	result=texture_->LockRect(0,&lockedRect,&rect,D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE );

	if (FAILED(result)) { DebugPrint(("Failed to lock backbuffer\n")); return; }

	unsigned char* destination=static_cast<unsigned char*>(lockedRect.pBits);
	int pitch=lockedRect.Pitch;
	unsigned char* source=reinterpret_cast<unsigned char*>(data);
	int bitmapPitch=width*2;
	int dwords=width;
	
	destination += width == 320 ? 32 * 2 : 0;
	destination += height == 200 ? 44 * pitch : 0;

	if (pitch==bitmapPitch)
		{
		memcpy(destination,source,pitch*height);
		}
	else
		{
		for (int y=0; y<height; y++)
			{
			memcpy(destination,source,bitmapPitch);
			destination+=pitch;
			source+=bitmapPitch;
			}
		}
	if( crtCursorHidden_ ) goto skipCursor;
	
	unsigned short pointer[11*11] = 
		{
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,
		0x0000,0xe76d,0xe76d,0xe76d,0xe76d,0xe76d,0x0000,0xf81f,0xf81f,0xf81f,0xf81f,
		0x0000,0xc2a5,0xc2a5,0xc2a5,0xc2a5,0xe76d,0x0000,0xf81f,0xf81f,0xf81f,0xf81f,
		0x0000,0xc2a5,0xc2a5,0xc2a5,0xe76d,0x0000,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,
		0x0000,0xc2a5,0xc2a5,0xc2a5,0xc2a5,0xe76d,0x0000,0xf81f,0xf81f,0xf81f,0xf81f,
		0x0000,0xc2a5,0xc2a5,0x0000,0xc2a5,0xc2a5,0xe76d,0x0000,0xf81f,0xf81f,0xf81f,
		0xf81f,0x0000,0x0000,0xf81f,0x0000,0xc2a5,0xc2a5,0xe76d,0x0000,0xf81f,0xf81f,
		0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0x0000,0xc2a5,0xc2a5,0xe76d,0x0000,0xf81f,
		0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0x0000,0xc2a5,0xc2a5,0xe76d,0x0000,
		0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0x0000,0xc2a5,0x0000,0xf81f,
		0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0x0000,0xf81f,0xf81f,
		};

	unsigned short B = RGB32TO16( 0x0A0C0B );
	unsigned short D = RGB32TO16( 0xFCFEFC );
	unsigned short T = 0xf81f;

	unsigned short pointer2[11*16] = 
		{
		B,B,T,T,T,T,T,T,T,T,T,
		B,D,B,T,T,T,T,T,T,T,T,
		B,D,D,B,T,T,T,T,T,T,T,
		B,D,D,D,B,T,T,T,T,T,T,
		B,D,D,D,D,B,T,T,T,T,T,
		B,D,D,D,D,D,B,T,T,T,T,
		B,D,D,D,D,D,D,B,T,T,T,
		B,D,D,D,D,D,D,D,B,T,T,
		B,D,D,D,D,D,D,D,D,B,T,
		B,D,D,D,D,D,B,B,B,B,B,
		B,D,D,B,D,D,B,T,T,T,T,
		B,D,B,T,B,D,D,B,T,T,T,
		B,B,T,T,B,D,D,B,T,T,T,
		B,T,T,T,T,B,D,D,B,T,T,
		T,T,T,T,T,B,D,D,B,T,T,
		T,T,T,T,T,T,B,B,B,T,T,
		};

	POINT point;
	point.x=0;
	point.y=0;
	BOOL ret=GetCursorPos(&point);
	if (!ret)
		{
		Platform::GetPlatform_OS()->OutputDebugText("Couldn't get mouse cursor\n");
		}

	ScreenToClient(windowHandle_, &point);
	float cursorPosX=(float)point.x;
	float cursorPosY=(float)point.y;

	TransformCursorCoordinates(cursorPosX,cursorPosY);
	if( lastPresentWidth_ == 320 ) cursorPosX+=32;
	if( lastPresentHeight_ == 200 ) cursorPosY+=44;


	bool clamped = false;
	if( cursorPosX < 32) { cursorPosX = 32; clamped = true; }
	if( cursorPosY < 44) { cursorPosY = 44; clamped = true; }
	if( cursorPosX > 319+32) { cursorPosX = 319+32; clamped = true; }
	if( cursorPosY > 199+44) { cursorPosY = 199+44; clamped = true; }

	int xp = (int)(cursorPosX + 0.5f);
	int yp = (int)(cursorPosY + 0.5f);

	if( clamped && Platform::GetPlatform_OS()->HasFocus() && !IsIconic( windowHandle_ ) && GetForegroundWindow() == windowHandle_ )
		{
		if( lastPresentWidth_ == 320 ) cursorPosX-=32;
		if( lastPresentHeight_ == 200 ) cursorPosY-=44;
		InvTransformCursorCoordinates( cursorPosX, cursorPosY );
		point.x = (int) (cursorPosX + 0.5f);
		point.y = (int) (cursorPosY + 0.5f); 
		ClientToScreen(windowHandle_, &point);
		SetCursorPos( point.x, point.y );
		}

	destination=static_cast<unsigned char*>(lockedRect.pBits);
	destination+= (xp*2) + (yp*pitch );
	unsigned short* ptr = height == 200 ? pointer2 : pointer;
	for( int y = 0; y < ( height == 200 ? 16 : 11); ++y )
		{
		unsigned short* d = (unsigned short*) destination;
		for( int x = 0; x < 11; ++x )
			{
			unsigned short p = ptr[ x + y * 11 ];
			if( p != 0xf81f && x+xp >= 32 && x+xp < 320+32 && y+yp >= 44 && y+yp < 200+44) *d = p;
			++d;
			}
		destination+=pitch;
		}

skipCursor:
	result=texture_->UnlockRect(0);
	Platform::SendEvent_CustomEvent("SetCursor",0);
	SetCursor(0);
	if (FAILED(result))	{ DebugPrint(("Failed to unlock texture\n")); return ; }
	}


void Platform_Win32_Screen_D3D9_CRT::DrawQuad( float srcX1, float srcY1, float srcX2, float srcY2, float dstX1, float dstY1, float dstX2, float dstY2, unsigned int modulate )
	{
	struct Vertex
		{
		float x;
		float y;
		float z;
		float rhw;
		DWORD color;
		float u;
		float v;
		float w;
		float h;
		float bw;
		float bh;
		float time;
		float dummy;
		};

	Vertex quad[4];

	float x1=dstX1;
	float y1=dstY1;
	float x2=dstX2;
	float y2=dstY2;

	quad[0].color = modulate;
	quad[1].color = modulate;
	quad[2].color = modulate;
	quad[3].color = modulate;

	quad[0].x=x2-0.5f;
	quad[0].y=y2-0.5f;
	quad[0].z=0;
	quad[0].rhw=0.5f;
	quad[0].u=srcX2/(float)screenWidth_;
	quad[0].v=srcY2/(float)screenHeight_;	

	quad[1].x=x1-0.5f;
	quad[1].y=y2-0.5f;
	quad[1].z=0;
	quad[1].rhw=0.5f;
	quad[1].u=srcX1/(float)screenWidth_;
	quad[1].v=srcY2/(float)screenHeight_;	

	quad[2].x=x2-0.5f;
	quad[2].y=y1-0.5f;
	quad[2].z=0;
	quad[2].rhw=0.5f;
	quad[2].u=srcX2/(float)screenWidth_;
	quad[2].v=srcY1/(float)screenHeight_;

	quad[3].x=x1-0.5f;
	quad[3].y=y1-0.5f;
	quad[3].z=0;
	quad[3].rhw=0.5f;
	quad[3].u=srcX1/(float)screenWidth_;
	quad[3].v=srcY1/(float)screenHeight_;

	quad[0].w = (float)screenWidth_;
	quad[0].h = (float)screenHeight_;
	quad[1].w = (float)screenWidth_;
	quad[1].h = (float)screenHeight_;
	quad[2].w = (float)screenWidth_;
	quad[2].h = (float)screenHeight_;
	quad[3].w = (float)screenWidth_;
	quad[3].h = (float)screenHeight_;

	float width = srcX2 - srcX1;
	float height = srcY2 - srcY1;
	quad[0].bw = (float)width;
	quad[0].bh = (float)height;
	quad[1].bw = (float)width;
	quad[1].bh = (float)height;
	quad[2].bw = (float)width;
	quad[2].bh = (float)height;
	quad[3].bw = (float)width;
	quad[3].bh = (float)height;

	quad[0].time = time_;
	quad[1].time = time_;
	quad[2].time = time_;
	quad[3].time = time_;

	HRESULT result=S_OK;
	
	result=device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX4);
	if (FAILED(result))	{ DebugPrint(("SetFVF failed\n")); return;}	

	result=device_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,quad,sizeof(Vertex));
	if (FAILED(result))	{ DebugPrint(("DrawPrimitiveUP\n")); return; }
	}


void Platform_Win32_Screen_D3D9_CRT::DrawQuadBlur( float srcX1, float srcY1, float srcX2, float srcY2, float dstX1, float dstY1, float dstX2, float dstY2, bool horiz, float r )
	{
	struct Vertex
		{
		float x;
		float y;
		float z;
		float rhw;
		float u;
		float v;
		float bx;
		float by;
		};

	Vertex quad[4];

	float x1=dstX1;
	float y1=dstY1;
	float x2=dstX2;
	float y2=dstY2;

	quad[0].x=x2-0.5f;
	quad[0].y=y2-0.5f;
	quad[0].z=0;
	quad[0].rhw=0.5f;
	quad[0].u=srcX2/(float)screenWidth_;
	quad[0].v=srcY2/(float)screenHeight_;	

	quad[1].x=x1-0.5f;
	quad[1].y=y2-0.5f;
	quad[1].z=0;
	quad[1].rhw=0.5f;
	quad[1].u=srcX1/(float)screenWidth_;
	quad[1].v=srcY2/(float)screenHeight_;	

	quad[2].x=x2-0.5f;
	quad[2].y=y1-0.5f;
	quad[2].z=0;
	quad[2].rhw=0.5f;
	quad[2].u=srcX2/(float)screenWidth_;
	quad[2].v=srcY1/(float)screenHeight_;

	quad[3].x=x1-0.5f;
	quad[3].y=y1-0.5f;
	quad[3].z=0;
	quad[3].rhw=0.5f;
	quad[3].u=srcX1/(float)screenWidth_;
	quad[3].v=srcY1/(float)screenHeight_;

	quad[0].bx = r * ( horiz ? 1.0f : 0.0f ) / (float)screenWidth_;
	quad[0].by = r * ( horiz ? 0.0f : 1.0f ) / (float)screenHeight_;
	quad[1].bx = r * ( horiz ? 1.0f : 0.0f ) / (float)screenWidth_;
	quad[1].by = r * ( horiz ? 0.0f : 1.0f ) / (float)screenHeight_;
	quad[2].bx = r * ( horiz ? 1.0f : 0.0f ) / (float)screenWidth_;
	quad[2].by = r * ( horiz ? 0.0f : 1.0f ) / (float)screenHeight_;
	quad[3].bx = r * ( horiz ? 1.0f : 0.0f ) / (float)screenWidth_;
	quad[3].by = r * ( horiz ? 0.0f : 1.0f ) / (float)screenHeight_;

	HRESULT result=S_OK;
	
	result=device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX2);
	if (FAILED(result))	{ DebugPrint(("SetFVF failed\n")); return;}	

	result=device_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,quad,sizeof(Vertex));
	if (FAILED(result))	{ DebugPrint(("DrawPrimitiveUP\n")); return; }
	}


void Platform_Win32_Screen_D3D9_CRT::Accumulate( IDirect3DTexture9* target, IDirect3DTexture9* sourceA, IDirect3DTexture9* sourceB, int width, int height, unsigned int modulate )
	{
	IDirect3DSurface9* prevSurf = 0;
	device_->GetRenderTarget(0,&prevSurf);
	IDirect3DSurface9* surface = 0;
	target->GetSurfaceLevel( 0, &surface );
	device_->SetRenderTarget( 0, surface );

	device_->BeginScene();

	HRESULT result=S_OK;

	result=device_->SetTexture(0,sourceA);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	
	result=device_->SetTexture(1,sourceB);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }

	device_->SetPixelShader(accumulationShader_);
	DrawQuad(0,0, (float) width, (float) height, 0,0, (float) width, (float) height, modulate );
	device_->SetPixelShader(0);

	result=device_->EndScene();

	result=device_->SetTexture(1,0);

	device_->SetRenderTarget( 0, prevSurf);
	}

void Platform_Win32_Screen_D3D9_CRT::Blur( IDirect3DTexture9* source, IDirect3DTexture9* blurA, IDirect3DTexture9* blurB, int width, int height, float r )
	{
	IDirect3DSurface9* prevSurf = 0;
	device_->GetRenderTarget(0,&prevSurf);
	IDirect3DSurface9* surface = 0;
	blurB->GetSurfaceLevel( 0, &surface );
	device_->SetRenderTarget( 0, surface );

	device_->BeginScene();

	HRESULT result=S_OK;

	result=device_->SetTexture(0,source);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	
	device_->SetPixelShader(blurShader_);
	DrawQuadBlur( 0,0, (float) width, (float) height, 0,0, (float) width, (float) height, true, r );
	device_->SetPixelShader(0);

	result=device_->EndScene();
	result=device_->SetTexture(0,0);

	blurA->GetSurfaceLevel( 0, &surface );
	device_->SetRenderTarget( 0, surface );

	device_->BeginScene();

	result=device_->SetTexture(0,blurB);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	
	device_->SetPixelShader(blurShader_);
	DrawQuadBlur( 0,0, (float) width, (float) height, 0,0, (float) width, (float) height, false, r );
	device_->SetPixelShader(0);

	result=device_->EndScene();


	result=device_->SetTexture(0,0);

	device_->SetRenderTarget( 0, prevSurf);
	}

void Platform_Win32_Screen_D3D9_CRT::Blend( IDirect3DTexture9* target, IDirect3DTexture9* sourceA, IDirect3DTexture9* sourceB, int width, int height, unsigned int modulate )
	{
	IDirect3DSurface9* prevSurf = 0;
	device_->GetRenderTarget(0,&prevSurf);
	IDirect3DSurface9* surface = 0;
	target->GetSurfaceLevel( 0, &surface );
	device_->SetRenderTarget( 0, surface );

	device_->BeginScene();

	HRESULT result=S_OK;

	result=device_->SetTexture(0,sourceA);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	
	result=device_->SetTexture(1,sourceB);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }

	device_->SetPixelShader(blendShader_);
	DrawQuad(0,0, (float) width, (float) height, 0,0, (float) width, (float) height, modulate );
	device_->SetPixelShader(0);

	result=device_->EndScene();

	result=device_->SetTexture(1,0);

	device_->SetRenderTarget( 0, prevSurf);
	}

void Platform_Win32_Screen_D3D9_CRT::TexToTex( IDirect3DTexture9* target, IDirect3DTexture9* source, int width, int height, unsigned int modulate )
	{
	IDirect3DSurface9* prevSurf = 0;
	device_->GetRenderTarget(0,&prevSurf);
	IDirect3DSurface9* surface = 0;
	target->GetSurfaceLevel( 0, &surface );
	device_->SetRenderTarget( 0, surface );

	device_->BeginScene();

	HRESULT result=S_OK;

	result=device_->SetTexture(0,source);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	
	DrawQuad(0,0, (float) width, (float) height, 0,0, (float) width, (float) height, modulate );

	result=device_->EndScene();

	device_->SetRenderTarget( 0, prevSurf);
	}


void Platform_Win32_Screen_D3D9_CRT::ToScreen( IDirect3DTexture9* texture, IDirect3DTexture9* blur, int width, int height, unsigned int modulate )
	{
	HRESULT result=S_OK;

	result=device_->SetTexture(0,texture);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	result=device_->SetTexture(1,blur);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	result=device_->SetTexture(2,frame_);
	if (FAILED(result))	{ DebugPrint(("SetTexture failed\n")); return; }
	
	DrawQuad(0,0, (float) width, (float) height, 0,0, (float) screenWidth_, (float) screenHeight_, modulate);
	}


//*** Present ***

bool Platform_Win32_Screen_D3D9_CRT::Present(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulateIn, unsigned short backgroundColor)
	{
	time_+=1.5f * 1.0f/60.0f;
	unsigned int color=RGB16TO32(backgroundColor);
	unsigned int modulate=RGB16TO32(modulateIn);

	lastPresentWidth_ = bitmapWidth;
	lastPresentHeight_ = bitmapHeight;
	
	HRESULT result=S_OK;

	result=device_->Clear( 0, 0, D3DCLEAR_TARGET, color, 0, 0);
	if (FAILED(result))	{ DebugPrint(("Clear failed\n")); return false;	}

	ToTexture( bitmapData, bitmapWidth, bitmapHeight );
	bitmapWidth = bitmapWidth == 320 ? 384 : bitmapWidth;
	bitmapHeight = bitmapHeight == 200 ? 288 : bitmapHeight;
	Blur( accumulationB_, blurA_, blurB_, bitmapWidth, bitmapHeight, 1.0f );
	Accumulate( accumulationA_, texture_, blurA_, bitmapWidth, bitmapHeight, modulate );
	TexToTex( accumulationB_, accumulationA_, bitmapWidth, bitmapHeight, 0xffffffff );
	Blend( accumulationA_, texture_, accumulationB_, bitmapWidth, bitmapHeight, modulate );
	Blur( accumulationA_, accumulationA_, blurB_, bitmapWidth, bitmapHeight, 0.17f );
	Blur( accumulationA_, blurA_, blurB_, bitmapWidth, bitmapHeight, 1.0f );

	result=device_->BeginScene();
	if (FAILED(result)) { DebugPrint(("BeginScene failed\n")); return false; }

	device_->SetPixelShader(crtShader_);
	ToScreen( accumulationA_, blurA_, bitmapWidth, bitmapHeight, 0xffffffff );
	device_->SetPixelShader(0);

	result=device_->EndScene();
	if (FAILED(result)) { DebugPrint(("EndScene failed\n")); return false; }	
	
	result=device_->Present(0, 0, 0, 0 );
	if (FAILED(result))	{ DebugPrint(("Present failed\n")); return false; }

	return true;
	}


//*** TransformCursorCoordinates ***

void Platform_Win32_Screen_D3D9_CRT::TransformCursorCoordinates(float& x, float& y)
	{
	if (lastPresentWidth_==0 || lastPresentHeight_==0)
		{
		return;
		}
	int w = lastPresentWidth_ == 320 ? 384 : lastPresentWidth_;
	int h = lastPresentHeight_ == 200 ? 288 : lastPresentHeight_;

	float hscale=screenWidth_/(float)w;
	float vscale=screenHeight_/(float)h;
	float pixelScale=min(hscale,vscale);

	float hborder=(screenWidth_-pixelScale*w)/2;
	float vborder=(screenHeight_-pixelScale*h)/2;

	x=(x-hborder)/pixelScale;
	y=(y-vborder)/pixelScale;

	x=x*1.025f -0.0125f;
	y=y*0.92f+0.04f;
	}

void Platform_Win32_Screen_D3D9_CRT::InvTransformCursorCoordinates(float& x, float& y)
	{
	if (lastPresentWidth_==0 || lastPresentHeight_==0)
		{
		return;
		}
	int w = lastPresentWidth_ == 320 ? 384 : lastPresentWidth_;
	int h = lastPresentHeight_ == 200 ? 288 : lastPresentHeight_;

	float hscale=screenWidth_/(float)w;
	float vscale=screenHeight_/(float)h;
	float pixelScale=min(hscale,vscale);

	float hborder=(screenWidth_-pixelScale*w)/2;
	float vborder=(screenHeight_-pixelScale*h)/2;

	x=(x+0.0125f)/1.025f;
	y=(y+0.04f)/0.92f;
	x=x*pixelScale+hborder;
	y=y*pixelScale+vborder;
	}

void Platform_Win32_Screen_D3D9_CRT::CreateFrame()
	{
	unsigned int* frame = new unsigned int[ 1024 * 1024 * 2];
	unsigned char* rle = CRT_frame_color;
	unsigned int* f = frame;
	int x = 0;
	int y = 0;
	for( int i = 0; i < sizeof( CRT_frame_color ) / sizeof( CRT_frame_color[ 0 ] ) / 2; ++i )
		{
		unsigned char count = *( rle++ );
		unsigned char value = *( rle++ );
		for( int j = 0; j < count; ++j )
			{
			if( x == 0 || x == 1023 || y == 0 || y == 1023 )
				*( f++ ) = 0x0;
			else
				*( f++ ) = value | value << 8 | value << 16;

			x = ( x+1 ) & 1023;
			if( x == 0 ) ++y;				
			}
		}
	rle = CRT_frame_alpha;
	f = frame;
	x = 0;
	y = 0;
	for( int i = 0; i < sizeof( CRT_frame_alpha ) / sizeof( CRT_frame_alpha[ 0 ] ) / 2; ++i )
		{
		unsigned char count = *( rle++ );
		unsigned char value = *( rle++ );
		for( int j = 0; j < count; ++j )
			{			
			if( x == 0 || x == 1023 || y == 0 || y == 1023 )
				*( f++ ) |= 0xff << 24;
			else
				*( f++ ) |= value << 24;

			x = ( x+1 ) & 1023;
			if( x == 0 ) ++y;				
			}
		}

	HRESULT result=device_->CreateTexture(1024,1024,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&frame_,0);
	if (FAILED(result)) { DebugPrint(("Couldn't create frame\n")); return ;	}			


	D3DLOCKED_RECT lockedRect;
	result=S_OK;
	RECT rect;
	rect.top=0; 
	rect.left=0;
	rect.right=1024;
	rect.bottom=1024;
	result=frame_->LockRect(0,&lockedRect,&rect,D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE );

	if (FAILED(result)) { DebugPrint(("Failed to lock frame\n")); return; }

	unsigned char* destination=static_cast<unsigned char*>(lockedRect.pBits);
	int pitch=lockedRect.Pitch;
	unsigned char* source=reinterpret_cast<unsigned char*>(frame);
	int bitmapPitch=1024*4;


	if (pitch==bitmapPitch)
		{
		memcpy(destination,source,pitch*1024);
		}
	else
		{
		for (int y=0; y<1024; y++)
			{
			memcpy(destination,source,bitmapPitch);
			destination+=pitch;
			source+=bitmapPitch;
			}
		}


	result=frame_->UnlockRect(0);
	if (FAILED(result))	{ DebugPrint(("Failed to unlock texture\n")); return ; }
	delete[] frame;
	}


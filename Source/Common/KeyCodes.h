/**
 * \enum	KeyCode
 * 
 * \ingroup	platform
 * \brief	Virtual key codes
 * \author	Mattias Gustavsson	
 * 
 * The engine uses virtual key codes to map physical keys to a platform
 * independent virtual keys
 *
 * \todo	Standardized keycodes (multiplatform)
 */

#ifndef __KeyCodes_H__
#define __KeyCodes_H__

// Includes

// Forward declares

// Keycodes
enum KeyCode
	{
	KEY_INVALID = 0x00, ///< Not a valid keycode
	KEY_LBUTTON	= 0x01, ///< Left mouse button
	KEY_RBUTTON	= 0x02, ///< Right mouse button
	KEY_CANCEL	= 0x03, ///< Control-break processing  
	KEY_MBUTTON = 0x04, 
	KEY_XBUTTON1 = 0x05, 
	KEY_XBUTTON2 = 0x06,

	// 07 Undefined  

	KEY_BACK	= 0x08, ///< BACKSPACE key  
	KEY_TAB		= 0x09, ///< TAB key  

	// 0A-0B Undefined  

	KEY_CLEAR	= 0x0C, ///< CLEAR key  
	KEY_RETURN	= 0x0D, ///< ENTER key  

	// 0E-0F Undefined  

	KEY_SHIFT	= 0x10, ///< SHIFT key  
	KEY_CONTROL	= 0x11, ///< CTRL key  
	KEY_MENU	= 0x12, ///< ALT key  
	KEY_PAUSE	= 0x13,
	KEY_CAPITAL	= 0x14, ///< CAPS LOCK key  

	KEY_KANA	= 0x15, 
	KEY_HANGUL	= 0x15, 
	KEY_JUNJA	= 0x17, 
	KEY_FINAL	= 0x18, 
	KEY_HANJA	= 0x19, 
	KEY_KANJI	= 0x19, 

	// 1A Undefined  

	KEY_ESCAPE		= 0x1B, ///< ESC key  
	KEY_CONVERT		= 0x1C, 
	KEY_NONCONVERT	= 0x1D,
	KEY_ACCEPT		= 0x1E, 
	KEY_MODECHANGE	= 0x1F, 

	KEY_SPACE	= 0x20, ///< SPACEBAR key 
	KEY_PRIOR	= 0x21, ///< PAGE UP key  
	KEY_NEXT	= 0x22, ///< PAGE DOWN key  
	KEY_END		= 0x23, ///< END key  
	KEY_HOME	= 0x24, ///< HOME key  
	KEY_LEFT	= 0x25, ///< LEFT ARROW key  
	KEY_UP		= 0x26, ///< UP ARROW key  
	KEY_RIGHT	= 0x27, ///< RIGHT ARROW key  
	KEY_DOWN	= 0x28, ///< DOWN ARROW key  
	KEY_SELECT	= 0x29, ///< SELECT key  

	KEY_PRINT	= 0x2A, 

	KEY_EXEC	= 0x2B, ///< EXECUTE key  
	KEY_SNAPSHOT= 0x2C, ///< PRINT SCREEN key for Windows 3.0 and later  
	KEY_INSERT	= 0x2D, 
	KEY_DELETE  = 0x2E, ///< Delete
	KEY_HELP	= 0x2F, ///< HELP key  
	KEY_0		= 0x30, ///< 0 key  
	KEY_1		= 0x31, ///< 1 key  
	KEY_2		= 0x32, ///< 2 key  
	KEY_3		= 0x33, ///< 3 key  
	KEY_4		= 0x34, ///< 4 key  
	KEY_5		= 0x35, ///< 5 key  
	KEY_6		= 0x36, ///< 6 key  
	KEY_7		= 0x37, ///< 7 key  
	KEY_8		= 0x38, ///< 8 key  
	KEY_9		= 0x39, ///< 9 key  

	// 3A-40 Undefined  

	KEY_A		= 0x41, ///< A key  
	KEY_B		= 0x42, ///< B key  
	KEY_C		= 0x43, ///< C key  
	KEY_D		= 0x44, ///< D key  
	KEY_E		= 0x45, ///< E key  
	KEY_F		= 0x46, ///< F key  
	KEY_G		= 0x47, ///< G key  
	KEY_H		= 0x48, ///< H key  
	KEY_I		= 0x49, ///< I key  
	KEY_J		= 0x4A, ///< J key  
	KEY_K		= 0x4B, ///< K key  
	KEY_L		= 0x4C, ///< L key  
	KEY_M		= 0x4D, ///< M key  
	KEY_N		= 0x4E, ///< N key  
	KEY_O		= 0x4F, ///< O key  
	KEY_P		= 0x50, ///< P key  
	KEY_Q		= 0x51, ///< Q key  
	KEY_R		= 0x52, ///< R key  
	KEY_S		= 0x53, ///< S key  
	KEY_T		= 0x54, ///< T key  
	KEY_U		= 0x55, ///< U key  
	KEY_V		= 0x56, ///< V key  
	KEY_W		= 0x57, ///< W key  
	KEY_X		= 0x58, ///< X key  
	KEY_Y		= 0x59, ///< Y key  
	KEY_Z		= 0x5A, ///< Z key  
	
	KEY_LWIN	= 0x5B,   
	KEY_RWIN	= 0x5C,   
	KEY_APPS	= 0x5D,   
	KEY_SLEEP	= 0x5F, 

	// 5F Undefined  

	KEY_NUMPAD0	= 0x60, ///< Numeric keypad 0 key  
	KEY_NUMPAD1	= 0x61, ///< Numeric keypad 1 key  
	KEY_NUMPAD2	= 0x62, ///< Numeric keypad 2 key  
	KEY_NUMPAD3	= 0x63, ///< Numeric keypad 3 key  
	KEY_NUMPAD4	= 0x64, ///< Numeric keypad 4 key  
	KEY_NUMPAD5	= 0x65, ///< Numeric keypad 5 key  
	KEY_NUMPAD6	= 0x66, ///< Numeric keypad 6 key  
	KEY_NUMPAD7	= 0x67, ///< Numeric keypad 7 key  
	KEY_NUMPAD8	= 0x68, ///< Numeric keypad 8 key  
	KEY_NUMPAD9	= 0x69, ///< Numeric keypad 9 key  
	KEY_MULTIPLY= 0x6A, ///< Asterisk (*) key  
	KEY_ADD		= 0x6B, ///< Plus sign (+) key  
	KEY_SEPARATOR=0x6C, ///< Separator key  
	KEY_SUBTRACT= 0x6D, ///< Minus sign (-) key  
	KEY_DECIMAL	= 0x6E, ///< Period (.) key  
	KEY_DIVIDE	= 0x6F, ///< Slash mark (/) key  
	
	KEY_F1		= 0x70, ///< F1
	KEY_F2		= 0x71, ///< F2
	KEY_F3		= 0x72, ///< F3
	KEY_F4		= 0x73, ///< F4
	KEY_F5		= 0x74, ///< F5
	KEY_F6		= 0x75, ///< F6
	KEY_F7		= 0x76, ///< F7
	KEY_F8		= 0x77, ///< F8
	KEY_F9		= 0x78, ///< F9
	KEY_F10		= 0x79, ///< F10
	KEY_F11		= 0x7A, ///< F11
	KEY_F12		= 0x7B, ///< F12
	KEY_F13		= 0x7C, 
	KEY_F14		= 0x7D,  
	KEY_F15		= 0x7E, 
	KEY_F16		= 0x7F,  
	KEY_F17		= 0x80, 
	KEY_F18		= 0x81, 
	KEY_F19		= 0x82, 
	KEY_F20		= 0x83, 
	KEY_F21		= 0x84, 
	KEY_F22		= 0x85, 
	KEY_F23		= 0x86, 
	KEY_F24		= 0x87, 

	// 88-8F Unassigned  

	KEY_NUMLOCK	= 0x90, 
	KEY_SCROLL	= 0x91,

	// 92-9F Unassigned  

	KEY_LSHIFT	= 0xA0,   
	KEY_RSHIFT	= 0xA1,
	KEY_LCONTROL= 0xA2,  
	KEY_RCONTROL= 0xA3,
	KEY_LMENU	= 0xA4,  
	KEY_RMENU	= 0xA5,

	KEY_BROWSER_BACK			= 0xA6,
	KEY_BROWSER_FORWARD		= 0xA7,
	KEY_BROWSER_REFRESH		= 0xA8, 
	KEY_BROWSER_STOP			= 0xA9, 
	KEY_BROWSER_SEARCH		= 0xAA, 
	KEY_BROWSER_FAVORITES	= 0xAB, 
	KEY_BROWSER_HOME			= 0xAC, 

	KEY_VOLUME_MUTE	= 0xAD, 
	KEY_VOLUME_DOWN	= 0xAE, 
	KEY_VOLUME_UP	= 0xAF, 

	KEY_MEDIA_NEXT_TRACK	= 0xB0,
	KEY_MEDIA_PREV_TRACK	= 0xB1,
	KEY_MEDIA_STOP		= 0xB2, 
	KEY_MEDIA_PLAY_PAUSE = 0xB3, 

	KEY_LAUNCH_MAIL			= 0xB4, 
	KEY_LAUNCH_MEDIA_SELECT	= 0xB5, 
	KEY_LAUNCH_APP1			= 0xB6, 
	KEY_LAUNCH_APP2			= 0xB7, 

	KEY_OEM_1 = 0xBA, 
	KEY_OEM_PLUS = 0xBB,
	KEY_OEM_COMMA = 0xBC, 
	KEY_OEM_MINUS = 0xBD, 
	KEY_OEM_PERIOD = 0xBE, 
	KEY_OEM_2 = 0xBF, 
	KEY_OEM_3 = 0xC0, 

	// C1-DA Unassigned  

	KEY_OEM_4 = 0xDB, 
	KEY_OEM_5 = 0xDC,
	KEY_OEM_6 = 0xDD, 
	KEY_OEM_7 = 0xDE, 
	KEY_OEM_8 = 0xDF, 

	KEY_OEM_102 = 0xE2, 
	KEY_PROCESSKEY = 0xE5, 
	KEY_PACKET = 0xE7, 

	// E9-F5 OEM-specific  

	KEY_ATTN	= 0xF6,   
	KEY_CRSEL	= 0xF7,   
	KEY_EXSEL	= 0xF8,   
	KEY_EREOF	= 0xF9,   
	KEY_PLAY	= 0xFA,   
	KEY_ZOOM	= 0xFB,   
	KEY_NONAME	= 0xFC,
	KEY_PA1		= 0xFD,

	KEY_OEM_CLEAR = 0xFE,
	
	KEYCODE_COUNT = 0xFF
	};


#endif /* __Keycodes_H__ */

	

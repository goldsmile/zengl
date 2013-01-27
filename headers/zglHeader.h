/*--------------------------------*/
/*-----------= ZenGL =------------*/
/*--------------------------------*/
/*                                */
/* version:  0.3.7                */
/* date:     2013.01.27           */
/* license:  zlib                 */
/* homepage: http://zengl.org     */
/*                                */
/*-------- developed by: ---------*/
/*                                */
/*     Andrey Kemka aka Andru     */
/*                                */
/* mail:  dr.andru@gmail.com      */
/* JID:   dr.andru@googlemail.com */
/* ICQ:   496929849               */
/* Skype: kemka.andrey            */
/*                                */
/*--------------------------------*/

/*
 *  Copyright (c) 2012 Andrey Kemka
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *     you must not claim that you wrote the original software.
 *     If you use this software in a product, an acknowledgment
 *     in the product documentation would be appreciated but
 *     is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *     and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *     source distribution.
*/
#ifndef __ZGLHEADER__
#define __ZGLHEADER__

#pragma pack(push,8)

#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
  #include <dlfcn.h>
  #define __LINUX__

  #define sprintf_s sprintf
#endif
#if ( defined _WIN32 || defined _WIN64 )
  #include <windows.h>
  #define __WINDOWS__

  #if ( defined __MINGW32__ || defined __MINGW64__ )
    #define sprintf_s sprintf
  #endif
#endif

#ifndef ZGL_IMPORT
  #define ZGLEXTERN extern
#else
  #define ZGLEXTERN
#endif

#define FALSE 0
#define TRUE 1

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
#ifndef __cplusplus
typedef unsigned char bool;
#else
#define __CPP__
#endif

typedef struct
{
  int    Count;
  char **Items;
} zglTStringList;

#ifdef __WINDOWS__
typedef long zglTFile;
#else
typedef uint zglTFile;
#endif
typedef zglTStringList zglTFileList;
typedef struct
{
  void *Memory;
  uint  Size;
  uint  Position;
} zglTMemory, *zglPMemory;

#ifdef __CPP__
ZGLEXTERN void ( *__zgl_Init )( byte FSAA, byte StencilBits );
static inline void zgl_Init( byte FSAA = 0, byte StencilBits = 0 )
{
  __zgl_Init( FSAA, StencilBits );
}
#else
ZGLEXTERN void ( *zgl_Init )( byte FSAA, byte StencilBits );
#endif
ZGLEXTERN void ( *zgl_Exit )();

#define SYS_APP_INIT           0x000001
#define SYS_APP_LOOP           0x000002
#define SYS_LOAD               0x000003
#define SYS_DRAW               0x000004
#define SYS_UPDATE             0x000005
#define SYS_EXIT               0x000006
#define SYS_ACTIVATE           0x000007
#define SYS_CLOSE_QUERY        0x000008

#define INPUT_MOUSE_MOVE       0x000020
#define INPUT_MOUSE_PRESS      0x000021
#define INPUT_MOUSE_RELEASE    0x000022
#define INPUT_MOUSE_WHEEL      0x000023
#define INPUT_KEY_PRESS        0x000030
#define INPUT_KEY_RELEASE      0x000031
#define INPUT_KEY_CHAR         0x000032

#define TEX_FORMAT_EXTENSION   0x000100
#define TEX_FORMAT_FILE_LOADER 0x000101
#define TEX_FORMAT_MEM_LOADER  0x000102
#define TEX_CURRENT_EFFECT     0x000103

#define SND_FORMAT_EXTENSION   0x000110
#define SND_FORMAT_FILE_LOADER 0x000111
#define SND_FORMAT_MEM_LOADER  0x000112
#define SND_FORMAT_DECODER     0x000113

#define VIDEO_FORMAT_DECODER   0x000130

ZGLEXTERN void ( *zgl_Reg )( uint What, void *UserData );

#define ZENGL_VERSION           1 // Major shr 16, ( Minor and $FF00 ) shr 8, Revision and $FF
#define ZENGL_VERSION_STRING    2 // PChar
#define ZENGL_VERSION_DATE      3 // PChar

#define DIRECTORY_APPLICATION   101 // PChar
#define DIRECTORY_HOME          102 // PChar

#define LOG_FILENAME            203 // PPAnsiChar

#define DESKTOP_WIDTH           300
#define DESKTOP_HEIGHT          301
#define RESOLUTION_LIST         302 // zglPResolutionList

#define WINDOW_HANDLE           400 // TWindow(GNU/Linux), HWND(Windows), WindowRef(MacOS X)
#define WINDOW_X                401
#define WINDOW_Y                402
#define WINDOW_WIDTH            403
#define WINDOW_HEIGHT           404

#define GAPI_CONTEXT            500 // GLXContext(GNU/Linux), HGLRC(Windows), TAGLContext(MacOS X)
#define GAPI_DEVICE             500 // For ZenGL with Direct3D render only
#define GAPI_MAX_TEXTURE_SIZE   501
#define GAPI_MAX_TEXTURE_UNITS  502
#define GAPI_MAX_ANISOTROPY     503
#define GAPI_CAN_BLEND_SEPARATE 504 // Boolean
#define GAPI_CAN_AUTOGEN_MIPMAP 505 // Boolean

#define VIEWPORT_WIDTH          600
#define VIEWPORT_HEIGHT         601
#define VIEWPORT_OFFSET_X       602
#define VIEWPORT_OFFSET_Y       603

#define RENDER_FPS              700
#define RENDER_BATCHES_2D       701
#define RENDER_CURRENT_MODE     702
#define RENDER_CURRENT_TARGET   703
#define RENDER_VRAM_USED        704

#define MANAGER_TIMER           800 // zglPTimerManager
#define MANAGER_TEXTURE         801 // zglPTextureManager
#define MANAGER_FONT            802 // zglPFontManager
#define MANAGER_RTARGET         803 // zglPRenderTargetManager
#define MANAGER_SOUND           804 // zglPSoundManager
#define MANAGER_EMITTER2D       805 // zglPEmitter2DManager

ZGLEXTERN size_t ( *zgl_Get )( uint What );
ZGLEXTERN void ( *zgl_GetMem )( void **Mem, uint Size );
ZGLEXTERN void ( *zgl_FreeMem )( void **Mem );
//ZGLEXTERN void ( zgl_FreeStrList )( zglTStringList* List );

#define COLOR_BUFFER_CLEAR    0x000001
#define DEPTH_BUFFER          0x000002
#define DEPTH_BUFFER_CLEAR    0x000004
#define DEPTH_MASK            0x000008
#define STENCIL_BUFFER_CLEAR  0x000010
#define CORRECT_RESOLUTION    0x000020
#define CORRECT_WIDTH         0x000040
#define CORRECT_HEIGHT        0x000080
#define APP_USE_AUTOPAUSE     0x000100
#define APP_USE_LOG           0x000200
#define APP_USE_ENGLISH_INPUT 0x000400
#define APP_USE_DT_CORRECTION 0x000800
#define WND_USE_AUTOCENTER    0x000100
#define SND_CAN_PLAY          0x002000
#define SND_CAN_PLAY_FILE     0x004000
#define CLIP_INVISIBLE        0x008000

ZGLEXTERN void ( *zgl_Enable )( uint What );
ZGLEXTERN void ( *zgl_Disable )( uint What );

// LOG
#ifdef __CPP__
ZGLEXTERN void ( *__log_Add )( const char *Message, bool Timings );
static inline void log_Add( const char *Message, bool Timings = TRUE )
{
  __log_Add( Message, Timings );
}
#else
ZGLEXTERN void ( *log_Add )( const char *Message, bool Timings );
#endif

// WINDOW
ZGLEXTERN void ( *wnd_SetCaption )( const char *NewCaption );
ZGLEXTERN void ( *wnd_SetSize )( int Width, int Height );
ZGLEXTERN void ( *wnd_SetPos )( int X, int Y );
ZGLEXTERN void ( *wnd_ShowCursor )( bool Show );

// SCREEN
typedef struct
{
  int Count;
  int* Width;
  int* Height;
} zglTResolutionList, *zglPResolutionList;

#define REFRESH_MAXIMUM 0
#define REFRESH_DEFAULT 1

ZGLEXTERN void ( *scr_Clear )();
ZGLEXTERN void ( *scr_Flush )();
ZGLEXTERN void ( *scr_SetVSync )( bool VSync );
// RU: ВНИМАНИЕ: Функция уничтожает контекст OpenGL, что потребует перезагрузку ресурсов
// EN: WARNING: Function will destroy OpenGL context, so all resources must be reloaded
ZGLEXTERN void ( *scr_SetFSAA )( byte FSAA );
ZGLEXTERN bool ( *scr_SetOptions )( ushort Width, ushort Height, ushort Refresh, bool FullScreen, bool VSync );
ZGLEXTERN void ( *scr_CorrectResolution )( ushort Width, ushort Height );
ZGLEXTERN void ( *scr_ReadPixels )( void **pData, ushort X, ushort Y, ushort Width, ushort Height );

// GL
#define TARGET_SCREEN  1
#define TARGET_TEXTURE 2

ZGLEXTERN void ( *Set2DMode )();
#ifdef __CPP__
ZGLEXTERN void ( *__Set3DMode )( float FOVY );
static inline void Set3DMode( float FOVY = 45 )
{
  __Set3DMode( FOVY );
}
#else
ZGLEXTERN void ( *Set3DMode )( float FOVY );
#endif

// Z BUFFER
ZGLEXTERN void ( *zbuffer_SetDepth )( float zNear, float zFar );
ZGLEXTERN void ( *zbuffer_Clear )();

// SCISSOR
#ifdef __CPP__
ZGLEXTERN void ( *__scissor_Begin )( int X, int Y, int Width, int Height, bool ConsiderCamera );
static inline void scissor_Begin( int X, int Y, int Width, int Height, bool ConsiderCamera = TRUE )
{
  __scissor_Begin( X, Y, Width, Height, ConsiderCamera );
}
#else
ZGLEXTERN void ( *scissor_Begin )( int X, int Y, int Width, int Height, bool ConsiderCamera );
#endif
ZGLEXTERN void ( *scissor_End )();

// INI
/*
  ini_LoadFromFile  : function( const FileName : String ) : Boolean;
  ini_SaveToFile    : procedure( const FileName : String );
  ini_Add           : procedure( const Section, Key : AnsiString );
  ini_Del           : procedure( const Section, Key : AnsiString );
  ini_Clear         : procedure( const Section : AnsiString );
  ini_IsSection     : function( const Section : AnsiString ) : Boolean;
  ini_IsKey         : function( const Section, Key : AnsiString ) : Boolean;
  _ini_ReadKeyStr   : function( const Section, Key : AnsiString ) : PAnsiChar;
  ini_ReadKeyInt    : function( const Section, Key : AnsiString ) : Integer;
  ini_ReadKeyFloat  : function( const Section, Key : AnsiString ) : Single;
  ini_ReadKeyBool   : function( const Section, Key : AnsiString ) : Boolean;
  ini_WriteKeyStr   : function( const Section, Key, Value : AnsiString ) : Boolean;
  ini_WriteKeyInt   : function( const Section, Key : AnsiString; Value : Integer ) : Boolean;
  ini_WriteKeyFloat : function( const Section, Key : AnsiString; Value : Single; Digits : Integer = 2 ) : Boolean;
  ini_WriteKeyBool  : function( const Section, Key : AnsiString; Value : Boolean ) : Boolean;

  function ini_ReadKeyStr( const Section, Key : AnsiString ) : AnsiString;
*/

// TIMERS
typedef struct
{
  bool   Active;
  bool   Custom;
  void   *UserData;
  uint   Interval;
  double LastTick;
  void*  OnTimer;
  void   ( *OnTimerEx )( void *Timer );

  void*  Prev;
  void*  Next;
} zglTTimer, *zglPTimer;

typedef struct
{
  int       Count;
  zglTTimer First;
} zglTTimerManager, *zglPTimerManager;

#ifdef __CPP__
ZGLEXTERN zglPTimer ( *__timer_Add )( void *OnTimer, uint Interval, bool UseSenderForCallback, void *UserData );
static inline void timer_Add( void *OnTimer, uint Interval, bool UseSenderForCallback = FALSE, void *UserData = NULL )
{
  __timer_Add( OnTimer, Interval, UseSenderForCallback, UserData );
}
#else
ZGLEXTERN zglPTimer ( *timer_Add )( void *OnTimer, uint Interval, bool UseSenderForCallback, void *UserData );
#endif
ZGLEXTERN void ( *timer_Del )( zglPTimer *Timer );
ZGLEXTERN double ( *timer_GetTicks )();
ZGLEXTERN void ( *timer_Reset )();

// MOUSE
#define M_BLEFT   0
#define M_BMIDDLE 1
#define M_BRIGHT  2
#define M_WUP     0
#define M_WDOWN   1

ZGLEXTERN int ( *mouse_X )();
ZGLEXTERN int ( *mouse_Y )();
ZGLEXTERN int ( *mouse_DX )();
ZGLEXTERN int ( *mouse_DY )();
ZGLEXTERN bool ( *mouse_Down )( byte Button );
ZGLEXTERN bool ( *mouse_Up )( byte Button );
ZGLEXTERN bool ( *mouse_Click )( byte Button );
ZGLEXTERN bool ( *mouse_DblClick )( byte Button );
ZGLEXTERN bool ( *mouse_Wheel )( byte Axis );
ZGLEXTERN void ( *mouse_ClearState )();
#ifdef __CPP__
ZGLEXTERN void ( *__mouse_Lock )( int X, int Y );
static inline void mouse_Lock( int X = -1, int Y = -1 )
{
  __mouse_Lock( X, Y );
}
#else
ZGLEXTERN void ( *mouse_Lock )( int X, int Y );
#endif

// KEYBOARD
#define K_SYSRQ      0xB7
#define K_PAUSE      0xC5
#define K_ESCAPE     0x01
#define K_ENTER      0x1C
#define K_KP_ENTER   0x9C

#define K_UP         0xC8
#define K_DOWN       0xD0
#define K_LEFT       0xCB
#define K_RIGHT      0xCD

#define K_BACKSPACE  0x0E
#define K_SPACE      0x39
#define K_TAB        0x0F
#define K_TILDE      0x29

#define K_INSERT     0xD2
#define K_DELETE     0xD3
#define K_HOME       0xC7
#define K_END        0xCF
#define K_PAGEUP     0xC9
#define K_PAGEDOWN   0xD1

#define K_CTRL       0xFF - 0x01
#define K_CTRL_L     0x1D
#define K_CTRL_R     0x9D
#define K_ALT        0xFF - 0x02
#define K_ALT_L      0x38
#define K_ALT_R      0xB8
#define K_SHIFT      0xFF - 0x03
#define K_SHIFT_L    0x2A
#define K_SHIFT_R    0x36
#define K_SUPER      0xFF - 0x04
#define K_SUPER_L    0xDB
#define K_SUPER_R    0xDC
#define K_APP_MENU   0xDD

#define K_CAPSLOCK   0x3A
#define K_NUMLOCK    0x45
#define K_SCROLL     0x46

#define K_BRACKET_L  0x1A // "[" or "{"
#define K_BRACKET_R  0x1B // "]" or "}"
#define K_BACKSLASH  0x2B // "\"
#define K_SLASH      0x35 // "/"
#define K_COMMA      0x33 // ","
#define K_DECIMAL    0x34 // "."
#define K_SEMICOLON  0x27 // ":" or ";"
#define K_APOSTROPHE 0x28 // ' or "

#define K_0          0x0B
#define K_1          0x02
#define K_2          0x03
#define K_3          0x04
#define K_4          0x05
#define K_5          0x06
#define K_6          0x07
#define K_7          0x08
#define K_8          0x09
#define K_9          0x0A

#define K_MINUS      0x0C
#define K_EQUALS     0x0D

#define K_A          0x1E
#define K_B          0x30
#define K_C          0x2E
#define K_D          0x20
#define K_E          0x12
#define K_F          0x21
#define K_G          0x22
#define K_H          0x23
#define K_I          0x17
#define K_J          0x24
#define K_K          0x25
#define K_L          0x26
#define K_M          0x32
#define K_N          0x31
#define K_O          0x18
#define K_P          0x19
#define K_Q          0x10
#define K_R          0x13
#define K_S          0x1F
#define K_T          0x14
#define K_U          0x16
#define K_V          0x2F
#define K_W          0x11
#define K_X          0x2D
#define K_Y          0x15
#define K_Z          0x2C

#define K_KP_0       0x52
#define K_KP_1       0x4F
#define K_KP_2       0x50
#define K_KP_3       0x51
#define K_KP_4       0x4B
#define K_KP_5       0x4C
#define K_KP_6       0x4D
#define K_KP_7       0x47
#define K_KP_8       0x48
#define K_KP_9       0x49

#define K_KP_SUB     0x4A
#define K_KP_ADD     0x4E
#define K_KP_MUL     0x37
#define K_KP_DIV     0xB5
#define K_KP_DECIMAL 0x53

#define K_F1         0x3B
#define K_F2         0x3C
#define K_F3         0x3D
#define K_F4         0x3E
#define K_F5         0x3F
#define K_F6         0x40
#define K_F7         0x41
#define K_F8         0x42
#define K_F9         0x43
#define K_F10        0x44
#define K_F11        0x57
#define K_F12        0x58

#define KA_DOWN      0
#define KA_UP        1

ZGLEXTERN bool ( *key_Down )( byte KeyCode );
ZGLEXTERN bool ( *key_Up )( byte KeyCode );
ZGLEXTERN bool ( *key_Press )( byte KeyCode );
ZGLEXTERN byte ( *key_Last )( byte KeyAction );
#ifdef __CPP__
ZGLEXTERN void ( *__key_BeginReadText )( const char *Text, int MaxSymbols );
static inline void key_BeginReadText( const char *Text, int MaxSymbols = -1 )
{
  __key_BeginReadText( Text, MaxSymbols );
}
ZGLEXTERN void ( *__key_UpdateReadText )( const char *Text, int MaxSymbols );
static inline void key_UpdateReadText( const char *Text, int MaxSymbols = -1 )
{
  __key_UpdateReadText( Text, MaxSymbols );
}
#else
ZGLEXTERN void ( *key_BeginReadText )( const char *Text, int MaxSymbols );
ZGLEXTERN void ( *key_UpdateReadText )( const char *Text, int MaxSymbols );
#endif
ZGLEXTERN const char * ( *key_GetText )();
ZGLEXTERN void ( *key_EndReadText )();
ZGLEXTERN void ( *key_ClearState )();

// JOYSTICK
typedef struct
{
  char* Name;

  struct
  {
    int Axes;
    int Buttons;
  } Count;

  uint Caps;
} zglTJoyInfo, *zglPJoyInfo;

#define JOY_HAS_Z   0x000001
#define JOY_HAS_R   0x000002
#define JOY_HAS_U   0x000004
#define JOY_HAS_V   0x000008
#define JOY_HAS_POV 0x000010

#define JOY_AXIS_X 0
#define JOY_AXIS_Y 1
#define JOY_AXIS_Z 2
#define JOY_AXIS_R 3
#define JOY_AXIS_U 4
#define JOY_AXIS_V 5
#define JOY_POVX   6
#define JOY_POVY   7

ZGLEXTERN byte ( *joy_Init )();
ZGLEXTERN zglPJoyInfo ( *joy_GetInfo )( byte JoyID );
ZGLEXTERN float ( *joy_AxisPos )( byte JoyID, byte Axis );
ZGLEXTERN bool ( *joy_Down )( byte JoyID, byte Button );
ZGLEXTERN bool ( *joy_Up )( byte JoyID, byte Button );
ZGLEXTERN bool ( *joy_Press )( byte JoyID, byte Button );
ZGLEXTERN void ( *joy_ClearState )();

// 2D
typedef struct
{
  float X;
  float Y;
} zglTPoint2D, *zglPPoint2D;

typedef zglTPoint2D* zglTPoints2D;
typedef zglTPoints2D* zglPPoints2D;

typedef struct
{
  float x0;
  float y0;
  float x1;
  float y1;
} zglTLine, *zglPLine;

typedef struct
{
  float X;
  float Y;
  float W;
  float H;
} zglTRect, *zglPRect;

typedef struct
{
  float cX;
  float cY;
  float Radius;
} zglTCircle, *zglPCircle;

// RESOURCES
ZGLEXTERN void ( *res_BeginQueue )( byte QueueID );
ZGLEXTERN void ( *res_EndQueue )();
ZGLEXTERN int ( *res_GetPercentage )( byte QueueID );
ZGLEXTERN int ( *res_GetCompleted )();
ZGLEXTERN void ( *res_Proc )();

// TEXTURES
typedef zglTPoint2D zglTTextureCoord[ 3 ];
typedef zglTTextureCoord *zglPTextureCoord;

typedef struct
{
  uint              ID;
  ushort            Width;
  ushort            Height;
  float             U;
  float             V;
  ushort            FramesX;
  ushort            FramesY;
  zglTTextureCoord *FramesCoord;
  uint              Flags;
  ushort            Format;

  void*             prev;
  void*             next;
} zglTTexture, *zglPTexture;

/*
typedef struct
{
    Extension  : String;
    FileLoader : procedure( const FileName : String; var pData : Pointer; var W, H, Format : Word );
    MemLoader  : procedure( const Memory : zglTMemory; var pData : Pointer; var W, H, Format : Word );
} zglTTextureFormat, *zglPTextureFormat;

type
  zglPTextureManager = ^zglTTextureManager;
  zglTTextureManager = record
    Count   : record
      Items   : Integer;
      Formats : Integer;
              end;
    First   : zglTTexture;
    Formats : array of zglTTextureFormat;
end;
*/

#define TEX_FORMAT_RGBA       0x01
#define TEX_FORMAT_RGBA_4444  0x02
#define TEX_FORMAT_RGBA_PVR2  0x10
#define TEX_FORMAT_RGBA_PVR4  0x11
#define TEX_FORMAT_RGBA_DXT1  0x20
#define TEX_FORMAT_RGBA_DXT3  0x21
#define TEX_FORMAT_RGBA_DXT5  0x22

#define TEX_NO_COLORKEY       0xFF000000

#define TEX_MIPMAP            0x000001
#define TEX_CLAMP             0x000002
#define TEX_REPEAT            0x000004
#define TEX_COMPRESS          0x000008

#define TEX_CONVERT_TO_POT    0x000010
#define TEX_CALCULATE_ALPHA   0x000020

#define TEX_GRAYSCALE         0x000040
#define TEX_INVERT            0x000080
#define TEX_CUSTOM_EFFECT     0x000100

#define TEX_FILTER_NEAREST    0x000200
#define TEX_FILTER_LINEAR     0x000400
#define TEX_FILTER_BILINEAR   0x000800
#define TEX_FILTER_TRILINEAR  0x001000
#define TEX_FILTER_ANISOTROPY 0x002000

#define TEX_DEFAULT_2D        TEX_CLAMP | TEX_FILTER_LINEAR | TEX_CONVERT_TO_POT | TEX_CALCULATE_ALPHA

ZGLEXTERN zglPTexture ( *tex_Add )();
ZGLEXTERN void ( *tex_Del )( zglPTexture *Texture );
ZGLEXTERN zglPTexture ( *tex_Create )( void *Data, ushort Width, ushort Height, ushort Format, uint Flags );
#ifdef __CPP__
ZGLEXTERN zglPTexture ( *__tex_CreateZero )( ushort Width, ushort Height, uint Color, uint Flags );
static inline zglPTexture tex_CreateZero( ushort Width, ushort Height, uint Color = 0x000000, uint Flags = TEX_DEFAULT_2D )
{
  return __tex_CreateZero( Width, Height, Color, Flags );
}
ZGLEXTERN zglPTexture ( *__tex_LoadFromFile )( const char *FileName, uint TransparentColor, uint Flags );
static inline zglPTexture tex_LoadFromFile( const char *FileName, uint TransparentColor = TEX_NO_COLORKEY, uint Flags = TEX_DEFAULT_2D )
{
  return __tex_LoadFromFile( FileName, TransparentColor, Flags );
}
ZGLEXTERN zglPTexture ( *__tex_LoadFromMemory )( const zglTMemory Memory, const char *Extension, uint TransparentColor, uint Flags );
static inline zglPTexture tex_LoadFromMemory( const zglTMemory Memory, const char *Extension, uint TransparentColor = TEX_NO_COLORKEY, uint Flags = TEX_DEFAULT_2D )
{
  return __tex_LoadFromMemory( Memory, Extension, TransparentColor, Flags );
}
#else
ZGLEXTERN zglPTexture ( *tex_CreateZero )( ushort Width, ushort Height, uint Color, uint Flags );
ZGLEXTERN zglPTexture ( *tex_LoadFromFile )( const char *FileName, uint TransparentColor, uint Flags );
ZGLEXTERN zglPTexture ( *tex_LoadFromMemory )( const zglTMemory Memory, const char *Extension, uint TransparentColor, uint Flags );
#endif
ZGLEXTERN void ( *tex_SetFrameSize )( zglPTexture *Texture, ushort FrameWidth, ushort FrameHeight );
ZGLEXTERN void ( *tex_SetMask )( zglPTexture *Texture, zglPTexture Mask );
#ifdef __CPP__
ZGLEXTERN void ( *__tex_SetData )( zglPTexture Texture, void *pData, ushort X, ushort Y, ushort Width, ushort Height, int Stride );
static inline void tex_SetData( zglPTexture Texture, void *pData, ushort X, ushort Y, ushort Width, ushort Height, int Stride = 0 )
{
  __tex_SetData( Texture, pData, X, Y, Width, Height, Stride );
}
#else
ZGLEXTERN void ( *tex_SetData )( zglPTexture Texture, void *pData, ushort X, ushort Y, ushort Width, ushort Height, int Stride );
#endif
ZGLEXTERN void ( *tex_GetData )( zglPTexture Texture, void **pData );
ZGLEXTERN void ( *tex_Filter )( zglPTexture Texture, uint Flags );
ZGLEXTERN void ( *tex_SetAnisotropy )( byte Level );

// RENDER TARGETS
typedef struct
{
  byte        _type;
  void*       Handle;
  zglPTexture Surface;
  byte        Flags;

  void*       prev;
  void*       next;
} zglTRenderTarget, *zglPRenderTarget;
/*
type
  zglPRenderTargetManager = ^zglTRenderTargetManager;
  zglTRenderTargetManager = record
    Count : Integer;
    First : zglTRenderTarget;
end;
*/
typedef void (*zglTRenderCallback)( void *Data );

#define RT_DEFAULT      0x00
#define RT_FULL_SCREEN  0x01
#define RT_USE_DEPTH    0x02
#define RT_CLEAR_COLOR  0x04
#define RT_CLEAR_DEPTH  0x08
#define RT_SAVE_CONTENT 0x10 // Direct3D only!

ZGLEXTERN zglPRenderTarget ( *rtarget_Add )( zglPTexture Surface, byte Flags );
ZGLEXTERN void ( *rtarget_Del )( zglPRenderTarget *Target );
ZGLEXTERN void ( *rtarget_Set )( zglPRenderTarget Target );
ZGLEXTERN void ( *rtarget_DrawIn )( zglPRenderTarget Target, zglTRenderCallback RenderCallback, void *Data );

// FX
#define FX_BLEND_NORMAL 0x00
#define FX_BLEND_ADD    0x01
#define FX_BLEND_MULT   0x02
#define FX_BLEND_BLACK  0x03
#define FX_BLEND_WHITE  0x04
#define FX_BLEND_MASK   0x05

#define FX_COLOR_MIX    0x00
#define FX_COLOR_SET    0x01

#define FX_BLEND        0x100000
#define FX_COLOR        0x200000

#ifdef __CPP__
ZGLEXTERN void ( *__fx_SetBlendMode )( byte Mode, bool SeparateAlpha );
static inline void fx_SetBlendMode( byte Mode, bool SeparateAlpha = TRUE )
{
  __fx_SetBlendMode( Mode, SeparateAlpha );
}
#else
ZGLEXTERN void ( *fx_SetBlendMode )( byte Mode, bool SeparateAlpha );
#endif
ZGLEXTERN void ( *fx_SetColorMode )( byte Mode );
ZGLEXTERN void ( *fx_SetColorMask )( bool R, bool G, bool B, bool Alpha );

// FX 2D
#define FX2D_FLIPX   0x000001
#define FX2D_FLIPY   0x000002
#define FX2D_VCA     0x000004
#define FX2D_VCHANGE 0x000008
#define FX2D_SCALE   0x000010
#define FX2D_RPIVOT  0x000020

ZGLEXTERN void ( *fx2d_SetColor )( uint Color );
ZGLEXTERN void ( *fx2d_SetVCA )( uint c1, uint c2, uint c3, uint c4, byte a1, byte a2, byte a3, byte a4 );
ZGLEXTERN void ( *fx2d_SetVertexes )( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
ZGLEXTERN void ( *fx2d_SetScale )( float scaleX, float scaleY );
ZGLEXTERN void ( *fx2d_SetRotatingPivot )( float X, float Y );

// Camera 2D
typedef struct
{
  float       X;
  float       Y;
  float       Angle;
  zglTPoint2D Zoom;
  zglTPoint2D Center;
} zglTCamera2D, *zglPCamera2D;

ZGLEXTERN void ( *cam2d_Init )( zglTCamera2D *Camera );
ZGLEXTERN void ( *cam2d_Set )( zglPCamera2D Camera );
ZGLEXTERN zglPCamera2D ( *cam2d_Get )();

// Render 2D
ZGLEXTERN void ( *batch2d_Begin )();
ZGLEXTERN void ( *batch2d_End )();
ZGLEXTERN void ( *batch2d_Flush )();

// Primitives 2D
#define PR2D_FILL   0x010000
#define PR2D_SMOOTH 0x020000

#ifdef __CPP__
ZGLEXTERN void ( *__pr2d_Pixel )( float X, float Y, uint Color, byte Alpha );
static inline void pr2d_Pixel( float X, float Y, uint Color = 0xFFFFFF, byte Alpha = 255 )
{
  __pr2d_Pixel( X, Y, Color, Alpha );
}
ZGLEXTERN void ( *__pr2d_Line )( float X1, float Y1, float X2, float Y2, uint Color, byte Alpha, uint FX );
static inline void pr2d_Line( float X1, float Y1, float X2, float Y2, uint Color = 0xFFFFFF, byte Alpha = 255, uint FX = 0 )
{
  __pr2d_Line( X1, Y1, X2, Y2, Color, Alpha, FX );
}
ZGLEXTERN void ( *__pr2d_Rect )( float X, float Y, float W, float H, uint Color, byte Alpha, uint FX );
static inline void pr2d_Rect( float X, float Y, float W, float H, uint Color = 0xFFFFFF, byte Alpha = 255, uint FX = 0 )
{
  __pr2d_Rect( X, Y, W, H, Color, Alpha, FX );
}
ZGLEXTERN void ( *__pr2d_Circle )( float X, float Y, float Radius, uint Color, byte Alpha, ushort Quality, uint FX );
static inline void pr2d_Circle( float X, float Y, float Radius, uint Color = 0xFFFFFF, byte Alpha = 255, ushort Quality = 32, uint FX = 0 )
{
  __pr2d_Circle( X, Y, Radius, Color, Alpha, Quality, FX );
}
ZGLEXTERN void ( *__pr2d_Ellipse )( float X, float Y, float xRadius, float yRadius, uint Color, byte Alpha, ushort Quality, uint FX );
static inline void pr2d_Ellipse( float X, float Y, float xRadius, float yRadius, uint Color = 0xFFFFFF, byte Alpha = 255, ushort Quality = 32, uint FX = 0 )
{
  __pr2d_Ellipse( X, Y, xRadius, yRadius, Color, Alpha, Quality, FX );
}
ZGLEXTERN void ( *__pr2d_TriList )( zglPTexture Texture, zglPPoints2D TriList, zglPPoints2D TexCoords, int iLo, int iHi, uint Color, byte Alpha, uint FX );
static inline void pr2d_TriList( zglPTexture Texture, zglPPoints2D TriList, zglPPoints2D TexCoords, int iLo, int iHi, uint Color = 0xFFFFFF, byte Alpha = 255, uint FX = 0 )
{
  __pr2d_TriList( Texture, TriList, TexCoords, iLo, iHi, Color, Alpha, FX );
}
#else
ZGLEXTERN void ( *pr2d_Pixel )( float X, float Y, uint Color, byte Alpha );
ZGLEXTERN void ( *pr2d_Line )( float X1, float Y1, float X2, float Y2, uint Color, byte Alpha, uint FX );
ZGLEXTERN void ( *pr2d_Rect )( float X, float Y, float W, float H, uint Color, byte Alpha, uint FX );
ZGLEXTERN void ( *pr2d_Circle )( float X, float Y, float Radius, uint Color, byte Alpha, ushort Quality, uint FX );
ZGLEXTERN void ( *pr2d_Ellipse )( float X, float Y, float xRadius, float yRadius, uint Color, byte Alpha, ushort Quality, uint FX );
ZGLEXTERN void ( *pr2d_TriList )( zglPTexture Texture, zglPPoints2D TriList, zglPPoints2D TexCoords, int iLo, int iHi, uint Color, byte Alpha, uint FX );
#endif

// Sprites 2D
struct zglTSprite2D;
struct zglTSEngine2D;

typedef void (*zglSpriteFunc)( struct zglTSprite2D *Sprite );

typedef struct zglTSEngine2D
{
  int           Count;
  struct zglTSprite2D *List;
} zglTSEngine2D, *zglPSEngine2D;

#define ZGLTSPRITE2D \
  int           ID; \
  zglPSEngine2D Manager; \
  zglPTexture   Texture; \
  bool          Destroy; \
  uint          Layer; \
  float         X; \
  float         Y; \
  float         W; \
  float         H; \
  float         Angle; \
  float         Frame; \
  int           Alpha; \
  uint          FxFlags; \
  void*         Data; \
  zglSpriteFunc OnInit; \
  zglSpriteFunc OnDraw; \
  zglSpriteFunc OnProc; \
  zglSpriteFunc OnFree;

typedef struct zglTSprite2D
{
  int           ID;
  zglPSEngine2D Manager;
  zglPTexture   Texture;
  bool          Destroy;
  uint          Layer;
  float         X;
  float         Y;
  float         W;
  float         H;
  float         Angle;
  float         Frame;
  int           Alpha;
  uint          FxFlags;
  void*         Data;

  zglSpriteFunc OnInit;
  zglSpriteFunc OnDraw;
  zglSpriteFunc OnProc;
  zglSpriteFunc OnFree;
} zglTSprite2D, *zglPSprite2D;

typedef struct
{
  struct
  {
    int X;
    int Y;
  } Count;

  struct
  {
    float W;
    float H;
  } Size;

  int **Tiles;
} zglTTiles2D, *zglPTiles2D;

typedef struct
{
  int           Cols;
  int           Rows;
  zglTPoint2D** Grid;
} zglTGrid2D, *zglPGrid2D;

ZGLEXTERN zglPSprite2D ( *sengine2d_AddSprite )( zglPTexture Texture, int Layer, zglSpriteFunc OnInit, zglSpriteFunc OnDraw, zglSpriteFunc OnProc, zglSpriteFunc OnFree );
ZGLEXTERN zglPSprite2D ( *sengine2d_AddCustom )( zglPTexture Texture, uint Size, int Layer, zglSpriteFunc OnInit, zglSpriteFunc OnDraw, zglSpriteFunc OnProc, zglSpriteFunc OnFree );
ZGLEXTERN void ( *sengine2d_DelSprite )( int ID );
ZGLEXTERN void ( *sengine2d_ClearAll )();
ZGLEXTERN void ( *sengine2d_Set )( zglPSEngine2D SEngine );
ZGLEXTERN void ( *sengine2d_Draw )();
ZGLEXTERN void ( *sengine2d_Proc )();

#ifdef __CPP__
ZGLEXTERN void ( *__texture2d_Draw )( zglPTexture Texture, zglTPoint2D *TexCoord, float X, float Y, float W, float H, float Angle, byte Alpha, uint FX );
static inline void texture2d_Draw( zglPTexture Texture, zglTPoint2D *TexCoord, float X, float Y, float W, float H, float Angle, byte Alpha = 255, uint FX = FX_BLEND )
{
  __texture2d_Draw( Texture, TexCoord, X, Y, W, H, Angle, Alpha, FX );
}
ZGLEXTERN void ( *__ssprite2d_Draw )( zglPTexture Texture, float X, float Y, float W, float H, float Angle, byte Alpha, uint FX );
static inline void ssprite2d_Draw( zglPTexture Texture, float X, float Y, float W, float H, float Angle, byte Alpha = 255, uint FX = FX_BLEND )
{
  __ssprite2d_Draw( Texture, X, Y, W, H, Angle, Alpha, FX );
}
ZGLEXTERN void ( *__asprite2d_Draw )( zglPTexture Texture, float X, float Y, float W, float H, float Angle, ushort Frame, byte Alpha, uint FX );
static inline void asprite2d_Draw( zglPTexture Texture, float X, float Y, float W, float H, float Angle, ushort Frame, byte Alpha = 255, uint FX = FX_BLEND )
{
  __asprite2d_Draw( Texture, X, Y, W, H, Angle, Frame, Alpha, FX );
}
ZGLEXTERN void ( *__csprite2d_Draw )( zglPTexture Texture, float X, float Y, float W, float H, float Angle, zglTRect CutRect, byte Alpha, uint FX );
static inline void csprite2d_Draw( zglPTexture Texture, float X, float Y, float W, float H, float Angle, zglTRect CutRect, byte Alpha = 255, uint FX = FX_BLEND )
{
  __csprite2d_Draw( Texture, X, Y, W, H, Angle, CutRect, Alpha, FX );
}
ZGLEXTERN void ( *__tiles2d_Draw )( zglPTexture Texture, float X, float Y, zglPTiles2D Tiles, byte Alpha, uint FX );
static inline void tiles2d_Draw( zglPTexture Texture, float X, float Y, zglPTiles2D Tiles, byte Alpha = 255, uint FX = FX_BLEND )
{
  __tiles2d_Draw( Texture, X, Y, Tiles, Alpha, FX );
}
ZGLEXTERN void ( *__sgrid2d_Draw )( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, byte Alpha, uint FX );
static inline void sgrid2d_Draw( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, byte Alpha = 255, uint FX = FX_BLEND )
{
  __sgrid2d_Draw( Texture, X, Y, Grid, Alpha, FX );
}
ZGLEXTERN void ( *__agrid2d_Draw )( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, int Frame, byte Alpha, uint FX );
static inline void agrid2d_Draw( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, int Frame, byte Alpha = 255, uint FX = FX_BLEND )
{
  __agrid2d_Draw( Texture, X, Y, Grid, Frame, Alpha, FX );
}
ZGLEXTERN void ( *__cgrid2d_Draw )( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, zglTRect CutRect, byte Alpha, uint FX );
static inline void cgrid2d_Draw( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, zglTRect CutRect, byte Alpha = 255, uint FX = FX_BLEND )
{
  __cgrid2d_Draw( Texture, X, Y, Grid, CutRect, Alpha, FX );
}
#else
ZGLEXTERN void ( *texture2d_Draw )( zglPTexture Texture, zglTPoint2D *TexCoord, float X, float Y, float W, float H, float Angle, byte Alpha, uint FX );
ZGLEXTERN void ( *ssprite2d_Draw )( zglPTexture Texture, float X, float Y, float W, float H, float Angle, byte Alpha, uint FX );
ZGLEXTERN void ( *asprite2d_Draw )( zglPTexture Texture, float X, float Y, float W, float H, float Angle, ushort Frame, byte Alpha, uint FX );
ZGLEXTERN void ( *csprite2d_Draw )( zglPTexture Texture, float X, float Y, float W, float H, float Angle, zglTRect CutRect, byte Alpha, uint FX );
ZGLEXTERN void ( *tiles2d_Draw )( zglPTexture Texture, float X, float Y, zglPTiles2D Tiles, byte Alpha, uint FX );
ZGLEXTERN void ( *sgrid2d_Draw )( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, byte Alpha, uint FX );
ZGLEXTERN void ( *agrid2d_Draw )( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, int Frame, byte Alpha, uint FX );
ZGLEXTERN void ( *cgrid2d_Draw )( zglPTexture Texture, float X, float Y, zglPGrid2D Grid, zglTRect CutRect, byte Alpha, uint FX );
#endif

// Particles
#define EMITTER_MAX_PARTICLES 1024

#define EMITTER_NONE      0
#define EMITTER_POINT     1
#define EMITTER_LINE      2
#define EMITTER_RECTANGLE 3
#define EMITTER_CIRCLE    4
#define EMITTER_RING      5

typedef struct
{
  float Life;
  byte  Value;
} TDiagramByte;

typedef struct
{
  float Life;
  uint  Value;
} TDiagramLW;

typedef struct
{
  float Life;
  float Value;
} TDiagramSingle;

typedef struct
{
  struct
  {
    int lColorID;
    int lAlphaID;
    int lSizeXID;
    int lSizeYID;
    int lVelocityID;
    int laVelocityID;
    int lSpinID;
  } _private;

  int         ID;

  float       Life;
  int         LifeTime;
  double      Time;

  ushort      Frame;
  uint        Color;
  byte        Alpha;

  zglTPoint2D Position;
  zglTPoint2D Size;
  zglTPoint2D SizeS;
  float       Angle;
  float       Direction;

  float       Velocity;
  float       VelocityS;
  float       aVelocity;
  float       aVelocityS;
  float       Spin;
} zglTParticle2D, *zglPParticle2D;

typedef struct
{
  float Direction;
  float Spread;
} zglTEmitterPoint;

typedef struct
{
  float Direction;
  float Spread;
  float Size;
  bool  TwoSide;
} zglTEmitterLine;

typedef struct
{
  float    Direction;
  float    Spread;
  zglTRect Rect;
} zglTEmitterRect;

typedef struct
{
  float Direction;
  float Spread;
  float cX;
  float cY;
  float Radius;
} zglTEmitterCircle;

typedef struct
{
  float Direction;
  float Spread;
  float cX;
  float cY;
  float Radius0;
  float Radius1;
} zglTEmitterRing;

typedef struct
{
  zglPTexture    Texture;
  byte           BlendMode;
  byte           ColorMode;

  int            LifeTimeS;
  int            LifeTimeV;
  int            Frame[2];
  TDiagramLW     *Color;
  TDiagramByte   *Alpha;
  bool           SizeXYBind;
  float          SizeXS;
  float          SizeYS;
  float          SizeXV;
  float          SizeYV;
  TDiagramSingle *SizeXD;
  TDiagramSingle *SizeYD;
  float          AngleS;
  float          AngleV;
  float          VelocityS;
  float          VelocityV;
  TDiagramSingle *VelocityD;
  float          aVelocityS;
  float          aVelocityV;
  TDiagramSingle *aVelocityD;
  float          SpinS;
  float          SpinV;
  TDiagramSingle *SpinD;
} zglTParticleParams;

typedef struct
{
  struct
  {
    struct zglTPEngine2D  *pengine;
    zglTParticle2D particle[EMITTER_MAX_PARTICLES];
    zglPParticle2D list[EMITTER_MAX_PARTICLES];
    int            parCreated;
    size_t         reserved; // texFile    : UTF8String;
    uint           texHash;
  } _private;

  int  ID;
  byte Type_;
  struct
  {
    int         Layer;
    int         LifeTime;
    bool        Loop;
    int         Emission;
    zglTPoint2D Position;
  } Params;

  zglTParticleParams ParParams;

  float  Life;
  double Time;
  double LastSecond;
  int    Particles;
  struct
  {
    float MinX;
    float MaxX;
    float MinY;
    float MaxY;
  } BBox;

  union
  {
    zglTEmitterPoint  AsPoint;
    zglTEmitterLine   AsLine;
    zglTEmitterRect   AsRect;
    zglTEmitterCircle AsCircle;
    zglTEmitterRing   AsRing;
  };
} zglTEmitter2D, *zglPEmitter2D, **zglPPEmitter2D;

typedef struct zglTPEngine2D
{
  struct
  {
    int Emitters;
    int Particles;
  } Count;

  zglPEmitter2D *List;
  zglPPEmitter2D *ListU;
} zglTPEngine2D, *zglPPEngine2D;

ZGLEXTERN void ( *pengine2d_Set )( zglPPEngine2D PEngine );
ZGLEXTERN zglPPEngine2D ( *pengine2d_Get )();
ZGLEXTERN void ( *pengine2d_Draw )();
ZGLEXTERN void ( *pengine2d_Proc )( double dt );
#ifdef __CPP__
ZGLEXTERN void ( *__pengine2d_AddEmitter )( zglPEmitter2D Emitter, zglPPEmitter2D Result, float X, float Y );
static inline void pengine2d_AddEmitter( zglPEmitter2D Emitter, zglPPEmitter2D Result, float X = 0, float Y = 0 )
{
  __pengine2d_AddEmitter( Emitter, Result, X, Y );
}
#else
ZGLEXTERN void ( *pengine2d_AddEmitter )( zglPEmitter2D Emitter, zglPPEmitter2D Result, float X, float Y );
#endif
ZGLEXTERN void ( *pengine2d_DelEmitter )( int ID );
ZGLEXTERN void ( *pengine2d_ClearAll )();
ZGLEXTERN zglPEmitter2D ( *emitter2d_Add )();
ZGLEXTERN void ( *emitter2d_Del )( zglPEmitter2D *Emitter );
ZGLEXTERN zglPEmitter2D ( *emitter2d_LoadFromFile )( const char *FileName );
ZGLEXTERN zglPEmitter2D ( *emitter2d_LoadFromMemory )( zglTMemory Memory );
ZGLEXTERN void ( *emitter2d_Init )( zglPEmitter2D Emitter );
ZGLEXTERN void ( *emitter2d_Free )( zglPEmitter2D *Emitter );
ZGLEXTERN void ( *emitter2d_Draw )( zglPEmitter2D Emitter );
ZGLEXTERN void ( *emitter2d_Proc )( zglPEmitter2D Emitter, double dt );

// Text
typedef struct
{
  ushort           Page;
  byte             Width;
  byte             Height;
  int              ShiftX;
  int              ShiftY;
  int              ShiftP;
  zglTTextureCoord TexCoords;
} zglTCharDesc, *zglPCharDesc;

typedef struct
{
  struct
  {
    ushort Pages;
    ushort Chars;
  } Count;

  zglPTexture *Pages;
  zglPCharDesc CharDesc[65536];
  int          MaxHeight;
  int          MaxShiftY;
  byte         Padding[3];

  void*        prev;
  void*        next;
} zglTFont, *zglPFont;

typedef struct
{
  int      Count;
  zglTFont First;
} zglTFontManager, *zglPFontManager;

#define TEXT_HALIGN_LEFT    0x000001
#define TEXT_HALIGN_CENTER  0x000002
#define TEXT_HALIGN_RIGHT   0x000004
#define TEXT_HALIGN_JUSTIFY 0x000008
#define TEXT_VALIGN_TOP     0x000010
#define TEXT_VALIGN_CENTER  0x000020
#define TEXT_VALIGN_BOTTOM  0x000040
#define TEXT_CLIP_RECT      0x000080
#define TEXT_FX_VCA         0x000100
#define TEXT_FX_LENGTH      0x000200

ZGLEXTERN zglPFont ( *font_Add )();
ZGLEXTERN void ( *font_Del )( zglPFont *Font );
ZGLEXTERN zglPFont ( *font_LoadFromFile )( const char *FileName );
ZGLEXTERN zglPFont ( *font_LoadFromMemory )( zglTMemory Memory );
#ifdef __CPP__
ZGLEXTERN void ( *__text_Draw )( zglPFont Font, float X, float Y, const char *Text, uint Flags );
static inline void text_Draw( zglPFont Font, float X, float Y, const char *Text, uint Flags = 0 )
{
  __text_Draw( Font, X, Y, Text, Flags );
}
ZGLEXTERN void ( *__text_DrawEx )( zglPFont Font, float X, float Y, float Scale, float Step, const char *Text, byte Alpha, uint Color, uint Flags );
static inline void text_DrawEx( zglPFont Font, float X, float Y, float Scale, float Step, const char *Text, byte Alpha = 255, uint Color = 0xFFFFFF, uint Flags = 0 )
{
  __text_DrawEx( Font, X, Y, Scale, Step, Text, Alpha, Color, Flags );
}
ZGLEXTERN void ( *__text_DrawInRect )( zglPFont Font, zglTRect Rect, const char *Text, uint Flags );
static inline void text_DrawInRect( zglPFont Font, zglTRect Rect, const char *Text, uint Flags = 0 )
{
  __text_DrawInRect( Font, Rect, Text, Flags );
}
ZGLEXTERN void ( *__text_DrawInRectEx )( zglPFont Font, zglTRect Rect, float Scale, float Step, const char *Text, byte Alpha, uint Color, uint Flags );
static inline void text_DrawInRectEx( zglPFont Font, zglTRect Rect, float Scale, float Step, const char *Text, byte Alpha = 255, uint Color = 0xFFFFFF, uint Flags = 0 )
{
  __text_DrawInRectEx( Font, Rect, Scale, Step, Text, Alpha, Color, Flags );
}
ZGLEXTERN float ( *__text_GetWidth )( zglPFont Font, const char *Text, float Step );
static inline float text_GetWidth( zglPFont Font, const char *Text, float Step = 0.0 )
{
  return __text_GetWidth( Font, Text, Step );
}
ZGLEXTERN float ( *__text_GetHeight )( zglPFont Font, float Width, const char *Text, float Scale, float Step );
static inline float text_GetHeight( zglPFont Font, float Width, const char *Text, float Scale = 1.0, float Step = 0.0 )
{
  return __text_GetHeight( Font, Width, Text, Scale, Step );
}
#else
ZGLEXTERN void ( *text_Draw )( zglPFont Font, float X, float Y, const char *Text, uint Flags );
ZGLEXTERN void ( *text_DrawEx )( zglPFont Font, float X, float Y, float Scale, float Step, const char *Text, byte Alpha, uint Color, uint Flags );
ZGLEXTERN void ( *text_DrawInRect )( zglPFont Font, zglTRect Rect, const char *Text, uint Flags );
ZGLEXTERN void ( *text_DrawInRectEx )( zglPFont Font, zglTRect Rect, float Scale, float Step, const char *Text, byte Alpha, uint Color, uint Flags );
ZGLEXTERN float ( *text_GetWidth )( zglPFont Font, const char *Text, float Step );
ZGLEXTERN float ( *text_GetHeight )( zglPFont Font, float Width, const char *Text, float Scale, float Step );
#endif
ZGLEXTERN void ( *textFx_SetLength )( int Length, zglPPoint2D LastCoord, zglPCharDesc LastCharDesc );

// Sound
#define SND_FORMAT_MONO8    1
#define SND_FORMAT_MONO16   2
#define SND_FORMAT_STEREO8  3
#define SND_FORMAT_STEREO16 4

#define SND_VOLUME_DEFAULT -1

#define SND_SOUNDS             NULL
#define SND_STREAM             NULL
#define SND_ALL_SOURCES        -1
#define SND_ALL_SOURCES_LOOPED -2
#define SND_ALL_STREAMS        -3
#define SND_ALL_STREAMS_LOOPED -4

#define SND_STATE_PLAYING 1
#define SND_STATE_LOOPED  2
#define SND_STATE_PERCENT 3
#define SND_STATE_TIME    4
#define SND_INFO_DURATION 5

typedef struct
{
  uint  Source;
  float Speed;
  float Volume;

  struct
  {
    float X;
    float Y;
    float Z;
  } Position;
} zglTSoundChannel, *zglPSoundChannel;

typedef struct
{
  uint              Buffer;
  uint              SourceCount;
  zglTSoundChannel *Channel;

  byte*             Data;
  uint              Size;
  double            Duration;
  uint              Frequency;

  void*             prev;
  void*             next;
} zglTSound, *zglPSound;
/*
typedef struct
{
  zglTSoundStream = record
    _data      : Pointer;
    _file      : zglTFile;
    _memory    : zglTMemory;
    _decoder   : zglPSoundDecoder;
    _playing   : Boolean;
    _paused    : Boolean;
    _waiting   : Boolean;
    _complete  : Double;
    _lastTime  : Double;

    Buffer     : Pointer;
    BufferSize : LongWord;

    Bits       : LongWord;
    Frequency  : LongWord;
    Channels   : LongWord;
    Length     : Double;

    Loop       : Boolean;
} zglTSoundStream, *zglPSoundStream;

  zglTSoundDecoder = record
    Ext     : String;
    Open    : function( var Stream : zglTSoundStream; const FileName : String ) : Boolean;
    OpenMem : function( var Stream : zglTSoundStream; const Memory : zglTMemory ) : Boolean;
    Read    : function( var Stream : zglTSoundStream; Buffer : Pointer; Bytes : LongWord; var _End : Boolean ) : LongWord;
    Loop    : procedure( var Stream : zglTSoundStream );
    Close   : procedure( var Stream : zglTSoundStream );
  end;

  zglTSoundFormat = record
    Extension  : String;
    Decoder    : zglPSoundDecoder;
    FileLoader : procedure( const FileName : String; var Data : Pointer; var Size, Format, Frequency : LongWord );
    MemLoader  : procedure( const Memory : zglTMemory; var Data : Pointer; var Size, Format, Frequency : LongWord );
  end;

  zglTSoundManager = record
    Count   : record
      Items   : Integer;
      Formats : Integer;
              end;
    First   : zglTSound;
    Formats : array of zglTSoundFormat;
  end;
*/
ZGLEXTERN bool ( *snd_Init )();
ZGLEXTERN void ( *snd_Free )();
ZGLEXTERN zglPSound ( *snd_Add )( int SourceCount );
ZGLEXTERN void ( *snd_Del )( zglPSound *Sound );
#ifdef __CPP__
ZGLEXTERN zglPSound ( *__snd_LoadFromFile )( const char *FileName, int SourceCount );
static inline zglPSound snd_LoadFromFile( const char *FileName, int SourceCount = 8 )
{
  return __snd_LoadFromFile( FileName, SourceCount );
}
ZGLEXTERN zglPSound ( *__snd_LoadFromMemory )( zglTMemory Memory, const char *Extension, int SourceCount );
static inline zglPSound snd_LoadFromMemory( zglTMemory Memory, const char *Extension, int SourceCount = 8 )
{
  return __snd_LoadFromMemory( Memory, Extension, SourceCount );
}
ZGLEXTERN int ( *__snd_Play )( zglPSound Sound, bool Loop, float X, float Y, float Z, float Volume );
static inline int snd_Play( zglPSound Sound, bool Loop = FALSE, float X = 0.0, float Y = 0.0, float Z = 0.0, float Volume = SND_VOLUME_DEFAULT )
{
  return __snd_Play( Sound, Loop, X, Y, Z, Volume );
}
#else
ZGLEXTERN zglPSound ( *snd_LoadFromFile )( const char *FileName, int SourceCount );
ZGLEXTERN zglPSound ( *snd_LoadFromMemory )( zglTMemory Memory, const char *Extension, int SourceCount );
ZGLEXTERN int ( *snd_Play )( zglPSound Sound, bool Loop, float X, float Y, float Z, float Volume );
#endif
ZGLEXTERN void ( *snd_Stop )( zglPSound Sound, int ID );
ZGLEXTERN void ( *snd_SetPos )( zglPSound Sound, int ID, float X, float Y, float Z );
ZGLEXTERN void ( *snd_SetVolume )( zglPSound Sound, int ID, float Volume );
ZGLEXTERN void ( *snd_SetSpeed )( zglPSound Sound, int ID, float Speed );
ZGLEXTERN int ( *snd_Get )( zglPSound Sound, int ID, int What );
#ifdef __CPP__
ZGLEXTERN int ( *__snd_PlayFile )( const char *FileName, bool Loop, float Volume );
static inline int snd_PlayFile( const char *FileName, bool Loop = FALSE, float Volume = SND_VOLUME_DEFAULT )
{
  return __snd_PlayFile( FileName, Loop, Volume );
}
ZGLEXTERN int ( *__snd_PlayMemory )( zglTMemory Memory, const char *Extension, bool Loop, float Volume );
static inline int snd_PlayMemory( zglTMemory Memory, const char *Extension, bool Loop = FALSE, float Volume = SND_VOLUME_DEFAULT )
{
  return __snd_PlayMemory( Memory, Extension, Loop, Volume );
}
#else
ZGLEXTERN int ( *snd_PlayFile )( const char *FileName, bool Loop, float Volume );
ZGLEXTERN int ( *snd_PlayMemory )( zglTMemory Memory, const char *Extension, bool Loop, float Volume );
#endif
ZGLEXTERN void ( *snd_PauseStream )( int ID );
ZGLEXTERN void ( *snd_StopStream )( int ID );
ZGLEXTERN void ( *snd_ResumeStream )( int ID );
ZGLEXTERN void ( *snd_SeekStream )( int ID, double Milliseconds );

// Video
typedef struct
{
  struct
  {
    void       *Data;
    zglTFile   File_;
    zglTMemory Memory;
    void       *Decoder; // zglPVideoDecoder;
  } _private;

  void        *Data;
  zglPTexture Texture;
  int         Frame;
  double      Time;

  struct
  {
    ushort  Width;
    ushort  Height;
    float   FrameRate;
    double  Duration;
    int     Frames;
  } Info;

  bool Loop;

  void *prev;
  void *next;
} zglTVideoStream, *zglPVideoStream;

ZGLEXTERN zglPVideoStream ( *video_Add )();
ZGLEXTERN void ( *video_Del )( zglPVideoStream *Stream );
ZGLEXTERN zglPVideoStream ( *video_OpenFile )( const char *FileName );
ZGLEXTERN zglPVideoStream ( *video_OpenMemory )( zglTMemory Memory, const char *Extension );
ZGLEXTERN void ( *video_Update )( zglPVideoStream *Stream, double Milliseconds, bool Loop );
ZGLEXTERN void ( *video_Seek )( zglPVideoStream *Stream, double Milliseconds );

// MATH
#define pi      3.141592654
#define rad2deg 57.29578049
#define deg2rad 0.017453292

#define ORIENTATION_LEFT  -1
#define ORIENTATION_RIGHT 1
#define ORIENTATION_ZERO  0

ZGLEXTERN float ( *m_Cos )( int Angle );
ZGLEXTERN float ( *m_Sin )( int Angle );
ZGLEXTERN float ( *m_Distance )( float x1, float y1, float x2, float y2 );
ZGLEXTERN float ( *m_FDistance )( float x1, float y1, float x2, float y2 );
ZGLEXTERN float ( *m_Angle )( float x1, float y1, float x2, float y2 );
ZGLEXTERN int ( *m_Orientation )( float x, float y, float x1, float y1, float x2, float y2 );

#ifdef __CPP__
ZGLEXTERN void ( *__tess_Triangulate )( zglPPoints2D Contour, int iLo, int iHi, bool AddHoles );
static inline void tess_Triangulate( zglPPoints2D Contour, int iLo, int iHi, bool AddHoles = FALSE )
{
  __tess_Triangulate( Contour, iLo, iHi, AddHoles );
}
ZGLEXTERN void ( *__tess_AddHole )( zglPPoints2D Contour, int iLo, int iHi, bool LastHole );
static inline void tess_AddHole( zglPPoints2D Contour, int iLo, int iHi, bool LastHole = TRUE )
{
  __tess_AddHole( Contour, iLo, iHi, LastHole );
}
#else
ZGLEXTERN void ( *tess_Triangulate )( zglPPoints2D Contour, int iLo, int iHi, bool AddHoles );
ZGLEXTERN void ( *tess_AddHole )( zglPPoints2D Contour, int iLo, int iHi, bool LastHole );
#endif
ZGLEXTERN int ( *tess_GetData )( zglPPoints2D* TriPoints );

// COLLISION 2D
ZGLEXTERN bool ( *col2d_PointInRect )( float X, float Y, zglTRect Rect );
ZGLEXTERN bool ( *col2d_PointInTriangle )( float X, float Y, zglTPoint2D P1, zglTPoint2D P2, zglTPoint2D P3 );
ZGLEXTERN bool ( *col2d_PointInCircle )( float X, float Y, zglTCircle Circle );
//line 2d
ZGLEXTERN bool ( *col2d_Line )( zglTLine A, zglTLine B, zglPPoint2D ColPoint );
ZGLEXTERN bool ( *col2d_LineVsRect )( zglTLine Line, zglTRect Rect );
ZGLEXTERN bool ( *col2d_LineVsCircle )( zglTLine Line, zglTCircle Circle );
// rect
ZGLEXTERN bool ( *col2d_Rect )( zglTRect Rect1, zglTRect Rect2 );
ZGLEXTERN bool ( *col2d_ClipRect )( zglTRect Rect1, zglTRect Rect2 );
ZGLEXTERN bool ( *col2d_RectInRect )( zglTRect Rect1, zglTRect Rect2 );
ZGLEXTERN bool ( *col2d_RectInCircle )( zglTRect Rect, zglTCircle Circle );
ZGLEXTERN bool ( *col2d_RectVsCircle )( zglTRect Rect, zglTCircle Circle );
// circle
ZGLEXTERN bool ( *col2d_Circle )( zglTCircle Circle1, zglTCircle Circle2 );
ZGLEXTERN bool ( *col2d_CircleInCircle )( zglTCircle Circle1, zglTCircle Circle2 );
ZGLEXTERN bool ( *col2d_CircleInRect )( zglTCircle Circle, zglTRect Rect );

#ifndef __WINDOWS__
  #define FILE_ERROR 0
#else
  #define FILE_ERROR -1
#endif

// Open Mode
#define FOM_CREATE 0x01 // Create
#define FOM_OPENR  0x02 // Read
#define FOM_OPENRW 0x03 // Read&Write

// Seek Mode
#define FSM_SET    0x01
#define FSM_CUR    0x02
#define FSM_END    0x03

ZGLEXTERN bool ( *file_Open )( zglTFile *FileHandle, const char *FileName, byte Mode );
ZGLEXTERN bool ( *file_MakeDir )( const char *Directory );
ZGLEXTERN bool ( *file_Remove )( const char *Name );
ZGLEXTERN bool ( *file_Exists )( const char *Name );
ZGLEXTERN uint ( *file_Seek )( zglTFile FileHandle, uint Offset, uint Mode );
ZGLEXTERN uint ( *file_GetPos )( zglTFile FileHandle );
ZGLEXTERN uint ( *file_Read )( zglTFile FileHandle, void *Buffer, uint Bytes );
ZGLEXTERN uint ( *file_Write )( zglTFile FileHandle, void *Buffer, uint Bytes );
ZGLEXTERN uint ( *file_GetSize )( zglTFile FileHandle );
ZGLEXTERN void ( *file_Flush )( zglTFile FileHandle );
ZGLEXTERN void ( *file_Close )( zglTFile* FileHandle );
ZGLEXTERN void ( *file_Find )( const char *Directory, zglTFileList *List, bool FindDir );
ZGLEXTERN char* ( *file_GetName )( const char *FileName );
ZGLEXTERN char* ( *file_GetExtension )( const char *FileName );
ZGLEXTERN char* ( *file_GetDirectory )( const char *FileName );
ZGLEXTERN void ( *file_SetPath )( const char *Path );

ZGLEXTERN bool ( *file_OpenArchive )( char *FileName, char *Password );
ZGLEXTERN void ( *file_CloseArchive )();

ZGLEXTERN bool ( *mem_LoadFromFile )( zglTMemory *Memory, const char *FileName );
ZGLEXTERN bool ( *mem_SaveToFile )( zglTMemory *Memory, const char *FileName );
ZGLEXTERN uint ( *mem_Seek )( zglTMemory *Memory, int Offset, int Mode );
ZGLEXTERN uint ( *mem_Read )( zglTMemory *Memory, void *Buffer, uint Bytes );
ZGLEXTERN uint ( *mem_Write )( zglTMemory *Memory, void *Buffer, uint Bytes );
ZGLEXTERN void ( *mem_SetSize )( zglTMemory *Memory, uint Size );
ZGLEXTERN void ( *mem_Free )( zglTMemory *Memory );

// Utils
ZGLEXTERN void ( *u_Sleep )( unsigned int Milliseconds );

#ifdef __LINUX__
  #define libZenGL "libZenGL.so"

  #define zglLoadLibrary( a ) dlopen( a, 0x01 )
  #define zglFreeLibrary dlclose
  #define zglGetAddress( a, b, c ) a = (__typeof__(a))dlsym(b, c)

  typedef void* LIBRARY;
  ZGLEXTERN void* zglLib;
#endif

#ifdef __WINDOWS__
  #define libZenGL "ZenGL.dll"

  #define zglLoadLibrary LoadLibraryA
  #define zglFreeLibrary FreeLibrary
  #if ( defined __MINGW32__ || defined __MINGW64__ )
    #define zglGetAddress( a, b, c ) a = (__typeof__(a))GetProcAddress( b, c )
  #else
    #define zglGetAddress( a, b, c ) *(void**)&a = (void*)GetProcAddress( b, c )
  #endif

  typedef HMODULE LIBRARY;
  ZGLEXTERN HMODULE zglLib;
#endif

#ifdef ZGL_IMPORT
bool zglLoad( const char* LibraryName )
{
  char libName[256];
  sprintf_s( libName, "./%s", LibraryName );
  zglLib = (LIBRARY)zglLoadLibrary( libName );
  if ( !zglLib )
    zglLib = (LIBRARY)zglLoadLibrary( LibraryName );

  if ( zglLib )
  {
#ifdef __CPP__
    zglGetAddress( __zgl_Init, zglLib, "zgl_Init" );
#else
    zglGetAddress( zgl_Init, zglLib, "zgl_Init" );
#endif
    zglGetAddress( zgl_Exit, zglLib, "zgl_Exit" );
    zglGetAddress( zgl_Reg, zglLib, "zgl_Reg" );
    zglGetAddress( zgl_Get, zglLib, "zgl_Get" );
    zglGetAddress( zgl_GetMem, zglLib, "zgl_GetMem" );
    zglGetAddress( zgl_FreeMem, zglLib, "zgl_FreeMem" );
//    zglGetAddress( zgl_FreeStrList, zglLib, "zgl_FreeStrList" );
    zglGetAddress( zgl_Enable, zglLib, "zgl_Enable" );
    zglGetAddress( zgl_Disable, zglLib, "zgl_Disable" );

#ifdef __CPP__
    zglGetAddress( __log_Add, zglLib, "log_Add" );
#else
    zglGetAddress( log_Add, zglLib, "log_Add" );
#endif

    zglGetAddress( wnd_SetCaption, zglLib, "_wnd_SetCaption" );
    zglGetAddress( wnd_SetSize, zglLib, "wnd_SetSize" );
    zglGetAddress( wnd_SetPos, zglLib, "wnd_SetPos" );
    zglGetAddress( wnd_ShowCursor, zglLib, "wnd_ShowCursor" );

    zglGetAddress( scr_Clear, zglLib, "scr_Clear" );
    zglGetAddress( scr_Flush, zglLib, "scr_Flush" );
    zglGetAddress( scr_SetVSync, zglLib, "scr_SetVSync" );
    zglGetAddress( scr_SetFSAA, zglLib, "scr_SetFSAA" );
    zglGetAddress( scr_SetOptions, zglLib, "scr_SetOptions" );
    zglGetAddress( scr_CorrectResolution, zglLib, "scr_CorrectResolution" );
    zglGetAddress( scr_ReadPixels, zglLib, "scr_ReadPixels" );
/*
    zglGetAddress( ini_LoadFromFile, zglLib, "ini_LoadFromFile" );
    zglGetAddress( ini_SaveToFile, zglLib, "ini_SaveToFile" );
    zglGetAddress( ini_Add, zglLib, "ini_Add" );
    zglGetAddress( ini_Del, zglLib, "ini_Del" );
    zglGetAddress( ini_Clear, zglLib, "ini_Clear" );
    zglGetAddress( ini_IsSection, zglLib, "ini_IsSection" );
    zglGetAddress( ini_IsKey, zglLib, "ini_IsKey" );
    zglGetAddress( ini_ReadKeyStr, zglLib, "ini_ReadKeyStr" );
    zglGetAddress( ini_ReadKeyInt, zglLib, "ini_ReadKeyInt" );
    zglGetAddress( ini_ReadKeyFloat, zglLib, "ini_ReadKeyFloat" );
    zglGetAddress( ini_ReadKeyBool, zglLib, "ini_ReadKeyBool" );
    zglGetAddress( ini_WriteKeyStr, zglLib, "ini_WriteKeyStr" );
    zglGetAddress( ini_WriteKeyInt, zglLib, "ini_WriteKeyInt" );
    zglGetAddress( ini_WriteKeyFloat, zglLib, "ini_WriteKeyFloat" );
    zglGetAddress( ini_WriteKeyBool, zglLib, "ini_WriteKeyBool" );
*/
#ifdef __CPP__
    zglGetAddress( __timer_Add, zglLib, "timer_Add" );
#else
    zglGetAddress( timer_Add, zglLib, "timer_Add" );
#endif
    zglGetAddress( timer_Del, zglLib, "timer_Del" );
    zglGetAddress( timer_GetTicks, zglLib, "timer_GetTicks" );
    zglGetAddress( timer_Reset, zglLib, "timer_Reset" );

    zglGetAddress( mouse_X, zglLib, "mouse_X" );
    zglGetAddress( mouse_Y, zglLib, "mouse_Y" );
    zglGetAddress( mouse_DX, zglLib, "mouse_DX" );
    zglGetAddress( mouse_DY, zglLib, "mouse_DY" );
    zglGetAddress( mouse_Down, zglLib, "mouse_Down" );
    zglGetAddress( mouse_Up, zglLib, "mouse_Up" );
    zglGetAddress( mouse_Click, zglLib, "mouse_Click" );
    zglGetAddress( mouse_DblClick, zglLib, "mouse_DblClick" );
    zglGetAddress( mouse_Wheel, zglLib, "mouse_Wheel" );
    zglGetAddress( mouse_ClearState, zglLib, "mouse_ClearState" );
#ifdef __CPP__
    zglGetAddress( __mouse_Lock, zglLib, "mouse_Lock" );
#else
    zglGetAddress( mouse_Lock, zglLib, "mouse_Lock" );
#endif

    zglGetAddress( key_Down, zglLib, "key_Down" );
    zglGetAddress( key_Up, zglLib, "key_Up" );
    zglGetAddress( key_Press, zglLib, "key_Press" );
    zglGetAddress( key_Last, zglLib, "key_Last" );
#ifdef __CPP__
    zglGetAddress( __key_BeginReadText, zglLib, "_key_BeginReadText" );
    zglGetAddress( __key_UpdateReadText, zglLib, "_key_UpdateReadText" );
#else
    zglGetAddress( key_BeginReadText, zglLib, "_key_BeginReadText" );
    zglGetAddress( key_UpdateReadText, zglLib, "_key_UpdateReadText" );
#endif
    zglGetAddress( key_GetText, zglLib, "key_GetText" );
    zglGetAddress( key_EndReadText, zglLib, "key_EndReadText" );
    zglGetAddress( key_ClearState, zglLib, "key_ClearState" );

    zglGetAddress( joy_Init, zglLib, "joy_Init" );
    zglGetAddress( joy_GetInfo, zglLib, "joy_GetInfo" );
    zglGetAddress( joy_AxisPos, zglLib, "joy_AxisPos" );
    zglGetAddress( joy_Down, zglLib, "joy_Down" );
    zglGetAddress( joy_Up, zglLib, "joy_Up" );
    zglGetAddress( joy_Press, zglLib, "joy_Press" );
    zglGetAddress( joy_ClearState, zglLib, "joy_ClearState" );

    zglGetAddress( res_BeginQueue, zglLib, "res_BeginQueue" );
    zglGetAddress( res_EndQueue, zglLib, "res_EndQueue" );
    zglGetAddress( res_GetPercentage, zglLib, "res_GetPercentage" );
    zglGetAddress( res_GetCompleted, zglLib, "res_GetCompleted" );
    zglGetAddress( res_Proc, zglLib, "res_Proc" );

    zglGetAddress( tex_Add, zglLib, "tex_Add" );
    zglGetAddress( tex_Del, zglLib, "tex_Del" );
    zglGetAddress( tex_Create, zglLib, "tex_Create" );
#ifdef __CPP__
    zglGetAddress( __tex_CreateZero, zglLib, "tex_CreateZero" );
    zglGetAddress( __tex_LoadFromFile, zglLib, "_tex_LoadFromFile" );
    zglGetAddress( __tex_LoadFromMemory, zglLib, "_tex_LoadFromMemory" );
#else
    zglGetAddress( tex_CreateZero, zglLib, "tex_CreateZero" );
    zglGetAddress( tex_LoadFromFile, zglLib, "_tex_LoadFromFile" );
    zglGetAddress( tex_LoadFromMemory, zglLib, "_tex_LoadFromMemory" );
#endif
    zglGetAddress( tex_SetFrameSize, zglLib, "tex_SetFrameSize" );
    zglGetAddress( tex_SetMask, zglLib, "tex_SetMask" );
#ifdef __CPP__
    zglGetAddress( __tex_SetData, zglLib, "tex_SetData" );
#else
    zglGetAddress( tex_SetData, zglLib, "tex_SetData" );
#endif
    zglGetAddress( tex_GetData, zglLib, "tex_GetData" );
    zglGetAddress( tex_Filter, zglLib, "tex_Filter" );
    zglGetAddress( tex_SetAnisotropy, zglLib, "tex_SetAnisotropy" );

    zglGetAddress( Set2DMode, zglLib, "Set2DMode" );
#ifdef __CPP__
    zglGetAddress( __Set3DMode, zglLib, "Set3DMode" );
#else
    zglGetAddress( Set3DMode, zglLib, "Set3DMode" );
#endif

    zglGetAddress( zbuffer_SetDepth, zglLib, "zbuffer_SetDepth" );
    zglGetAddress( zbuffer_Clear, zglLib, "zbuffer_Clear" );

#ifdef __CPP__
    zglGetAddress( __scissor_Begin, zglLib, "scissor_Begin" );
#else
    zglGetAddress( scissor_Begin, zglLib, "scissor_Begin" );
#endif
    zglGetAddress( scissor_End, zglLib, "scissor_End" );

    zglGetAddress( rtarget_Add, zglLib, "rtarget_Add" );
    zglGetAddress( rtarget_Del, zglLib, "rtarget_Del" );
    zglGetAddress( rtarget_Set, zglLib, "rtarget_Set" );
    zglGetAddress( rtarget_DrawIn, zglLib, "rtarget_Set" );

#ifdef __CPP__
    zglGetAddress( __fx_SetBlendMode, zglLib, "fx_SetBlendMode" );
#else
    zglGetAddress( fx_SetBlendMode, zglLib, "fx_SetBlendMode" );
#endif
    zglGetAddress( fx_SetColorMode, zglLib, "fx_SetColorMode" );
    zglGetAddress( fx_SetColorMask, zglLib, "fx_SetColorMask" );
    zglGetAddress( fx2d_SetColor, zglLib, "fx2d_SetColor" );
    zglGetAddress( fx2d_SetVCA, zglLib, "fx2d_SetVCA" );
    zglGetAddress( fx2d_SetVertexes, zglLib, "fx2d_SetVertexes" );
    zglGetAddress( fx2d_SetScale, zglLib, "fx2d_SetScale" );
    zglGetAddress( fx2d_SetRotatingPivot, zglLib, "fx2d_SetRotatingPivot" );

    zglGetAddress( cam2d_Init, zglLib, "cam2d_Init" );
    zglGetAddress( cam2d_Set, zglLib, "cam2d_Set" );
    zglGetAddress( cam2d_Get, zglLib, "cam2d_Get" );

    zglGetAddress( batch2d_Begin, zglLib, "batch2d_Begin" );
    zglGetAddress( batch2d_End, zglLib, "batch2d_End" );
    zglGetAddress( batch2d_Flush, zglLib, "batch2d_Flush" );

#ifdef __CPP__
    zglGetAddress( __pr2d_Pixel, zglLib, "pr2d_Pixel" );
    zglGetAddress( __pr2d_Line, zglLib, "pr2d_Line" );
    zglGetAddress( __pr2d_Rect, zglLib, "pr2d_Rect" );
    zglGetAddress( __pr2d_Circle, zglLib, "pr2d_Circle" );
    zglGetAddress( __pr2d_Ellipse, zglLib, "pr2d_Ellipse" );
    zglGetAddress( __pr2d_TriList, zglLib, "pr2d_TriList" );
#else
    zglGetAddress( pr2d_Pixel, zglLib, "pr2d_Pixel" );
    zglGetAddress( pr2d_Line, zglLib, "pr2d_Line" );
    zglGetAddress( pr2d_Rect, zglLib, "pr2d_Rect" );
    zglGetAddress( pr2d_Circle, zglLib, "pr2d_Circle" );
    zglGetAddress( pr2d_Ellipse, zglLib, "pr2d_Ellipse" );
    zglGetAddress( pr2d_TriList, zglLib, "pr2d_TriList" );
#endif

    zglGetAddress( sengine2d_AddSprite, zglLib, "sengine2d_AddSprite" );
    zglGetAddress( sengine2d_AddCustom, zglLib, "sengine2d_AddCustom" );
    zglGetAddress( sengine2d_DelSprite, zglLib, "sengine2d_DelSprite" );
    zglGetAddress( sengine2d_ClearAll, zglLib, "sengine2d_ClearAll" );
    zglGetAddress( sengine2d_Set, zglLib, "sengine2d_Set" );
    zglGetAddress( sengine2d_Draw, zglLib, "sengine2d_Draw" );
    zglGetAddress( sengine2d_Proc, zglLib, "sengine2d_Proc" );

#ifdef __CPP__
    zglGetAddress( __texture2d_Draw, zglLib, "texture2d_Draw" );
    zglGetAddress( __ssprite2d_Draw, zglLib, "ssprite2d_Draw" );
    zglGetAddress( __asprite2d_Draw, zglLib, "asprite2d_Draw" );
    zglGetAddress( __csprite2d_Draw, zglLib, "csprite2d_Draw" );
    zglGetAddress( __tiles2d_Draw, zglLib, "tiles2d_Draw" );
    zglGetAddress( __sgrid2d_Draw, zglLib, "sgrid2d_Draw" );
    zglGetAddress( __agrid2d_Draw, zglLib, "agrid2d_Draw" );
    zglGetAddress( __cgrid2d_Draw, zglLib, "cgrid2d_Draw" );
#else
    zglGetAddress( texture2d_Draw, zglLib, "texture2d_Draw" );
    zglGetAddress( ssprite2d_Draw, zglLib, "ssprite2d_Draw" );
    zglGetAddress( asprite2d_Draw, zglLib, "asprite2d_Draw" );
    zglGetAddress( csprite2d_Draw, zglLib, "csprite2d_Draw" );
    zglGetAddress( tiles2d_Draw, zglLib, "tiles2d_Draw" );
    zglGetAddress( sgrid2d_Draw, zglLib, "sgrid2d_Draw" );
    zglGetAddress( agrid2d_Draw, zglLib, "agrid2d_Draw" );
    zglGetAddress( cgrid2d_Draw, zglLib, "cgrid2d_Draw" );
#endif

    zglGetAddress( pengine2d_Set, zglLib, "pengine2d_Set" );
    zglGetAddress( pengine2d_Get, zglLib, "pengine2d_Get" );
    zglGetAddress( pengine2d_Draw, zglLib, "pengine2d_Draw" );
    zglGetAddress( pengine2d_Proc, zglLib, "pengine2d_Proc" );
#ifdef __CPP__
	zglGetAddress( __pengine2d_AddEmitter, zglLib, "pengine2d_AddEmitter" );
#else
    zglGetAddress( pengine2d_AddEmitter, zglLib, "pengine2d_AddEmitter" );
#endif
    zglGetAddress( pengine2d_DelEmitter, zglLib, "pengine2d_DelEmitter" );
    zglGetAddress( pengine2d_ClearAll, zglLib, "pengine2d_ClearAll" );
    zglGetAddress( emitter2d_Add, zglLib, "emitter2d_Add" );
    zglGetAddress( emitter2d_Del, zglLib, "emitter2d_Del" );
    zglGetAddress( emitter2d_LoadFromFile, zglLib, "_emitter2d_LoadFromFile" );
    zglGetAddress( emitter2d_LoadFromMemory, zglLib, "emitter2d_LoadFromMemory" );
    zglGetAddress( emitter2d_Init, zglLib, "emitter2d_Init" );
    zglGetAddress( emitter2d_Free, zglLib, "emitter2d_Free" );
    zglGetAddress( emitter2d_Draw, zglLib, "emitter2d_Draw" );
    zglGetAddress( emitter2d_Proc, zglLib, "emitter2d_Proc" );

    zglGetAddress( font_Add, zglLib, "font_Add" );
    zglGetAddress( font_Del, zglLib, "font_Del" );
    zglGetAddress( font_LoadFromFile, zglLib, "_font_LoadFromFile" );
    zglGetAddress( font_LoadFromMemory, zglLib, "font_LoadFromMemory" );
#ifdef __CPP__
    zglGetAddress( __text_Draw, zglLib, "_text_Draw" );
    zglGetAddress( __text_DrawEx, zglLib, "_text_DrawEx" );
    zglGetAddress( __text_DrawInRect, zglLib, "_text_DrawInRect" );
    zglGetAddress( __text_DrawInRectEx, zglLib, "_text_DrawInRectEx" );
    zglGetAddress( __text_GetWidth, zglLib, "_text_GetWidth" );
    zglGetAddress( __text_GetHeight, zglLib, "_text_GetHeight" );
#else
    zglGetAddress( text_Draw, zglLib, "_text_Draw" );
    zglGetAddress( text_DrawEx, zglLib, "_text_DrawEx" );
    zglGetAddress( text_DrawInRect, zglLib, "_text_DrawInRect" );
    zglGetAddress( text_DrawInRectEx, zglLib, "_text_DrawInRectEx" );
    zglGetAddress( text_GetWidth, zglLib, "_text_GetWidth" );
    zglGetAddress( text_GetHeight, zglLib, "_text_GetHeight" );
#endif
    zglGetAddress( textFx_SetLength, zglLib, "textFx_SetLength" );

    zglGetAddress( snd_Init, zglLib, "snd_Init" );
    zglGetAddress( snd_Free, zglLib, "snd_Free" );
    zglGetAddress( snd_Add , zglLib, "snd_Add" );
    zglGetAddress( snd_Del , zglLib, "snd_Del" );
#ifdef __CPP__
    zglGetAddress( __snd_LoadFromFile, zglLib, "_snd_LoadFromFile" );
    zglGetAddress( __snd_LoadFromMemory, zglLib, "_snd_LoadFromMemory" );
    zglGetAddress( __snd_Play, zglLib, "snd_Play" );
#else
    zglGetAddress( snd_LoadFromFile, zglLib, "_snd_LoadFromFile" );
    zglGetAddress( snd_LoadFromMemory, zglLib, "_snd_LoadFromMemory" );
    zglGetAddress( snd_Play, zglLib, "snd_Play" );
#endif
    zglGetAddress( snd_Stop, zglLib, "snd_Stop" );
    zglGetAddress( snd_SetPos, zglLib, "snd_SetPos" );
    zglGetAddress( snd_SetVolume, zglLib, "snd_SetVolume" );
    zglGetAddress( snd_SetSpeed, zglLib, "snd_SetSpeed" );
    zglGetAddress( snd_Get, zglLib, "snd_Get" );
#ifdef __CPP__
    zglGetAddress( __snd_PlayFile, zglLib, "_snd_PlayFile" );
    zglGetAddress( __snd_PlayMemory, zglLib, "_snd_PlayMemory" );
#else
    zglGetAddress( snd_PlayFile, zglLib, "_snd_PlayFile" );
    zglGetAddress( snd_PlayMemory, zglLib, "_snd_PlayMemory" );
#endif
    zglGetAddress( snd_PauseStream, zglLib, "snd_PauseStream" );
    zglGetAddress( snd_StopStream, zglLib, "snd_StopStream" );
    zglGetAddress( snd_ResumeStream, zglLib, "snd_ResumeStream" );
    zglGetAddress( snd_SeekStream, zglLib, "snd_SeekStream" );

    zglGetAddress( video_Add, zglLib, "video_Add" );
    zglGetAddress( video_Del, zglLib, "video_Del" );
    zglGetAddress( video_OpenFile, zglLib, "_video_OpenFile" );
    zglGetAddress( video_OpenMemory, zglLib, "_video_OpenMemory" );
    zglGetAddress( video_Update, zglLib, "video_Update" );
    zglGetAddress( video_Seek, zglLib, "video_Seek" );

    zglGetAddress( m_Cos, zglLib, "m_Cos" );
    zglGetAddress( m_Sin, zglLib, "m_Sin" );
    zglGetAddress( m_Distance, zglLib, "m_Distance" );
    zglGetAddress( m_FDistance, zglLib, "m_FDistance" );
    zglGetAddress( m_Angle, zglLib, "m_Angle" );
    zglGetAddress( m_Orientation, zglLib, "m_Orientation" );

#ifdef __CPP__
    zglGetAddress( __tess_Triangulate, zglLib, "tess_Triangulate" );
    zglGetAddress( __tess_AddHole, zglLib, "tess_AddHole" );
#else
    zglGetAddress( tess_Triangulate, zglLib, "tess_Triangulate" );
    zglGetAddress( tess_AddHole, zglLib, "tess_AddHole" );
#endif
    zglGetAddress( tess_GetData, zglLib, "tess_GetData" );

    zglGetAddress( col2d_PointInRect, zglLib, "col2d_PointInRect" );
    zglGetAddress( col2d_PointInTriangle, zglLib, "col2d_PointInTriangle" );
    zglGetAddress( col2d_PointInCircle, zglLib, "col2d_PointInCircle" );
    zglGetAddress( col2d_Line, zglLib, "col2d_Line" );
    zglGetAddress( col2d_LineVsRect, zglLib, "col2d_LineVsRect" );
    zglGetAddress( col2d_LineVsCircle, zglLib, "col2d_LineVsCircle" );
    zglGetAddress( col2d_Rect, zglLib, "col2d_Rect" );
    zglGetAddress( col2d_ClipRect, zglLib, "col2d_ClipRect" );
    zglGetAddress( col2d_RectInRect, zglLib, "col2d_RectInRect" );
    zglGetAddress( col2d_RectInCircle, zglLib, "col2d_RectInCircle" );
    zglGetAddress( col2d_RectVsCircle, zglLib, "col2d_RectVsCircle" );
    zglGetAddress( col2d_Circle, zglLib, "col2d_Circle" );
    zglGetAddress( col2d_CircleInCircle, zglLib, "col2d_CircleInCircle" );
    zglGetAddress( col2d_CircleInRect, zglLib, "col2d_CircleInRect" );

    zglGetAddress( file_Open, zglLib, "_file_Open" );
    zglGetAddress( file_MakeDir, zglLib, "_file_MakeDir" );
    zglGetAddress( file_Remove, zglLib, "_file_Remove" );
    zglGetAddress( file_Exists, zglLib, "_file_Exists" );
    zglGetAddress( file_Seek, zglLib, "file_Seek" );
    zglGetAddress( file_GetPos, zglLib, "file_GetPos" );
    zglGetAddress( file_Read, zglLib, "file_Read" );
    zglGetAddress( file_Write, zglLib, "file_Write" );
    zglGetAddress( file_GetSize, zglLib, "file_GetSize" );
    zglGetAddress( file_Flush, zglLib, "file_Flush" );
    zglGetAddress( file_Close, zglLib, "file_Close" );
    zglGetAddress( file_Find, zglLib, "file_Find" );
    zglGetAddress( file_GetName, zglLib, "file_GetName" );
    zglGetAddress( file_GetExtension, zglLib, "file_GetExtension" );
    zglGetAddress( file_GetDirectory, zglLib, "file_GetDirectory" );
    zglGetAddress( file_SetPath, zglLib, "_file_SetPath" );

    zglGetAddress( file_OpenArchive, zglLib, "_file_OpenArchive" );
    zglGetAddress( file_CloseArchive, zglLib, "file_CloseArchive" );

    zglGetAddress( mem_LoadFromFile, zglLib, "mem_LoadFromFile" );
    zglGetAddress( mem_SaveToFile, zglLib, "mem_SaveToFile" );
    zglGetAddress( mem_Seek, zglLib, "mem_Seek" );
    zglGetAddress( mem_Read, zglLib, "mem_Read" );
    zglGetAddress( mem_Write, zglLib, "mem_Write" );
    zglGetAddress( mem_SetSize, zglLib, "mem_SetSize" );
    zglGetAddress( mem_Free, zglLib, "mem_Free" );

//    zglGetAddress( u_SortList, zglLib, "u_SortList" );
    zglGetAddress( u_Sleep, zglLib, "u_Sleep" );

    return TRUE;
  }
  else
  {
    #ifdef __LINUX__
    printf( "Error while loading ZenGL\n" );
    #endif
    #ifdef __WINDOWS__
    MessageBoxA( 0, "Error while loading ZenGL", "Error", 0x00000010 );
    #endif
    return FALSE;
  }
}

void zglFree()
{
  if ( zglLib )
    zglFreeLibrary( zglLib );
}
#else
extern bool zglLoad( const char* LibraryName );
extern void zglFree();
#endif

#endif

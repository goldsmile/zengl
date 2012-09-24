#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

zglPFont		fntMain;
zglPVideoStream	video;
bool			videoSeek;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void Init()
{
	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );

	// EN: Open the video file.
	// RU: Открыть видео файл.
	video = video_OpenFile( GetResource( "video.ogv" ) );
}

void Draw()
{
	if ( video )
	{
		// EN: Rendering the current video frame in the center of screen using parameters of it from video.Info.
		// RU: Рендеринг текущего кадра видео в центре экрана используя параметры из video.Info.
		ssprite2d_Draw( video->Texture, ( 800 - video->Info.Width ) / 2.f, ( 600 - video->Info.Height ) / 2.f, video->Info.Width, video->Info.Height, 0 );

		// EN: Rendering of progress bar.
		// RU: Рендеринг полосы прогресса.
		pr2d_Rect( 0, 600 - 100, 800, 20, 0x00FF00, 255 );
		pr2d_Rect( 0, 600 - 100, ( 800.f / (float)video->Info.Duration ) * (float)video->Time, 20, 0x00FF00, 155, PR2D_FILL );

		char text[64];
		sprintf_s( text, "FPS: %i", zgl_Get( RENDER_FPS ) );
		text_Draw( fntMain, 0, 0, text );

		sprintf_s( text, "Frame: %i", video->Frame );
		text_Draw( fntMain, 0, 20, text );

		sprintf_s( text, "Duration: %3.2f", video->Info.Duration / 1000.f );
		text_Draw( fntMain, 100, 0, text );

		sprintf_s( text, "Frames: %i", video->Info.Frames );
		text_Draw( fntMain, 100, 20, text );

		sprintf_s( text, "Time: %3.2f", video->Time / 1000.f );
		text_Draw( fntMain, 230, 0, text );
	}
}

void Timer()
{
	if ( key_Press( K_ESCAPE ) ) zgl_Exit();

	// EN: If left mouse button is down on progress bar, then seek the video.
	// RU: Если зажата левая кнопка мыши над полосой прогресса - перемещаться по видео.
	if ( mouse_Down( M_BLEFT ) && ( mouse_Y() > 500 ) && ( mouse_Y() < 520 ) )
	{
		videoSeek = TRUE;
		video_Seek( &video, ( mouse_X() / 800.f ) * video->Info.Duration );
	}
	else
		videoSeek = FALSE;

	key_ClearState();
	mouse_ClearState();
}

void Update( double dt )
{
	if ( !videoSeek )
		video_Update( &video, dt, TRUE );
}

int CALLBACK WinMain (
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in_opt LPSTR lpCmdLine,
	__in int nShowCmd
	)
{
	if ( !zglLoad( libZenGL ) ) return 0;

	srand( 0xDeaDBeeF );

	timer_Add( (void*)&Timer, 16 );

	zgl_Reg( SYS_LOAD, (void*)&Init );
	zgl_Reg( SYS_DRAW, (void*)&Draw );
	zgl_Reg( SYS_UPDATE, (void*)&Update );

	wnd_SetCaption( "15 - Video" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
#define ZGL_IMPORT

#include <memory.h>
#include "zglHeader.h"

zglPFont	fntMain;
zglPTexture	texBack;

char resource[256];

char* GetResource( char* FileName )
{
  sprintf_s( resource, "../data/%s", FileName );
  return resource;
}

void Init()
{
	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );
	texBack = tex_LoadFromFile( GetResource( "back03.jpg" ) );
}

void Draw()
{
	ssprite2d_Draw( texBack, 0, 0, 800, 600, 0 );

	text_Draw( fntMain, 0, 0, "Escape - Exit" );
	text_Draw( fntMain, 0, fntMain->MaxHeight * 1.0f, "F1 - Fullscreen with desktop resolution and correction of aspect" );
	text_Draw( fntMain, 0, fntMain->MaxHeight * 2.0f, "F2 - Fullscreen with desktop resolution and simple scaling" );
	text_Draw( fntMain, 0, fntMain->MaxHeight * 3.0f, "F3 - Fullscreen with resolution 800x600" );
	text_Draw( fntMain, 0, fntMain->MaxHeight * 4.0f, "F4 - Windowed mode" );
}

void Timer()
{
	// RU: Рекомендуемый к использованию полноэкранный режим. Основная идея - переключиться в полноэкранный режим используя текущее разрешение рабочего стола пользователя, но при этом
	//     сохранить пропорции изображения. Это позволит избежать некоторых проблем с LCD.
	// EN: Recommended fullscreen mode for using. Main idea is switching to fullscreen mode using current desktop resolution of user and saving the aspect. This will avoid some problems
	//     with LCD's.
	if ( key_Press( K_F1 ) )
	{
		// RU: Включить коррекцию пропорций.
		// EN: Enable aspect correction.
		zgl_Enable( CORRECT_RESOLUTION );
		// RU: Установить разрешение под которое изначально написано приложение.
		// EN: Set resolution for what application was wrote.
		scr_CorrectResolution( 800, 600 );
		scr_SetOptions( (ushort)zgl_Get( DESKTOP_WIDTH ), (ushort)zgl_Get( DESKTOP_HEIGHT ), REFRESH_MAXIMUM, TRUE, FALSE );
	}

	// RU: Схожий режим с предыдущим за одним исключением - отключена коррекция по ширине и высоте. Например, отключение коррекции по высоте может пригодиться при соотошении
	//     сторон 5:4(разрешение экрана 1280x1024), т.к. можно заполнить всю область экрана без существенных искажений.
	// EN: Similar mode to previous one with one exception - disabled correction for width and height. E.g. this can be useful for aspect 5:4(resolution 1280x1024),
	//     because screen can be filled without significant distortion.
	if ( key_Press( K_F2 ) )
	{
		zgl_Enable( CORRECT_RESOLUTION );
		zgl_Disable( CORRECT_WIDTH );
		zgl_Disable( CORRECT_HEIGHT );
		scr_CorrectResolution( 800, 600 );
		scr_SetOptions( (ushort)zgl_Get( DESKTOP_WIDTH ), (ushort)zgl_Get( DESKTOP_HEIGHT ), REFRESH_MAXIMUM, TRUE, FALSE );
	}

	// RU: Переключение в полноэкранный режим используя указанные размеры. В наше время такой подход имеет два больших недостатка на LCD:
	//     - если указываемое разрешение не является родным для LCD, то без специальных настройках в драйверах пользователь будет наблюдать пикселизацию
	//     - на широкоэкранных мониторах картинка с соотношением 4:3 будет смотрется растянутой
	// EN: Switching to fullscreen mode using set values. Nowadays this method two main problems with LCD:
	//     - if used resolution is not main for LCD, then without special options in drivers user will see pixelization
	//     - picture with aspect 4:3 will be stretched on widescreen monitors
	if ( key_Press( K_F3 ) )
	{
		zgl_Disable( CORRECT_RESOLUTION );
		scr_SetOptions( 800, 600, REFRESH_MAXIMUM, TRUE, FALSE );
	}

	// RU: Оконный режим.
	// EN: Windowed mode.
	if ( key_Press( K_F4 ) )
	{
		zgl_Disable( CORRECT_RESOLUTION );
		scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );
	}

	if ( key_Press( K_ESCAPE ) ) zgl_Exit();

	key_ClearState();
}

int CALLBACK WinMain (
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in_opt LPSTR lpCmdLine,
	__in int nShowCmd
	)
{
	if ( !zglLoad( libZenGL ) ) return 0;

	timer_Add( (void*)&Timer, 16 );

	zgl_Reg( SYS_LOAD, (void*)&Init );
	zgl_Reg( SYS_DRAW, (void*)&Draw );

	wnd_SetCaption( "04 - Screen Settings" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

typedef struct
{
	zglPTexture Texture;
	int         Frame;
	zglTPoint2D Pos;
} TTux;

zglPFont		fntMain;
zglPTexture		texLogo;
zglPTexture		texBack;
zglPTexture		texGround;
zglPTexture		texTuxWalk;
zglPTexture		texTuxStand;
TTux			tux[21];
int				time;
zglTCamera2D	camMain;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void Init()
{
	// RU: Т.к. по умолчанию вся структура камеры заполняется нулями, следует инициализировать её стандартными значениями.
	// EN: Camera must be initialized, because camera structure is zero-filled by default.
	cam2d_Init( &camMain );

	// RU: Загружаем текстуру.
	//     TEX_NO_COLORKEY - указывает на то, что бы использовать альфа-канал из изображения.
	//     TEX_DEFAULT_2D - комплекс флагов, необходимых для 2D-спрайтов. Описание есть в справке.
	// EN: Load the texture.
	//     TEX_NO_COLORKEY - means that alpha channel must be used from file, without colorkey.
	//     TEX_DEFAULT_2D - complex of flags that needed for 2D sprites. Description can be found in help.
	texLogo = tex_LoadFromFile( GetResource( "zengl.png" ), TEX_NO_COLORKEY, TEX_DEFAULT_2D );

	texBack = tex_LoadFromFile( GetResource( "back01.jpg" ) );

	texGround = tex_LoadFromFile( GetResource( "ground.png" ) );
	// RU: Указываем размер кадра в текстуре.
	// EN: Set the size of single frame for texture.
	tex_SetFrameSize( &texGround, 32, 32 );

	texTuxWalk = tex_LoadFromFile( GetResource( "tux_walking.png" ) );
	tex_SetFrameSize( &texTuxWalk, 64, 64 );
	texTuxStand = tex_LoadFromFile( GetResource( "tux_stand.png" ) );
	tex_SetFrameSize( &texTuxStand, 64, 64 );

	for ( int i = 0; i < 10; i++ )
	{
		tux[ i ].Texture = texTuxWalk;
		tux[ i ].Frame   = rand() % 19 + 2;
		tux[ i ].Pos.X   = (float)i * 96;
		tux[ i ].Pos.Y   = 32;
	}
	for ( int i = 10; i < 20; i++ )
	{
		tux[ i ].Texture = texTuxWalk;
		tux[ i ].Frame   = rand() % 19 + 2;
		tux[ i ].Pos.X   = (float)( i - 9 ) * 96;
		tux[ i ].Pos.Y   = 600 - 96;
	}
	tux[ 20 ].Texture = texTuxStand;
	tux[ 20 ].Frame   = rand() % 19 + 2;
	tux[ 20 ].Pos.X   = 400 - 32;
	tux[ 20 ].Pos.Y   = 300 - 64 - 4;

	// RU: Загружаем шрифт.
	// EN: Load the font.
	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );
}

void Draw()
{
	float t;

	batch2d_Begin();
	if ( time > 255 )
	{
		// RU: Для увеличения быстродействия можно отключить очистку буфера цвета, учитывая что экран полностью заполнен.
		// EN: Rendering perfomance can be increased by disabling clearing the color buffer. This is a good idea because screen is full of objects.
		zgl_Disable( COLOR_BUFFER_CLEAR );

		// RU: Рисуем задний фон с размерами 800х600 используя текстуру back.
		// EN: Render the background with size 800x600 and using texture "back".
		ssprite2d_Draw( texBack, 0, 0, 800, 600, 0 );

		// RU: Установить текущую камеру.
		// EN: Set the current camera.
		cam2d_Set( &camMain );

		// RU: Рисуем землю.
		// EN: Render the ground.
		for ( int i = -2; i <= 800 / 32 + 1; i++ )
			asprite2d_Draw( texGround, i * 32.0f, 96 - 12, 32, 32, 0, 2 );
		for ( int i = -2; i <= 800 / 32 + 1; i++ )
			asprite2d_Draw( texGround, i * 32.0f, 600 - 32 - 12, 32, 32, 0, 2 );

		// RU: Рисуем шагающих пингвинов.
		// EN: Render penguins
		for ( int i = 0; i < 10; i++ )
			if ( i == 2 )
			{
				// RU: Рисуем надпись в "рамочке" над пингвином.
				// EN: Render the text in frame over penguins.
				t = text_GetWidth( fntMain, "I'm so red...", 0 ) * 0.75f + 4;
				pr2d_Rect( tux[ i ].Pos.X - 2, tux[ i ].Pos.Y - fntMain->MaxHeight + 4, t, (float)fntMain->MaxHeight, 0x000000, 200, PR2D_FILL );
				pr2d_Rect( tux[ i ].Pos.X - 2, tux[ i ].Pos.Y - fntMain->MaxHeight + 4, t, (float)fntMain->MaxHeight, 0xFFFFFF, 255 );
				text_DrawEx( fntMain, tux[ i ].Pos.X, tux[ i ].Pos.Y - fntMain->MaxHeight + 8, 0.75, 0, "I'm so red...", 255, 0xFFFFFF );
				// RU: Рисуем красного пингвина используя fx2d-функцию и флаг FX_COLOR.
				// EN: Render red penguin using fx2d-function and flag FX_COLOR.
				fx2d_SetColor( 0xFF0000 );
				asprite2d_Draw( tux[ i ].Texture, tux[ i ].Pos.X, tux[ i ].Pos.Y, 64, 64, 0, tux[ i ].Frame / 2, 255, FX_BLEND | FX_COLOR );
			}
			else if ( i == 7 )
			{
				t = text_GetWidth( fntMain, "???", 0 ) * 0.75f + 4;
				pr2d_Rect( tux[ i ].Pos.X + 32 - t / 2, tux[ i ].Pos.Y - fntMain->MaxHeight + 4, t, (float)fntMain->MaxHeight, 0x000000, 200, PR2D_FILL );
				pr2d_Rect( tux[ i ].Pos.X + 32 - t / 2, tux[ i ].Pos.Y - fntMain->MaxHeight + 4, t, (float)fntMain->MaxHeight, 0xFFFFFF, 255 );
				text_DrawEx( fntMain, tux[ i ].Pos.X + 32, tux[ i ].Pos.Y - fntMain->MaxHeight + 8, 0.75, 0, "???", 255, 0xFFFFFF, TEXT_HALIGN_CENTER );
				// RU: Рисуем пингвина приведение используя флаг FX_COLOR установив режим в FX_COLOR_SET :)
				// EN: Render penguin ghost using flag FX_COLOR and mode FX_COLOR_SET :)
				fx_SetColorMode( FX_COLOR_SET );
				fx2d_SetColor( 0xFFFFFF );
				asprite2d_Draw( tux[ i ].Texture, tux[ i ].Pos.X, tux[ i ].Pos.Y, 64, 64, 0, tux[ i ].Frame / 2, 155, FX_BLEND | FX_COLOR );
				// RU: Возвращаем обычный режим.
				// EN: Return default mode.
				fx_SetColorMode( FX_COLOR_MIX );
			}
			else
				asprite2d_Draw( tux[ i ].Texture, tux[ i ].Pos.X, tux[ i ].Pos.Y, 64, 64, 0, tux[ i ].Frame / 2 );

		// RU: Рисуем пингвинов шагающих в обратную сторону используя флаг отражения текстуры FX2D_FLIPX.
		// EN: Render penguins, that go another way using special flag for flipping texture - FX2D_FLIPX.
		for ( int i = 10; i < 20; i++ )
			if ( i == 13 )
			{
				t = text_GetWidth( fntMain, "I'm so big...", 0 ) * 0.75f + 4;
				pr2d_Rect( tux[ i ].Pos.X - 2, tux[ i ].Pos.Y - fntMain->MaxHeight - 10, t, (float)fntMain->MaxHeight, 0x000000, 200, PR2D_FILL );
				pr2d_Rect( tux[ i ].Pos.X - 2, tux[ i ].Pos.Y - fntMain->MaxHeight - 10, t, (float)fntMain->MaxHeight, 0xFFFFFF, 255 );
				text_DrawEx( fntMain, tux[ i ].Pos.X, tux[ i ].Pos.Y - fntMain->MaxHeight - 4, 0.75, 0, "I'm so big...", 255, 0xFFFFFF );
				// RU: Рисуем "большего" пингвина. Т.к. FX2D_SCALE увеличивает спрайт относительно центра, то пингвина следует немного "поднять".
				// EN: Render "big" penguin. It must be shifted up, because FX2D_SCALE scale sprite relative to the center.
				fx2d_SetScale( 1.25, 1.25 );
				asprite2d_Draw( tux[ i ].Texture, tux[ i ].Pos.X, tux[ i ].Pos.Y - 8, 64, 64, 0, tux[ i ].Frame / 2, 255, FX_BLEND | FX2D_FLIPX | FX2D_SCALE );
			}
			else if ( i == 17 )
			{
				// RU: Рисуем "высокого" пингвина используя вместо флага FX2D_SCALE флаг FX2D_VCHANGE и функцию fx2d_SetVertexes для смещения координат двух верхних вершин спрайта.
				// EN: Render "tall" penguin using flag FX2D_VCHANGE instead of FX2D_SCALE, and function fx2d_SetVertexes for shifting upper vertexes of sprite.
				fx2d_SetVertexes( 0, -16, 0, -16, 0, 0, 0, 0 );
				asprite2d_Draw( tux[ i ].Texture, tux[ i ].Pos.X, tux[ i ].Pos.Y, 64, 64, 0, tux[ i ].Frame / 2, 255, FX_BLEND | FX2D_FLIPX | FX2D_VCHANGE );
			}
			else
				asprite2d_Draw( tux[ i ].Texture, tux[ i ].Pos.X, tux[ i ].Pos.Y, 64, 64, 0, tux[ i ].Frame / 2, 255, FX_BLEND | FX2D_FLIPX );

		// RU: Сбросить камеру.
		// EN: Reset the camera.
		cam2d_Set( NULL );

		// RU: Рисуем участок земли по центру экрана.
		// EN: Render piece of ground in the center of screen.
		asprite2d_Draw( texGround, 11 * 32, 300 - 16, 32, 32, 0, 1 );
		asprite2d_Draw( texGround, 12 * 32, 300 - 16, 32, 32, 0, 2 );
		asprite2d_Draw( texGround, 13 * 32, 300 - 16, 32, 32, 0, 3 );

		t = text_GetWidth( fntMain, "o_O", 0 ) * 0.75f + 4;
		pr2d_Rect( tux[ 20 ].Pos.X + 32 - t / 2, tux[ 20 ].Pos.Y - fntMain->MaxHeight + 4, t, (float)fntMain->MaxHeight, 0x000000, 200, PR2D_FILL );
		pr2d_Rect( tux[ 20 ].Pos.X + 32 - t / 2, tux[ 20 ].Pos.Y - fntMain->MaxHeight + 4, t, (float)fntMain->MaxHeight, 0xFFFFFF, 255 );
		text_DrawEx( fntMain, tux[ 20 ].Pos.X + 32, tux[ 20 ].Pos.Y - fntMain->MaxHeight + 8, 0.75, 0, "o_O", 255, 0xFFFFFF, TEXT_HALIGN_CENTER );
		asprite2d_Draw( tux[ 20 ].Texture, tux[ 20 ].Pos.X, tux[ 20 ].Pos.Y, 64, 64, 0, tux[ 20 ].Frame / 2 );
	}

	if ( time <= 255 )
		ssprite2d_Draw( texLogo, 400 - 256, 300 - 128, 512, 256, 0, time );
	else if ( time < 510 )
	{
		pr2d_Rect( 0, 0, 800, 600, 0x000000, 510 - time, PR2D_FILL );
		ssprite2d_Draw( texLogo, 400 - 256, 300 - 128, 512, 256, 0, 510 - time );
	}

	if ( time > 255 )
	{
		char fps[256];
		sprintf_s( fps, "FPS: %i", (int)zgl_Get( RENDER_FPS ) );
		text_Draw( fntMain, 0, 0, fps );
	}
	batch2d_End();
}

void Timer()
{
	time += 2;

	camMain.Angle = camMain.Angle + cosf( (float)time / 1000.0f ) / 10;

	for ( int i = 0; i <= 20; i++ )
	{
		tux[ i ].Frame++;
		if ( tux[ i ].Frame > 20 )
			tux[ i ].Frame = 2;
	}
	for ( int i = 0; i < 10; i++ )
	{
		tux[ i ].Pos.X = tux[ i ].Pos.X + 1.5f;
		if ( tux[ i ].Pos.X > 864 )
			tux[ i ].Pos.X = -96;
	}
	for ( int i = 10; i < 20; i++ )
	{
		tux[ i ].Pos.X = tux[ i ].Pos.X - 1.5f;
		if ( tux[ i ].Pos.X < -96 )
			tux[ i ].Pos.X = 864;
	}

	if ( key_Press( K_ESCAPE ) )
		zgl_Exit();
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

	srand( 0xDeaDBeeF );

	timer_Add( (void*)&Timer, 16 );

	zgl_Reg( SYS_LOAD, (void*)&Init );
	zgl_Reg( SYS_DRAW, (void*)&Draw );

	wnd_SetCaption( "07 - Sprites" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
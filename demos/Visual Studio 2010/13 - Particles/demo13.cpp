#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

zglPFont		fntMain;
zglPTexture		texBack;
bool			debug;
zglTPEngine2D	particles;
zglPEmitter2D	emitterFire[3];
zglPEmitter2D	emitterDiamond;
zglPEmitter2D	emitterRain;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void Init()
{
	texBack = tex_LoadFromFile( GetResource( "back02.png" ) );

	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );

	// EN: Load three types of fire emitters.
	// RU: Загрузка трёх разных видов эмиттеров огня.
	emitterFire[ 0 ] = emitter2d_LoadFromFile( GetResource( "emitter_fire00.zei" ) );
	emitterFire[ 1 ] = emitter2d_LoadFromFile( GetResource( "emitter_fire01.zei" ) );
	emitterFire[ 2 ] = emitter2d_LoadFromFile( GetResource( "emitter_fire02.zei" ) );

	// EN: Set own particels engine.
	// RU: Установка собственного движка эмиттеров.
	pengine2d_Set( &particles );

	// EN: Add 6 fire emitters to particles engine. Second parameter of function returns pointer to instance of new emitter, which can be processed manually.
	//     This instance will be  NULL after the death, so check everything.
	// RU: Добавляем в движок 6 эмиттеров огня. Второй параметр функции позволяет получить указатель на конкретный экземпляр эмиттера, который можно будет обрабатывать вручную.
	//     Данный экземпляр после смерти будет содержать NULL, поэтому используйте проверку.
	pengine2d_AddEmitter( emitterFire[ 0 ], NULL, 642, 190 );
	pengine2d_AddEmitter( emitterFire[ 0 ], NULL, 40, 368 );
	pengine2d_AddEmitter( emitterFire[ 0 ], NULL, 246, 368 );
	pengine2d_AddEmitter( emitterFire[ 1 ], NULL, 532, 244 );
	pengine2d_AddEmitter( emitterFire[ 1 ], NULL, 318, 422 );
	pengine2d_AddEmitter( emitterFire[ 1 ], NULL, 583, 420 );
	pengine2d_AddEmitter( emitterFire[ 2 ], NULL, 740, 525 );

	emitterDiamond = emitter2d_LoadFromFile( GetResource( "emitter_diamond.zei" ) );
	pengine2d_AddEmitter( emitterDiamond, NULL );

	emitterRain = emitter2d_LoadFromFile( GetResource( "emitter_rain.zei" ) );
	pengine2d_AddEmitter( emitterRain, NULL );
}

void Draw()
{
	batch2d_Begin();

	ssprite2d_Draw( texBack, 0, 0, 800, 600, 0 );

	// EN: Rendering of all emitters in current particles engine.
	// RU: Рендеринг всех эмиттеров в текущем движке частиц.
	pengine2d_Draw();

	if ( debug )
		for ( int i = 0; i < particles.Count.Emitters; i++ )
			pr2d_Rect( particles.List[ i ]->BBox.MinX, particles.List[ i ]->BBox.MinY,
						particles.List[ i ]->BBox.MaxX - particles.List[ i ]->BBox.MinX,
						particles.List[ i ]->BBox.MaxY - particles.List[ i ]->BBox.MinY, 0xFF0000, 255 );

	char text[64];
	sprintf_s( text, "FPS: %i", zgl_Get( RENDER_FPS ) );
	text_Draw( fntMain, 0, 0, text );

	sprintf_s( text, "Particles: %i", particles.Count.Particles );
	text_Draw( fntMain, 0, 20, text );

	sprintf_s( text, "Debug(F1): %s", debug ? "TRUE" : "FALSE" );
	text_Draw( fntMain, 0, 40, text );

	batch2d_End();
}

void Timer()
{
	if ( key_Press( K_ESCAPE ) ) zgl_Exit();
	if ( key_Press( K_F1 ) ) debug = !debug;

	key_ClearState();
}

void Update( double dt )
{
	// EN: Process all emitters in current particles engine.
	// RU: Обработка всех эмиттеров в текущем движке частиц.
	pengine2d_Proc( dt );
}

void Quit()
{
	// RU: Очищаем память от созданных эмиттеров.
	// EN: Free allocated memory for emitters.
	pengine2d_Set( &particles );
	pengine2d_ClearAll();
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
	zgl_Reg( SYS_UPDATE, (void*)&Update );
	zgl_Reg( SYS_EXIT, (void*)&Quit );

	wnd_SetCaption( "13 - Particles" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
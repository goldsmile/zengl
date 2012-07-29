#define ZGL_IMPORT

#include <memory.h>
#include "zglHeader.h"

// RU: У каждого ресурса есть свой определённый тип, который является указателем на структуру.
// EN: Every resource has its own typem which is just a pointer to structure.
zglPFont fntMain;
//
zglPTexture texLogo;
zglPTexture texTest;
//
zglPSound sndClick;
zglPSound sndMusic;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void TextureCalcEffect( byte *pData, short Width, short Height )
{
	u_Sleep( 1000 );
}

void Init()
{
	// RU: Более детальное рассмотрение параметров функций загрузки ресурсов есть в соответствующих примерах, тут же показана лишь основная суть.
	// EN: Description with more details about parameters of functions can be found in other demos, here is only main idea shown.

	snd_Init();

	// RU: Функции загрузки ресурсов именуются в формате "$(префикс)_LoadFrom$(откуда)", где "$(префикс)" может быть tex, snd, font и т.д., а "$(откуда)" - File и Memory.
	// EN: Functions for loading resources named in format "$(prefix)_LoadFrom$(where)", where "$(prefix)" can be tex, snd, font and so on, and $(where) - File and Memory.
	fntMain  = font_LoadFromFile( GetResource( "font.zfi" ) );
	texLogo  = tex_LoadFromFile( GetResource( "zengl.png" ) );
	sndClick = snd_LoadFromFile( GetResource( "click.wav" ) );

	// RU: Многопоточная загрузка ресурсов позволяет составить очередь и не ожидать загрузки выполняя другие операции, например рендеринг какой-то анимации.
	//     Процесс загрузки в многопоточном режиме практически ничем не отличается от обычного за исключением вызова функций старта и окончания очереди.
	// EN: Multithreaded resource loading allows to make queue and do something while loading, e.g. rendering some animation.
	//     Loading resources in multithreaded mode has almost no difference with standard mode, except using functions for beginning and ending queues.
	res_BeginQueue( 0 );
	// RU: Между res_BeginQueue и res_EndQueue могут использоваться все обычные функции загрузки ресурсов.
	//     Для задержки экрана загрузки ресурсы будут загружены несколько раз, а для текстуры будет использована обработка с задержкой.
	// EN: All standard functions for loading resources can be used between res_BeginQueue and res_EndQueue.
	//     Just for holding loading screen resources will be loaded multiple times, and texture will be post-processed with delay.
	zgl_Reg( TEX_CURRENT_EFFECT, (void*)&TextureCalcEffect );
	for ( int i = 0; i < 4; i++ )
	{
		texTest  = tex_LoadFromFile( GetResource( "back01.jpg" ) );
		sndMusic = snd_LoadFromFile( GetResource( "music.ogg" ) );
	}
	res_EndQueue();

	// RU: Для загрузки ресурсов из zip-архива необходимо его сначала "открыть" и потом "закрыть" :) Для этого существуют функции file_OpenArchive и file_CloseArchive.
	// EN: For loading resources from zip-archive this archive should be "opened" first and then "closed" :) There are functions file_OpenArchive and file_CloseArchive for this.
	file_OpenArchive( GetResource( "zengl.zip" ), NULL );
	texLogo = tex_LoadFromFile( "zengl.png" );
	file_CloseArchive();
}

void Draw()
{
	// RU: К ресурсам, которые загружаются в многопоточном режиме, можно обращаться только после завершения загрузки. Код ниже рисует экран загрузки если ресурсы ещё не загрузились.
	// EN: Resources which are loading in multithreaded mode can be used only after finishing the loading process. Code below renders loading screen if resources are not loaded yet.
	if ( res_GetCompleted() < 100 )
	{
		ssprite2d_Draw( texLogo, ( 800.0f - texLogo->Width ) / 2, ( 600.0f - texLogo->Height ) / 2, texLogo->Width, texLogo->Height, 0 );
		char text[64];
		sprintf_s( text, "Loading... %i%%", res_GetCompleted() );
		text_Draw( fntMain, 400.0f, 300.0f + texLogo->Height / 4, text, TEXT_HALIGN_CENTER );
		return;
	}

	ssprite2d_Draw( texTest, 0, 0, 800, 600, 0 );
	char text[64];
	sprintf_s( text, "FPS: %i", (int)zgl_Get( RENDER_FPS ) );
	text_Draw( fntMain, 0, 0, text, 0 );
	sprintf_s( text, "VRAM Used: %iMb", (int)zgl_Get( RENDER_VRAM_USED ) / 1024 / 1024 );
	text_Draw( fntMain, 0, 16, text );
}

int CALLBACK WinMain (
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in_opt LPSTR lpCmdLine,
	__in int nShowCmd
	)
{
	if ( !zglLoad( libZenGL ) ) return 0;

	zgl_Reg( SYS_LOAD, (void*)&Init );
	zgl_Reg( SYS_DRAW, (void*)&Draw );

	wnd_SetCaption( "02 - Resources" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}

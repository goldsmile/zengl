#define ZGL_IMPORT

#include <memory.h>
#include "zglHeader.h"

zglPFont fntMain;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void Init()
{
	// RU: Загружаем данные о шрифте.
	// EN: Load the font.
	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );
	// RU: Если же текстуры именуются без использования маски вида "$(имя_шрифта)FontName-page$(номер).$(расширение)", то загрузку можно произвести следующим образом(для png):
	// EN: If textures were named without special mask - "$(font_name)-page$(number).$(extension)", then use this method to load them(for png):
	//for ( int i = 0; i < fntMain.Count.Pages; i++ )
	//{
	//	char texture[32];
	//	sprintf_s( texture, "font-page%i.png", i );
	//	fntMain->Pages[ i ] = tex_LoadFromFile( GetResource( texture ) );
	//}
}

void Draw()
{
	zglTRect r;
	char str[256];

	batch2d_Begin();

	// RU: ZenGL работает исключительно с кодировкой UTF-8, поэтому весь текст должен быть в UTF-8.
	// EN: ZenGL works only with UTF-8 encoding, so all text should be encoded with UTF-8.

	text_Draw( fntMain, 400, 25, "String with center alignment", TEXT_HALIGN_CENTER );

	text_DrawEx( fntMain, 400, 65, 2, 0, "Scaling", 255, 0xFFFFFF, TEXT_HALIGN_CENTER );

	fx2d_SetVCA( 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 255, 255, 255, 255 );
	text_Draw( fntMain, 400, 125, "Gradient color for every symbol", TEXT_FX_VCA | TEXT_HALIGN_CENTER );

	r.X = 0;
	r.Y = 300 - 128;
	r.W = 192;
	r.H = 256;
	text_DrawInRect( fntMain, r, "Simple text rendering in rectangle", 0 );
	pr2d_Rect( r.X, r.Y, r.W, r.H, 0xFF0000, 255 );

	r.X = 800 - 192;
	r.Y = 300 - 128;
	r.W = 192;
	r.H = 256;
	text_DrawInRect( fntMain, r, "Text rendering using horizontal right alignment and vertical bottom alignment", TEXT_HALIGN_RIGHT | TEXT_VALIGN_BOTTOM );
	pr2d_Rect( r.X, r.Y, r.W, r.H, 0xFF0000, 255 );

	r.X = 400 - 192;
	r.Y = 300 - 128;
	r.W = 384;
	r.H = 256;
	text_DrawInRect( fntMain, r, "This text uses justify alignment and centered vertically. Text which doesn't fit inside the rectangle will be cropped.", TEXT_HALIGN_JUSTIFY | TEXT_VALIGN_CENTER );
	pr2d_Rect( r.X, r.Y, r.W, r.H, 0xFF0000, 255 );

	r.X = 400 - 320;
	r.Y = 300 + 160;
	r.W = 640;
	r.H = 128;
	text_DrawInRect( fntMain, r, "For starting new line LF symbol can be used\ncode of which is equal to 10 and named in Unicode as \"Line Feed\"", TEXT_HALIGN_CENTER | TEXT_VALIGN_CENTER );
	pr2d_Rect( r.X, r.Y, r.W, r.H, 0xFF0000, 255 );

	// RU: Выводим количество FPS в правом углу, используя text_GetWidth.
	// EN: Render frames per second in the top right corner using text_GetWidth.
	sprintf_s( str, "FPS: %i", (int)zgl_Get( RENDER_FPS ) );
	text_Draw( fntMain, 800 - text_GetWidth( fntMain, str ), 0, str );
	batch2d_End();
}

void Timer()
{
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

	wnd_SetCaption( "06 - Text" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
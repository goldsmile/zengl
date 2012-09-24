#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

zglPFont	fntMain;
zglTTiles2D	map;
zglPTexture	texTiles;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void Init()
{
	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );

	texTiles = tex_LoadFromFile( GetResource( "tiles.png" ) );
	tex_SetFrameSize( &texTiles, 32, 32 );

	// RU: Инициализация тайлов размером 32x32. Параметр Count указывает на количество тайлов по X и Y. Массив Tiles содержит кадры для каждого тайла.
	// EN: Initialization of tiles with size 32x32. Parameter Count set amount of tiles on X and Y. Array Tiles contains frames for every tile.
	map.Size.W  = 32;
	map.Size.H  = 32;
	map.Count.X = 25;
	map.Count.Y = 19;
	map.Tiles = (int**)malloc( sizeof( int* ) * map.Count.X );
	// RU: Заполняем карту "травой", 19 кадр.
	// EN: Fill the map by "grass", 19 frame.
	for ( int i = 0; i < map.Count.X; i++ )
	{
		map.Tiles[ i ] = (int*)malloc( sizeof( int ) * map.Count.Y );
		for ( int j = 0; j < map.Count.Y; j++ )
			map.Tiles[ i ][ j ] = 19;
	}

	// RU: Загружаем карту из бинарного файла.
	// EN: Load map from binary file.
	zglTFile f;
	file_Open( &f, GetResource( "ground.map" ), FOM_OPENR );
	for ( int i = 0; i < map.Count.X; i++ )
		file_Read( f, &map.Tiles[ i ][ 0 ], map.Count.Y * sizeof( int ) );
	file_Close( &f );
}

void Draw()
{
	// RU: Рендерим тайлы в координатах 0,0.
	// EN: Render tiles in coordinates 0,0.
	tiles2d_Draw( texTiles, 0, 0, &map );

	char text[64];
	sprintf_s( text, "FPS: %i", zgl_Get( RENDER_FPS ) );
	text_Draw( fntMain, 0, 0, text );

	text_Draw( fntMain, 180, 30, "This is a tarrible example of tile map, but main idea should be clear :)" );
}

void Timer()
{
	if ( key_Press( K_ESCAPE ) ) zgl_Exit();

	key_ClearState();
}

void Quit()
{
	for ( int i = 0; i < map.Count.X; i++ )
		free( map.Tiles[ i ] );
	free( map.Tiles );
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
	zgl_Reg( SYS_EXIT, (void*)&Quit );

	wnd_SetCaption( "10 - Tiles" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
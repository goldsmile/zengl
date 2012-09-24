#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

zglPFont	fntMain;
zglPTexture	texBack;
zglTGrid2D	grid;
float		wave;

char resource[256];

char* GetResource( char* FileName )
{
	sprintf_s( resource, "../data/%s", FileName );
	return resource;
}

void Init()
{
	texBack = tex_LoadFromFile( GetResource( "back04.jpg" ) );

	fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );

	// RU: Инициализация сетки размером 21x16. Основная идея - каждый узел сетки это смещение относительно её верхнего левого угла.
	// EN: Initialization of grid with size 21x16. Main idea - every node of grid is an offset from the top left corner.
	grid.Cols = 21;
	grid.Rows = 16;
	grid.Grid = (zglTPoint2D**)malloc( grid.Cols * sizeof( zglTPoint2D* ) );
	for ( int i = 0; i < grid.Cols; i++ )
	{
		grid.Grid[ i ] = (zglTPoint2D*)malloc( grid.Rows * sizeof( zglTPoint2D ) );
		for ( int j = 0; j < grid.Rows; j++ )
		{
			grid.Grid[ i ][ j ].X = i * 40.f;
			grid.Grid[ i ][ j ].Y = j * 40.f;
		}
	}
}

void Draw()
{
	// RU: Рендерим сетку в координатах 0,0.
	// EN: Render grid in coordinates 0,0.
	sgrid2d_Draw( texBack, 0, 0, &grid );

	char text[64];
	sprintf_s( text, "FPS: %i", zgl_Get( RENDER_FPS ) );
	text_Draw( fntMain, 0, 0, text );
}

void Timer()
{
	wave = wave + ( rand() % 1000 ) / 10000.f;
	float cwave = cos( wave );
	float swave = sin( wave );

	// RU: Симуляция простого эффекта под водой.
	// EN: Simulation of simple underwater effect.
	for ( int i = 1; i < grid.Cols - 1; i++ )
		for ( int j = 1; j < grid.Rows - 1; j++ )
		{
			if ( ( i % 2 == 0 ) && ( j % 2 == 0 ) )
			{
				grid.Grid[ i ][ j ].X = i * 40.f + cwave;
				grid.Grid[ i ][ j ].Y = j * 40.f + swave;
			}
			else
			{
				grid.Grid[ i ][ j ].X = i * 40.f - cwave;
				grid.Grid[ i ][ j ].Y = j * 40.f - swave;
			}
		}

	if ( key_Press( K_ESCAPE ) ) zgl_Exit();
	key_ClearState();
}

void Quit()
{
	for ( int i = 0; i < grid.Cols; i++ )
		free( grid.Grid[ i ] );
	free( grid.Grid );
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
	zgl_Reg( SYS_EXIT, (void*)&Quit );

	wnd_SetCaption( "11 - Grid" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
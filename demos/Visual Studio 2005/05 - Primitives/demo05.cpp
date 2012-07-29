#define ZGL_IMPORT

#include "zglHeader.h"

int calc;
zglTPoint2D points[360];

void Init()
{
	for ( int i = 0; i < 360; i++ )
	{
		points[ i ].X = 400 + m_Cos( i ) * ( 96 + rand() % 32 );
		points[ i ].Y = 300 + m_Sin( i ) * ( 96 + rand() % 32 );
	}
}

void Draw()
{
	// RU: Устанавливаем цвет и альфу для каждой вершины.
	// EN: Set color and alpha for each vertex.
	fx2d_SetVCA( 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 255, 255, 255, 255 );
	// RU: Рисуем прямоугольник с заливкой(флаг PR2D_FILL) с использованием отдельных цветов для каждой вершины(флаг FX2D_VCA).
	// EN: Render filled rectangle(flag PR2D_FILL) and use different colors for each vertex(flag FX2D_VCA).
	pr2d_Rect( 0, 0, 800, 600, 0x000000, 255, FX2D_VCA | PR2D_FILL );

	// RU: Рисуем в центре экрана круг с радиусом 128 пиксела.
	// EN: Render circle in the center of screen with radius 128 pixels.
	pr2d_Circle( 400, 300, 128, 0x000000, 155, 32, PR2D_FILL );

	calc++;
	if ( calc > 359 )
		calc = 0;
	points[ calc ].X = 400 + m_Cos( calc ) * ( 96 + rand() % 32 );
	points[ calc ].Y = 300 + m_Sin( calc ) * ( 96 + rand() % 32 );
	// RU: Рисуем линии внутри круга.
	// EN: Render lines inside the circle.
	for ( int i = 0; i < 360; i++ )
		pr2d_Line( 400, 300, points[ i ].X, points[ i ].Y, 0xFFFFFF, 255 );

	// RU: Рисуем эллипсы с заливкой и без, со сглаженными контурами(флаг PR2D_SMOOTH).
	// EN: Render filled ellipses with smoothed edges(flag PR2D_SMOOTH).
	pr2d_Ellipse( 400 + 300, 300, 64, 256, 0xFFFFFF, 55, 32, PR2D_FILL | PR2D_SMOOTH );
	pr2d_Ellipse( 400 + 300, 300, 64, 256, 0x000000, 255, 32, PR2D_SMOOTH );

	pr2d_Ellipse( 400 - 300, 300, 64, 256, 0xFFFFFF, 55, 32, PR2D_FILL | PR2D_SMOOTH );
	pr2d_Ellipse( 400 - 300, 300, 64, 256, 0x000000, 255, 32, PR2D_SMOOTH );
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

	srand( 0xDeaDBeeF );

	timer_Add( (void*)&Timer, 16 );

	zgl_Reg( SYS_LOAD, (void*)&Init );
	zgl_Reg( SYS_DRAW, (void*)&Draw );

	wnd_SetCaption( "05 - Primitives" );

	wnd_ShowCursor( TRUE );

	scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

	zgl_Init();

	zglFree();
	return 0;
}
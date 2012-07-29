#define ZGL_IMPORT

#include <math.h>
#include <memory.h>
#include "zglHeader.h"

typedef struct
{
  // RU: Обязательная часть нового типа спрайта.
  // EN: New type should start with this.
  ZGLTSPRITE2D

  // RU: Новые параметры.
  // EN: New params.
  zglTPoint2D Speed;
} zglTMiku, *zglPMiku;

zglPFont      fntMain;
zglPTexture   texLogo;
zglPTexture   texMiku;
int           time;
zglTSEngine2D sengine2d;

char resource[256];

char* GetResource( char* FileName )
{
#ifndef __MACOSX__
  sprintf_s( resource, "../data/%s", FileName );
  return resource;
#else
  return FileName;
#endif
}

// Miku
void MikuInit( zglPMiku Miku )
{
  Miku->X = 800.0f + rand() % 800;
  Miku->Y = (float)( rand() % ( 600 - 128 ) );
  // RU: Задаем скорость движения.
  // EN: Set the moving speed.
  Miku->Speed.X = -( rand() % 10 ) / 5.0f - 0.5f;
  Miku->Speed.Y = ( rand() % 10 - 5 ) / 5.0f;
}

void MikuDraw( zglPMiku Miku )
{
  asprite2d_Draw( Miku->Texture, Miku->X, Miku->Y, Miku->W, Miku->H, Miku->Angle, (ushort)Miku->Frame, Miku->Alpha, Miku->FxFlags );
}

void MikuProc( zglPMiku Miku )
{
  Miku->X += Miku->Speed.X;
  Miku->Y += Miku->Speed.Y;
  Miku->Frame += ( abs( Miku->Speed.X ) + abs( Miku->Speed.Y ) ) / 25.0f;
  if ( Miku->Frame > 8 )
    Miku->Frame = 1;

  // RU: Если спрайт выходит за пределы по X, сразу же удаляем его.
  // EN: Delete the sprite if it goes beyond X.
  if ( Miku->X < -128 ) sengine2d_DelSprite( Miku->ID );

  // RU: Если спрайт выходит за пределы по Y, ставим его в очередь на удаление.
  // EN: Add sprite to queue for delete if it goes beyond Y.
  if ( Miku->Y < -128 )
    Miku->Destroy = TRUE;
  if ( Miku->Y > 600 )
    Miku->Destroy = TRUE;
}

void MikuFree( zglPMiku Miku )
{
}

// RU: Добавить 100 спрайтов.
// EN: Add 100 sprites.
void AddMiku()
{
  // RU: При добавлении спрайта в менеджер спрайтов указывается текстура, слой(положение по Z) и указатели на основные функции - Инициализация, Рендер, Обработка и Уничтожение.
  // EN: For adding sprite to sprite engine must be set next parameters: texture, layer(Z-coordinate) and pointers to Initialization, Render, Process and Destroy functions.
  for ( int i = 1; i <= 100; i++ )
    sengine2d_AddCustom( texMiku, sizeof( zglTMiku ), rand() % 10, (zglSpriteFunc)&MikuInit, (zglSpriteFunc)&MikuDraw, (zglSpriteFunc)&MikuProc, (zglSpriteFunc)&MikuFree );
}

// RU: Удалить 100 спрайтов.
// EN: Delete 100 sprites.
void DelMiku()
{
  if ( sengine2d.Count < 100 )
    return;

  // RU: Удалим 100 спрайтов со случайным ID.
  // EN: Delete 100 sprites with random ID.
  for ( int i = 1; i <= 100; i++ )
    sengine2d_DelSprite( rand() % sengine2d.Count );
}

void Init()
{
  texLogo = tex_LoadFromFile( GetResource( "zengl.png" ), TEX_NO_COLORKEY, TEX_DEFAULT_2D );

  texMiku = tex_LoadFromFile( GetResource( "miku.png" ), TEX_NO_COLORKEY, TEX_DEFAULT_2D );
  tex_SetFrameSize( &texMiku, 128, 128 );

  // RU: Устанавливаем текущим менеджером спрайтов свой.
  // EN: Set own sprite engine as current.
  sengine2d_Set( &sengine2d );

  // RU: Создадим 1000 спрайтов Miku-chan :)
  // EN: Create 1000 sprites of Miku-chan :)
  for ( int i = 0; i < 10; i++ )
    AddMiku();

  fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );
}

void Draw()
{
  batch2d_Begin();
  // RU: Рисуем все спрайты находящиеся в текущем спрайтовом менеджере.
  // EN: Render all sprites contained in current sprite engine.
  if ( time > 255 )
    sengine2d_Draw();

  if ( time <= 255 )
    ssprite2d_Draw( texLogo, 400 - 256, 300 - 128, 512, 256, 0, time, FX_BLEND );
  else if ( time < 510 )
  {
    pr2d_Rect( 0, 0, 800, 600, 0x000000, 510 - time, PR2D_FILL );
    ssprite2d_Draw( texLogo, 400 - 256, 300 - 128, 512, 256, 0, 510 - time, FX_BLEND );
  }

  if ( time > 255 )
  {
    char text[256];

    pr2d_Rect( 0, 0, 256, 64, 0x000000, 200, PR2D_FILL );
    sprintf_s( text, "FPS: %i", (int)zgl_Get( RENDER_FPS ) );
    text_Draw( fntMain, 0, 0, text, 0 );
    sprintf_s( text, "Sprites: %i", sengine2d.Count );
    text_Draw( fntMain, 0, 20, text, 0 );
    text_Draw( fntMain, 0, 40, "Up/Down - Add/Delete Miku :)", 0 );
  }
  batch2d_End();
}

void Timer()
{
  time +=2;

  // RU: Выполняем обработку всех спрайтов в текущем спрайтовом менеджере.
  // EN: Process all sprites contained in current sprite engine.
  sengine2d_Proc();

  // RU: По нажатию пробела очистить все спрайты.
  // EN: Delete all sprites if space was pressed.
  if ( key_Press( K_SPACE ) ) sengine2d_ClearAll();
  if ( key_Press( K_UP ) ) AddMiku();
  if ( key_Press( K_DOWN ) ) DelMiku();

  if ( key_Press( K_ESCAPE ) ) zgl_Exit();

  key_ClearState();
}

void Quit()
{
  // RU: Очищаем память от созданных спрайтов.
  // EN: Free allocated memory for sprites.
  sengine2d_Set( &sengine2d );
  sengine2d_ClearAll();
}

int main()
{
  if ( !zglLoad( libZenGL ) ) return 0;

  srand( 0xDeaDBeeF );

  timer_Add( (void*)&Timer, 16, FALSE, NULL );
  timer_Add( (void*)&AddMiku, 1000, FALSE, NULL );

  zgl_Reg( SYS_LOAD, (void*)&Init );
  zgl_Reg( SYS_DRAW, (void*)&Draw );
  zgl_Reg( SYS_EXIT, (void*)&Quit );

  wnd_SetCaption( "08 - Sprite Engine" );

  wnd_ShowCursor( TRUE );

  scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

  zgl_Init( 0, 0 );

  zglFree();
  return 0;
}

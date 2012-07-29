#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

zglPFont         fntMain;
zglPTexture      texTux;
zglPRenderTarget rtFull;
zglPRenderTarget rtDefault;

char resource[256];

char* GetResource( char* FileName )
{
  sprintf_s( resource, "../data/%s", FileName );
  return resource;
}

void Init()
{
  texTux = tex_LoadFromFile( GetResource( "tux_stand.png" ), TEX_NO_COLORKEY, TEX_DEFAULT_2D );
  tex_SetFrameSize( &texTux, 64, 64 );

  fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );

  // RU: Создаем RenderTarget и "цепляем" пустую текстуру. В процессе текстуру можно сменить присвоив rtarget.Surface другую zglPTexture, главное что бы совпадали размеры с теми, что указаны в
  //     tex_CreateZero. Таргету также указан флаг RT_FULL_SCREEN, отвечающий за то, что бы в текстуру помещалось все содержимое экрана а не область 256x256(как с флагом RT_DEFAULT).
  // EN: Create a RenderTarget and "bind" empty texture to it. Later texture can be changed via changing rtarget.Surface to another zglPTexture, the only requirement - the same size, that was
  //     set in tex_CreateZero. Also target use flag RT_FULL_SCREEN that responsible for rendering whole content of screen into target, not only region 256x256(like with flag RT_DEFAULT).
  rtFull = rtarget_Add( tex_CreateZero( 256, 256, 0, TEX_DEFAULT_2D ), RT_FULL_SCREEN );

  // RU: Для сравнения создадим ещё один RenderTarget с флагом RT_DEFAULT.
  // EN: Create one more RenderTarget with flag RT_DEFAULT for comparison.
  rtDefault = rtarget_Add( tex_CreateZero( 256, 256, 0, TEX_DEFAULT_2D ), RT_DEFAULT );
}

void Draw()
{
  // RU: Устанавливаем текущий RenderTarget.
  // EN: Set current RenderTarget.
  rtarget_Set( rtFull );
  // RU: Рисуем в него.
  // EN: Render to it.
  asprite2d_Draw( texTux, (float)(rand() % ( 800 - 64 )), (float)(rand() % ( 600 - 64 )), 64.f, 64.f, 0, rand() % 9 + 1, 255, FX_BLEND );
  // RU: Возвращаемся к обычному рендеру.
  // EN: Return to default rendering.
  rtarget_Set( NULL );

  rtarget_Set( rtDefault );
  asprite2d_Draw( texTux, (float)(rand() % ( 800 - 64 )), (float)(rand() % ( 600 - 64 )), 64, 64, 0, rand() % 9 + 1, 255, FX_BLEND );
  rtarget_Set( NULL );

  // RU: Теперь рисуем содержимое RenderTarget'ов.
  // EN: Render content of RenderTargets.
  ssprite2d_Draw( rtFull->Surface, ( 400 - 256 ) / 2, ( 600 - 256 ) / 2, 256, 256, 0, 255, FX_BLEND );
  ssprite2d_Draw( rtDefault->Surface, ( 400 - 256 ) / 2 + 400, ( 600 - 256 ) / 2, 256, 256, 0, 255, FX_BLEND );

  char text[64];
  sprintf_s( text, "FPS: %i", (int)zgl_Get( RENDER_FPS ) );
  text_Draw( fntMain, 0, 0, text, 0 );
}

void Timer()
{
  if ( key_Press( K_ESCAPE ) ) zgl_Exit();
  key_ClearState();
}

int main()
{
  if ( !zglLoad( libZenGL ) ) return 0;

  srand( 0xDeaDBeeF );

  timer_Add( (void*)&Timer, 16, FALSE, NULL );

  zgl_Reg( SYS_LOAD, (void*)&Init );
  zgl_Reg( SYS_DRAW, (void*)&Draw );

  wnd_SetCaption( "12 - Render into Texture" );

  wnd_ShowCursor( TRUE );

  scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

  zgl_Init( 0, 0 );

  zglFree();
  return 0;
}
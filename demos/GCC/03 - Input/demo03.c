#define ZGL_IMPORT

#include <memory.h>
#include "zglHeader.h"

zglPFont fntMain;

int        joyCount;
const char *userInput;
bool       trackInput;
zglTRect   inputRect;
byte       lineAlpha;

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

void Init()
{
  fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );

  inputRect.X = 400 - 192;
  inputRect.Y = 300 - 100 - 32;
  inputRect.W = 384;
  inputRect.H = 96;

  // RU: Инициализируем обработку ввода джойстиков и получаем количество подключенных джойстиков.
  // EN: Initialize processing joystick input and get count of plugged joysticks.
  joyCount = joy_Init();
}

void Draw()
{
  float w;
  char text[64];

  text_Draw( fntMain, 0, 0, "Escape - Exit", 0 );

  // RU: Координаты мыши можно получить при помощи функций mouse_X и mouse_Y.
  // EN: Mouse coordinates can be got using functions mouse_X and mouse_Y.
  sprintf_s( text, "Mouse X, Y: %i; %i", mouse_X(), mouse_Y() );
  text_Draw( fntMain, 0, 16, text, 0 );

  // RU: Выводим введённый пользователем текст.
  // EN: Show the inputted text.
  pr2d_Rect( inputRect.X, inputRect.Y, inputRect.W, inputRect.H, 0xFFFFFF, 255, 0 );
  if ( trackInput )
  {
    text_Draw( fntMain, 400, 300 - 100, "Press Enter to stop track text input:", TEXT_HALIGN_CENTER );
    w = text_GetWidth( fntMain, userInput, 0 );
    pr2d_Rect( 400 + w / 2 + 2, 300 - 70, 10, 20, 0xFFFFFF, lineAlpha, PR2D_FILL );
  }
  else
    text_Draw( fntMain, 400, 300 - 100, "Click here to enter text(maximum - 24 symbols):", TEXT_HALIGN_CENTER );
  text_Draw( fntMain, 400, 300 - 70, userInput, TEXT_HALIGN_CENTER );


  // RU: Вывод состояния осей и кнопок первого джойстика в системе.
  // EN: Show the state of axes and buttons of first joystick in the system.
  sprintf_s( text, "JOYSTICK ( Found: %i )", joyCount );
  text_Draw( fntMain, 400, 360, text, TEXT_HALIGN_CENTER );

  sprintf_s( text, "Axis X: %1.2f", joy_AxisPos( 0, JOY_AXIS_X ) );
  text_Draw( fntMain, 100, 400, text, 0 );
  sprintf_s( text, "Axis Y: %1.2f", joy_AxisPos( 0, JOY_AXIS_Y ) );
  text_Draw( fntMain, 100, 420, text, 0 );
  sprintf_s( text, "Axis Z: %1.2f", joy_AxisPos( 0, JOY_AXIS_Z ) );
  text_Draw( fntMain, 100, 440, text, 0 );
  sprintf_s( text, "Axis R: %1.2f", joy_AxisPos( 0, JOY_AXIS_R ) );
  text_Draw( fntMain, 100, 460, text, 0 );
  sprintf_s( text, "Axis U: %1.2f", joy_AxisPos( 0, JOY_AXIS_U ) );
  text_Draw( fntMain, 100, 480, text, 0 );
  sprintf_s( text, "Axis V: %1.2f", joy_AxisPos( 0, JOY_AXIS_V ) );
  text_Draw( fntMain, 100, 500, text, 0 );
  sprintf_s( text, "POVX: %1.2f", joy_AxisPos( 0, JOY_POVX ) );
  text_Draw( fntMain, 100, 520, text, 0 );
  sprintf_s( text, "POVY: %1.2f", joy_AxisPos( 0, JOY_POVY ) );
  text_Draw( fntMain, 100, 540, text, 0 );

  sprintf_s( text, "Button1: %s", joy_Down( 0, 0 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 400, text, 0 );
  sprintf_s( text, "Button2: %s", joy_Down( 0, 1 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 420, text, 0 );
  sprintf_s( text, "Button3: %s", joy_Down( 0, 2 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 440, text, 0 );
  sprintf_s( text, "Button4: %s", joy_Down( 0, 3 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 460, text, 0 );
  sprintf_s( text, "Button5: %s", joy_Down( 0, 4 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 480, text, 0 );
  sprintf_s( text, "Button6: %s", joy_Down( 0, 5 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 500, text, 0 );
  sprintf_s( text, "Button7: %s", joy_Down( 0, 6 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 520, text, 0 );
  sprintf_s( text, "Button8: %s", joy_Down( 0, 7 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 400, 540, text, 0 );
  sprintf_s( text, "Button9: %s", joy_Down( 0, 8 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 400, text, 0 );
  sprintf_s( text, "Button10: %s", joy_Down( 0, 9 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 420, text, 0 );
  sprintf_s( text, "Button11: %s", joy_Down( 0, 10 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 440, text, 0 );
  sprintf_s( text, "Button12: %s", joy_Down( 0, 11 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 460, text, 0 );
  sprintf_s( text, "Button13: %s", joy_Down( 0, 12 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 480, text, 0 );
  sprintf_s( text, "Button14: %s", joy_Down( 0, 13 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 500, text, 0 );
  sprintf_s( text, "Button15: %s", joy_Down( 0, 14 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 520, text, 0 );
  sprintf_s( text, "Button16: %s", joy_Down( 0, 15 ) ? "TRUE" : "FALSE" );
  text_Draw( fntMain, 550, 540, text, 0 );
}

void Timer()
{
  if ( lineAlpha > 5 )
    lineAlpha -= 10;
  else
    lineAlpha = 255;

  // RU: Проверить нажата ли левая кнопка мыши в пределах inputRect и начать отслеживать ввод текста.
  // EN: Check if left mouse button was pressed inside inputRect and start to track text input.
  if ( mouse_Click( M_BLEFT ) && col2d_PointInRect( mouse_X(), mouse_Y(), inputRect ) )
  {
    trackInput = TRUE;
    key_BeginReadText( userInput, 24 );
  }

  // RU: Если был нажат Enter прекращаем отслеживать ввод текста.
  // EN: Finish to track text input if Enter was pressed.
  if ( key_Press( K_ENTER ) )
  {
    trackInput = FALSE;
    key_EndReadText();
  }

  // RU: Получаем введённый пользователем текст.
  // EN: Get inputted by user text.
  if ( trackInput )
    userInput = key_GetText();

  // RU: По нажатию Escape завершить приложение.
  // EN: If Escape was pressed - shutdown the application.
  if ( key_Press( K_ESCAPE ) ) zgl_Exit();

  // RU: Обязательно очищаем состояния всех подсистем ввода.
  // EN: Necessarily clear all the states of input subsystems.
  mouse_ClearState();
  key_ClearState();
  joy_ClearState();
}

int main()
{
  if ( !zglLoad( libZenGL ) ) return 0;

  timer_Add( (void*)&Timer, 16, FALSE, NULL );

  zgl_Reg( SYS_LOAD, (void*)&Init );
  zgl_Reg( SYS_DRAW, (void*)&Draw );

  wnd_SetCaption( "03 - Input" );

  wnd_ShowCursor( TRUE );

  scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );
  zgl_Init( 0, 0 );

  zglFree();
  return 0;
}

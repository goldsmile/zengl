// RU: Этот дефайн необходим только в главном модуле.
// EN: This define is needed only in main unit.
#define ZGL_IMPORT

#include "zglHeader.h"

char* DirApp;
char* DirHome;

void Init()
{
  // RU: Тут можно выполнять загрузку основных ресурсов.
  // EN: Here can be loading of main resources.
}

void Draw()
{
  // RU: Тут "рисуем" что угодно :)
  // EN: Here "draw" anything :)
}

void Update( double dt )
{
  // RU: Эта функция наземенима для реализация плавного движения чего-либо, т.к. точность таймеров ограничена FPS.
  // EN: This function is the best way to implement smooth moving of something, because accuracy of timers are restricted by FPS.
}

void Timer()
{
  // RU: Будем в заголовке показывать количество кадров в секунду.
  // EN: Caption will show the frames per second.
  char caption[256];
  sprintf_s( caption, "01 - Initialization[ FPS: %i ]", (int)zgl_Get( RENDER_FPS ) );
  wnd_SetCaption( caption );
}

void Quit()
{
  //
}

int main()
{
#ifdef __LINUX__
  // RU: В GNU/Linux все библиотеки принято хранить в /usr/lib, поэтому libZenGL.so должна быть предварительно установлена.
  //     Но zglLoad сначала проверить есть ли libZenGL.so рядом с исполняемым файлом.
  //
  // EN: In GNU/Linux all libraries placed in /usr/lib, so libZenGL.so must be installed before it will be used.
  //     But zglLoad will check first if there is libZenGL.so near executable file.
  if ( !zglLoad( libZenGL ) ) return 0;
#endif
#ifdef __WINDOWS__
  if ( !zglLoad( libZenGL ) ) return 0;
#endif
#ifdef __MACOSX__
  // RU: libZenGL.dylib следует предварительно поместить в каталог MyApp.app/Contents/Frameworks/, где MyApp.app - Bundle вашего приложения.
  //     Также следует упомянуть, что лог-файл будет создаваться в корневом каталоге поэтому либо отключайте его, либо указывайте свой путь и имя, как описано в справке.
  //
  // EN: libZenGL.dylib must be placed into this directory MyApp.app/Contents/Frameworks/, where MyApp.app - Bundle of your application.
  //     Also you must know, that log-file will be created in root directory, so you must disable a log, or choose your own path and name for it. How to do this you can find in documentation.
  if ( !zglLoad( libZenGL ) ) return 0;
#endif

  // RU: Для загрузки/создания каких-то своих настроек/профилей/etc. можно получить путь к домашенему каталогу пользователя, или к исполняемому файлу(не работает для GNU/Linux).
  // EN: For loading/creating your own options/profiles/etc. you can get path to user home directory, or to executable file(not works for GNU/Linux).
  DirApp  = (char*)zgl_Get( DIRECTORY_APPLICATION );
  DirHome = (char*)zgl_Get( DIRECTORY_HOME );

  // RU: Создаем таймер с интервалом 1000мс.
  // EN: Create a timer with interval 1000ms.
  timer_Add( (void*)&Timer, 1000, FALSE, NULL );

  // RU: Регистрируем процедуру, что выполнится сразу после инициализации ZenGL.
  // EN: Register the procedure, that will be executed after ZenGL initialization.
  zgl_Reg( SYS_LOAD, (void*)&Init );
  // RU: Регистрируем процедуру, где будет происходить рендер.
  // EN: Register the render procedure.
  zgl_Reg( SYS_DRAW, (void*)&Draw );
  // RU: Регистрируем процедуру, которая будет принимать разницу времени между кадрами.
  // EN: Register the procedure, that will get delta time between the frames.
  zgl_Reg( SYS_UPDATE, (void*)&Update );
  // RU: Регистрируем процедуру, которая выполнится после завершения работы ZenGL.
  // EN: Register the procedure, that will be executed after ZenGL shutdown.
  zgl_Reg( SYS_EXIT, (void*)&Quit );

  // RU: Устанавливаем заголовок окна.
  // EN: Set the caption of the window.
  wnd_SetCaption( "01 - Initialization" );

  // RU: Разрешаем курсор мыши.
  // EN: Allow to show the mouse cursor.
  wnd_ShowCursor( TRUE );

  // RU: Указываем первоначальные настройки.
  // EN: Set screen options.
  scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

  // RU: Инициализируем ZenGL.
  // EN: Initialize ZenGL.
  zgl_Init( 0, 0 );

  zglFree();
  return 0;
}

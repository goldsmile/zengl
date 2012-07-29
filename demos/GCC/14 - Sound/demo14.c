#define ZGL_IMPORT

#include <memory.h>
#include <math.h>
#include "zglHeader.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

zglPFont    fntMain;
zglPTexture icon[2];
zglPSound   sound;
int         audio;
int         state;

char resource[256];

char* GetResource( char* FileName )
{
  sprintf_s( resource, "../data/%s", FileName );
  return resource;
}

// RU: Т.к. звуковая подсистема нацелена на 3D, для позиционирования звуков в 2D нужны некоторые ухищрения.
// EN: Because sound subsystem using 3D, there is some tricky way to calculate sound position in 2D.
float CalcX2D( float X )
{
  return ( X - SCREEN_WIDTH / 2.f ) * ( 1.f / SCREEN_WIDTH / 2.f );
}

float CalcY2D( float Y )
{
  return ( Y - SCREEN_HEIGHT / 2.f ) * ( 1.f / SCREEN_HEIGHT / 2.f );
}

void Init()
{
  // RU: Инициализируем звуковую подсистему. Для Windows можно сделать выбор между DirectSound и OpenAL отредактировав файл zgl_config.cfg.
  // EN: Initializing sound subsystem. For Windows can be used DirectSound or OpenAL, see zgl_config.cfg.
  snd_Init();

  // RU: Загружаем звуковой файл и устанавливаем для него максимальноe количество проигрываемых источников в 2.
  // EN: Load the sound file and set maximum count of sources that can be played to 2.
  sound = snd_LoadFromFile( GetResource( "click.wav" ), 2 );

  // RU: Загружаем текстуры, которые будут индикаторами.
  // EN: Load the textures, that will be indicators.
  icon[ 0 ] = tex_LoadFromFile( GetResource( "audio-stop.png" ), TEX_NO_COLORKEY, TEX_DEFAULT_2D );
  icon[ 1 ] = tex_LoadFromFile( GetResource( "audio-play.png" ), TEX_NO_COLORKEY, TEX_DEFAULT_2D );

  fntMain = font_LoadFromFile( GetResource( "font.zfi" ) );
}

void Draw()
{
  zglTRect r;

  ssprite2d_Draw( icon[ state ], ( SCREEN_WIDTH - 128 ) / 2.f, ( SCREEN_HEIGHT - 128 ) / 2.f, 128.f, 128.f, 0, 255, FX_BLEND );
  text_Draw( fntMain, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 64.f, "Skillet - Comatose - Whispers In The Dark", TEXT_HALIGN_CENTER );

  r.X = ( SCREEN_WIDTH - 128 ) / 2.f;
  r.Y = ( SCREEN_HEIGHT - 128 ) / 2.f;
  r.W = 128.f;
  r.H = 128.f;
  if ( col2d_PointInRect( (float)mouse_X(), (float)mouse_Y(), r ) )
  {
    fx_SetBlendMode( FX_BLEND_ADD, TRUE );
    ssprite2d_Draw( icon[ state ], ( SCREEN_WIDTH - 132 ) / 2.f, ( SCREEN_HEIGHT - 132 ) / 2.f, 132.f, 132.f, 0, 155, FX_BLEND );
    fx_SetBlendMode( FX_BLEND_NORMAL, TRUE );
  }
}

void Timer()
{
  zglTRect r;
  int p;

  // RU: Проверяем играет ли музыка(1 - играет, 0 - не играет). Так же можно проверить и звуки - подставив zglPSound и ID вот так:
  //     snd_Get( Sound, ID...
  //     ID возвращается функцией snd_Play
  //
  // EN: Check if music playing(1 - playing, 0 - not playing). Sounds also can be checked this way - just use zglPSound and ID:
  //     snd_Get( Sound, ID...
  //     ID returns by function snd_Play.
  state = snd_Get( SND_STREAM, audio, SND_STATE_PLAYING );
  if ( state == 0 )
    audio = 0;

  if ( mouse_Click( M_BLEFT ) )
  {
    // RU: В данном случаи мы начинаем воспроизводить звук сразу в указанных координатах, но их можно менять и в процессе используя процедуру snd_SetPos.
    //     Важно: Для OpenAL можно позиционировать только mono-звуки
    //
    // EN: In this case, we begin to play the sound directly in these coordinates, but they can be changed later using procedure snd_SetPos.
    //     Important: OpenAL can position only mono-sounds.
    snd_Play( sound, FALSE, CalcX2D( (float)mouse_X() ), CalcY2D( (float)mouse_Y() ), 0, FALSE );

    r.X = ( SCREEN_WIDTH - 128 ) / 2.f;
    r.Y = ( SCREEN_HEIGHT - 128 ) / 2.f;
    r.W = 128.f;
    r.H = 128.f;
    if ( col2d_PointInRect( (float)mouse_X(), (float)mouse_Y(), r ) && ( audio == 0 ) )
      audio = snd_PlayFile( GetResource( "music.ogg" ), FALSE, SND_VOLUME_DEFAULT );
  }

  // RU: Получаем в процентах позицию проигрывания аудиопотока и ставим громкость для плавных переходов.
  // EN: Get position in percent's for audio stream and set volume for smooth playing.
  p = snd_Get( SND_STREAM, audio, SND_STATE_PERCENT );
  if ( ( p >= 0 ) && ( p < 25 ) )
    snd_SetVolume( SND_STREAM, audio, ( 1.f / 24.f ) * p );
  if ( ( p >= 75 ) && ( p < 100 ) )
    snd_SetVolume( SND_STREAM, audio, 1.f - ( 1.f / 24.f ) * ( p - 75 ) );

  if ( key_Press( K_ESCAPE ) ) zgl_Exit();

  key_ClearState();
  mouse_ClearState();
}

int main()
{
  if ( !zglLoad( libZenGL ) ) return 0;

  srand( 0xDeaDBeeF );

  timer_Add( (void*)&Timer, 16, FALSE, NULL );

  zgl_Reg( SYS_LOAD, (void*)&Init );
  zgl_Reg( SYS_DRAW, (void*)&Draw );

  wnd_SetCaption( "14 - Sound" );

  wnd_ShowCursor( TRUE );

  scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

  zgl_Init( 0, 0 );

  zglFree();
  return 0;
}
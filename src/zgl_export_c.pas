{
 *  Copyright © Andrey Kemka aka Andru
 *  mail: dr.andru@gmail.com
 *  site: http://zengl.org
 *
 *  This file is part of ZenGL.
 *
 *  ZenGL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  ZenGL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with ZenGL. If not, see http://www.gnu.org/licenses/
}
unit zgl_export_c;

{$I zgl_config.cfg}

interface
uses
  zgl_main,
  zgl_window,
  zgl_log,
  zgl_file,
  zgl_memory,
  zgl_keyboard,
  zgl_textures,
  zgl_font,
  zgl_text,
  {$IFDEF USE_PARTICLES}
  zgl_particles_2d,
  {$ENDIF}
  {$IFDEF USE_SOUND}
  zgl_sound,
  {$ENDIF}
  {$IFDEF USE_VIDEO}
  zgl_video,
  {$ENDIF}
  zgl_math_2d,
  zgl_utils;

procedure _wnd_SetCaption( NewCaption : PAnsiChar );

procedure _log_Add( Message : PAnsiChar; Timings : Boolean = TRUE );

procedure _key_BeginReadText( Text : PAnsiChar; MaxSymbols : Integer = -1 );
procedure _key_UpdateReadText( Text : PAnsiChar; MaxSymbols : Integer = -1 );

function _tex_LoadFromFile( FileName : PAnsiChar; TransparentColor : LongWord = $FF000000; Flags : LongWord = TEX_DEFAULT_2D ) : zglPTexture;
function _tex_LoadFromMemory( Memory : zglTMemory; Extension : PAnsiChar; TransparentColor : LongWord = $FF000000; Flags : LongWord = TEX_DEFAULT_2D ) : zglPTexture;

function _font_LoadFromFile( FileName : PAnsiChar ) : zglPFont;

procedure _text_Draw( Font : zglPFont; X, Y : Single; Text : PAnsiChar; Flags : LongWord = 0 );
procedure _text_DrawEx( Font : zglPFont; X, Y, Scale, Step : Single; Text : PAnsiChar; Alpha : Byte = 255; Color : LongWord = $FFFFFF; Flags : LongWord = 0 );
procedure _text_DrawInRect( Font : zglPFont; Rect : zglTRect; Text : PAnsiChar; Flags : LongWord = 0 );
procedure _text_DrawInRectEx( Font : zglPFont; Rect : zglTRect; Scale, Step : Single; Text : PAnsiChar; Alpha : Byte = 0; Color : LongWord = $FFFFFF; Flags : LongWord = 0 );
function  _text_GetWidth( Font : zglPFont; Text : PAnsiChar; Step : Single = 0.0 ) : Single;
function  _text_GetHeight( Font : zglPFont; Width : Single; Text : PAnsiChar; Scale : Single = 1.0; Step : Single = 0.0 ) : Single;

{$IFDEF USE_PARTICLES}
function _emitter2d_LoadFromFile( FileName : PAnsiChar ) : zglPEmitter2D;
{$ENDIF}

{$IFDEF USE_SOUND}
function _snd_LoadFromFile( FileName : PAnsiChar; SourceCount : Integer = 8 ) : zglPSound;
function _snd_LoadFromMemory( Memory : zglTMemory; Extension : PAnsiChar; SourceCount : Integer = 8 ) : zglPSound;
function _snd_PlayFile( FileName : PAnsiChar; Loop : Boolean = FALSE; Volume : Single = SND_VOLUME_DEFAULT ) : Integer;
function _snd_PlayMemory( Memory : zglTMemory; Extension : PAnsiChar; Loop : Boolean = FALSE; Volume : Single = SND_VOLUME_DEFAULT ) : Integer;
{$ENDIF}

{$IFDEF USE_VIDEO}
function _video_OpenFile( FileName : PAnsiChar ) : zglPVideoStream;
function _video_OpenMemory( Memory : zglTMemory; Extension : PAnsiChar ) : zglPVideoStream;
{$ENDIF}

{$IFDEF USE_ZIP}
function _file_OpenArchive( FileName : PAnsiChar; Password : PAnsiChar = '' ) : Boolean;
{$ENDIF}

implementation

procedure _wnd_SetCaption( NewCaption : PAnsiChar );
begin
  wnd_SetCaption( NewCaption );
end;

procedure _log_Add( Message : PAnsiChar; Timings : Boolean = TRUE );
begin
  log_Add( Message, Timings );
end;

procedure _key_BeginReadText( Text : PAnsiChar; MaxSymbols : Integer = -1 );
begin
  key_BeginReadText( Text, MaxSymbols );
end;

procedure _key_UpdateReadText( Text : PAnsiChar; MaxSymbols : Integer = -1 );
begin
  key_UpdateReadText( Text, MaxSymbols );
end;

function _tex_LoadFromFile( FileName : PAnsiChar; TransparentColor : LongWord = $FF000000; Flags : LongWord = TEX_DEFAULT_2D ) : zglPTexture;
begin
  Result := tex_LoadFromFile( FileName, TransparentColor, Flags );
end;

function _tex_LoadFromMemory( Memory : zglTMemory; Extension : PAnsiChar; TransparentColor : LongWord = $FF000000; Flags : LongWord = TEX_DEFAULT_2D ) : zglPTexture;
begin
  Result := tex_LoadFromMemory( Memory, Extension, TransparentColor, Flags );
end;

function _font_LoadFromFile( FileName : PAnsiChar ) : zglPFont;
begin
  Result := font_LoadFromFile( FileName );
end;

procedure _text_Draw( Font : zglPFont; X, Y : Single; Text : PAnsiChar; Flags : LongWord = 0 );
begin
  text_Draw( Font, X, Y, Text, Flags );
end;

procedure _text_DrawEx( Font : zglPFont; X, Y, Scale, Step : Single; Text : PAnsiChar; Alpha : Byte = 255; Color : LongWord = $FFFFFF; Flags : LongWord = 0 );
begin
  text_DrawEx( Font, X, Y, Scale, Step, Text, Alpha, Color, Flags );
end;

procedure _text_DrawInRect( Font : zglPFont; Rect : zglTRect; Text : PAnsiChar; Flags : LongWord = 0 );
begin
  text_DrawInRect( Font, Rect, Text, Flags );
end;

procedure _text_DrawInRectEx( Font : zglPFont; Rect : zglTRect; Scale, Step : Single; Text : PAnsiChar; Alpha : Byte = 0; Color : LongWord = $FFFFFF; Flags : LongWord = 0 );
begin
  text_DrawInRectEx( Font, Rect, Scale, Step, Text, Alpha, Color, Flags );
end;

function  _text_GetWidth( Font : zglPFont; Text : PAnsiChar; Step : Single = 0.0 ) : Single;
begin
  Result := text_GetWidth( Font, Text, Step );
end;

function  _text_GetHeight( Font : zglPFont; Width : Single; Text : PAnsiChar; Scale : Single = 1.0; Step : Single = 0.0 ) : Single;
begin
  Result := text_GetHeight( Font, Width, Text, Scale, Step );
end;

{$IFDEF USE_PARTICLES}
function _emitter2d_LoadFromFile( FileName : PAnsiChar ) : zglPEmitter2D;
begin
  Result := emitter2d_LoadFromFile( FileName );
end;
{$ENDIF}

{$IFDEF USE_SOUND}
function _snd_LoadFromFile( FileName : PAnsiChar; SourceCount : Integer = 8 ) : zglPSound;
begin
  Result := snd_LoadFromFile( FileName, SourceCount );
end;

function _snd_LoadFromMemory( Memory : zglTMemory; Extension : PAnsiChar; SourceCount : Integer = 8 ) : zglPSound;
begin
  Result := snd_LoadFromMemory( Memory, Extension, SourceCount );
end;

function _snd_PlayFile( FileName : PAnsiChar; Loop : Boolean = FALSE; Volume : Single = SND_VOLUME_DEFAULT ) : Integer;
begin
  Result := snd_PlayFile( FileName, Loop, Volume );
end;

function _snd_PlayMemory( Memory : zglTMemory; Extension : PAnsiChar; Loop : Boolean = FALSE; Volume : Single = SND_VOLUME_DEFAULT ) : Integer;
begin
  Result := snd_PlayMemory( Memory, Extension, Loop, Volume );
end;
{$ENDIF}

{$IFDEF USE_VIDEO}
function _video_OpenFile( FileName : PAnsiChar ) : zglPVideoStream;
begin
  Result := video_OpenFile( FileName );
end;

function _video_OpenMemory( Memory : zglTMemory; Extension : PAnsiChar ) : zglPVideoStream;
begin
  Result := video_OpenMemory( Memory, Extension );
end;
{$ENDIF}

{$IFDEF USE_ZIP}
function _file_OpenArchive( FileName : PAnsiChar; Password : PAnsiChar = '' ) : Boolean;
begin
  Result := file_OpenArchive( FileName, Password );
end;
{$ENDIF}

end.

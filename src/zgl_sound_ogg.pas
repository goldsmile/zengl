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
unit zgl_sound_ogg;

{$I zgl_config.cfg}

interface
uses
  zgl_types,
  zgl_memory;

const
  OGG_EXTENSION : UTF8String = 'OGG';

procedure ogg_LoadFromFile( const FileName : UTF8String; out Data : PByteArray; out Size, Format, Frequency : LongWord );
procedure ogg_LoadFromMemory( const Memory : zglTMemory; out Data : PByteArray; out Size, Format, Frequency : LongWord );

implementation
uses
  zgl_lib_ogg,
  zgl_main,
  zgl_sound,
  zgl_file;

var
  oggDecoder : zglTSoundDecoder;

function ogg_Read( ptr : pointer; size, nmemb : csize_t; datasource : pointer ) : csize_t; cdecl;
begin
  Result := file_Read( zglTFile( datasource^ ), ptr^, size * nmemb );
end;

function ogg_Seek( datasource : pointer; offset : cint64; whence : cint ) : cint; cdecl;
begin
  case whence of
    0: file_Seek( zglTFile( datasource^ ), offset, FSM_SET );
    1: file_Seek( zglTFile( datasource^ ), offset, FSM_CUR );
    2: file_Seek( zglTFile( datasource^ ), offset, FSM_END );
  end;
  Result := 0;
end;

function ogg_Close( datasource : pointer ) : cint; cdecl;
begin
  file_Close( zglTFile( datasource^ ) );
  Result := 0;
end;

function ogg_CloseMem( datasource : pointer ) : cint; cdecl;
begin
  Result := 0;
end;

function ogg_GetPos( datasource : pointer ) : clong; cdecl;
begin
  Result := file_GetPos( zglTFile( datasource^ ) );
end;


function ogg_ReadMem( ptr : pointer; size, nmemb : csize_t; datasource : pointer ) : csize_t; cdecl;
begin
  Result := mem_Read( zglTMemory( datasource^ ), ptr^, size * nmemb );
end;

function ogg_SeekMem( datasource : pointer; offset : cint64; whence : cint ) : cint; cdecl;
begin
  case whence of
    0: mem_Seek( zglTMemory( datasource^ ), offset, FSM_SET );
    1: mem_Seek( zglTMemory( datasource^ ), offset, FSM_CUR );
    2: mem_Seek( zglTMemory( datasource^ ), offset, FSM_END );
  end;
  Result := 0;
end;

function ogg_GetPosMem( datasource : pointer ) : clong; cdecl;
begin
  Result := zglTMemory( datasource^ ).Position;
end;

function ogg_DecoderOpen( var Stream : zglTSoundStream; const FileName : UTF8String ) : Boolean;
begin
  Result := FALSE;
  if not vorbisInit Then exit;

  file_Open( Stream._file, FileName, FOM_OPENR );
  zgl_GetMem( Stream._data, SizeOf( zglTOggStream ) );

  with zglTOggStream( Stream._data^ ) do
    begin
      vc.read  := @ogg_Read;
      vc.seek  := @ogg_Seek;
      vc.close := @ogg_Close;
      vc.tell  := @ogg_GetPos;
      if ov_open_callbacks( @Stream._file, vf, nil, 0, vc ) >= 0 Then
        begin
          vi                := ov_info( vf, -1 );
          Stream.Bits       := 16;
          Stream.Frequency  := vi.rate;
          Stream.Channels   := vi.channels;
          Stream.Duration   := ov_pcm_total( vf, -1 ) / vi.rate * 1000;
          Stream.BufferSize := 64 * 1024;
          zgl_GetMem( Pointer( Stream.Buffer ), Stream.BufferSize );
          Result := TRUE;
        end;
        ov_pcm_seek( vf, 0 );
    end;
end;

function ogg_DecoderOpenMem( var Stream : zglTSoundStream; const Memory : zglTMemory ) : Boolean;
begin
  Result := FALSE;
  if not vorbisInit Then exit;

  zgl_GetMem( Stream._data, SizeOf( zglTOggStream ) );
  Stream._memory := Memory;

  with zglTOggStream( Stream._data^ ) do
    begin
      FillChar( vc, SizeOf( vc ), 0 );
      vc.read  := @ogg_ReadMem;
      vc.seek  := @ogg_SeekMem;
      vc.close := @ogg_CloseMem;
      vc.tell  := @ogg_GetPosMem;
      if ov_open_callbacks( @Stream._memory, vf, Pointer( @PByteArray( Memory.Memory )[ Memory.Position ] ), Memory.Size - Memory.Position, vc ) >= 0 Then
        begin
          vi                := ov_info( vf, -1 );
          Stream.Bits       := 16;
          Stream.Frequency  := vi.rate;
          Stream.Channels   := vi.channels;
          Stream.Duration   := ov_pcm_total( vf, -1 ) / vi.rate * 1000;
          Stream.BufferSize := 64 * 1024;
          zgl_GetMem( Pointer( Stream.Buffer ), Stream.BufferSize );
          Result := TRUE;
        end;
      ov_pcm_seek( vf, 0 );
    end;
end;

function ogg_DecoderRead( var Stream : zglTSoundStream; Buffer : PByteArray; Bytes : LongWord; out _End : Boolean ) : LongWord;
  var
    bytesRead : LongWord;
begin
  Result := 0;
  if not vorbisInit Then exit;

  bytesRead := 0;
  repeat
    Result := ov_read( zglTOggStream( Stream._data^ ).vf, Pointer( @Buffer[ bytesRead ] ), Bytes - bytesRead, {$IFDEF USE_VORBIS} BIG_ENDIAN, 2, TRUE, {$ENDIF} nil );
    bytesRead := bytesRead + Result;
  until ( Result = 0 ) or ( bytesRead = Bytes );

  _End   := Result = 0;
  Result := bytesRead;
end;

procedure ogg_DecoderSeek( var Stream : zglTSoundStream; Milliseconds : Double );
begin
  if not vorbisInit Then exit;

  ov_time_seek( zglTOggStream( Stream._data^ ).vf, {$IFDEF USE_VORBIS} Milliseconds / 1000 {$ELSE} Round( Milliseconds ) {$ENDIF} );
end;

procedure ogg_DecoderLoop( var Stream : zglTSoundStream );
begin
  if not vorbisInit Then exit;

  ov_pcm_seek( zglTOggStream( Stream._data^ ).vf, 0 );
end;

procedure ogg_DecoderClose( var Stream : zglTSoundStream );
begin
  if not vorbisInit Then exit;
  if not Assigned( zglTOggStream( Stream._data^ ).vi ) Then exit;
  zglTOggStream( Stream._data^ ).vi := nil;
  ov_clear( zglTOggStream( Stream._data^ ).vf );
end;

function decoderRead( var VorbisFile : OggVorbis_File; Buffer : PByteArray; Bytes : LongWord; out _End : Boolean ) : LongWord;
  var
    bytesRead : LongWord;
begin
  bytesRead := 0;
  repeat
    Result := ov_read( VorbisFile, Pointer( @Buffer[ bytesRead ] ), Bytes - bytesRead, {$IFDEF USE_VORBIS} BIG_ENDIAN, 2, TRUE, {$ENDIF} nil );
    bytesRead := bytesRead + Result;
  until ( Result = 0 ) or ( bytesRead = Bytes );

  _End   := Result = 0;
  Result := bytesRead;
end;

procedure ogg_LoadFromFile( const FileName : UTF8String; out Data : PByteArray; out Size, Format, Frequency : LongWord );
  var
    oggMemory : zglTMemory;
begin
  mem_LoadFromFile( oggMemory, FileName );
  ogg_LoadFromMemory( oggMemory, Data, Size, Format, Frequency );
  mem_Free( oggMemory );
end;

procedure ogg_LoadFromMemory( const Memory : zglTMemory; out Data : PByteArray; out Size, Format, Frequency : LongWord );
  var
    bytesRead : Integer;
    buffer    : Pointer;
    _end      : Boolean;

    _vi : pvorbis_info;
    _vf : OggVorbis_File;
    _vc : ov_callbacks;
begin
  if not vorbisInit Then exit;

  FillChar( _vc, SizeOf( _vc ), 0 );
  if ov_open_callbacks( nil, _vf, Pointer( @PByteArray( Memory.Memory )[ Memory.Position ] ), Memory.Size - Memory.Position, _vc ) >= 0 Then
    begin
      _vi       := ov_info( _vf, -1 );
      Frequency := _vi.rate;
      case _vi.channels of
        1: format := SND_FORMAT_MONO16;
        2: format := SND_FORMAT_STEREO16;
      end;

      Size := 0;
      zgl_GetMem( Buffer, 64 * 1024 );
      repeat
        bytesRead := decoderRead( _vf, Buffer, 64 * 1024, _End );
        INC( Size, bytesRead );
      until _End;
      FreeMem( Buffer );
      ov_clear( _vf );

      if ov_open_callbacks( nil, _vf, Pointer( @PByteArray( Memory.Memory )[ Memory.Position ] ), Memory.Size - Memory.Position, _vc ) >= 0 Then
        begin
          GetMem( Data, Size );
          decoderRead( _vf, Data, Size, _end );
          ov_clear( _vf );
        end;
    end;
end;

{$IFDEF USE_OGG}
initialization
  oggDecoder.Ext     := OGG_EXTENSION;
  oggDecoder.Open    := ogg_DecoderOpen;
  oggDecoder.OpenMem := ogg_DecoderOpenMem;
  oggDecoder.Read    := ogg_DecoderRead;
  oggDecoder.Seek    := ogg_DecoderSeek;
  oggDecoder.Loop    := ogg_DecoderLoop;
  oggDecoder.Close   := ogg_DecoderClose;
  zgl_Reg( SND_FORMAT_EXTENSION,   @OGG_EXTENSION[ 1 ] );
  zgl_Reg( SND_FORMAT_FILE_LOADER, @ogg_LoadFromFile );
  zgl_Reg( SND_FORMAT_MEM_LOADER,  @ogg_LoadFromMemory );
  zgl_Reg( SND_FORMAT_DECODER,     @oggDecoder );
{$ENDIF}

end.

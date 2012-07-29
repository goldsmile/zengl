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
unit zgl_sound_wav;

{$I zgl_config.cfg}

interface

uses
  zgl_types,
  zgl_memory;

const
  WAV_EXTENSION : UTF8String = 'WAV';

procedure wav_LoadFromFile( const FileName : UTF8String; out Data : PByteArray; out Size, Format, Frequency : LongWord );
procedure wav_LoadFromMemory( const Memory : zglTMemory; out Data : PByteArray; out Size, Format, Frequency : LongWord );

implementation
uses
  zgl_main,
  zgl_file,
  zgl_sound,
  zgl_log;

const
  WAV_STANDARD  = $0001;
  WAV_IMA_ADPCM = $0011;
  WAV_MP3       = $0055;

type
  zglTWAVHeader = record
    RIFFHeader       : array[ 1..4 ] of AnsiChar;
    FileSize         : Integer;
    WAVEHeader       : array[ 1..4 ] of AnsiChar;
    FormatHeader     : array[ 1..4 ] of AnsiChar;
    FormatHeaderSize : Integer;
    FormatCode       : Word;
    ChannelNumber    : Word;
    SampleRate       : LongWord;
    BytesPerSecond   : LongWord;
    BytesPerSample   : Word;
    BitsPerSample    : Word;
 end;

procedure wav_LoadFromFile( const FileName : UTF8String; out Data : PByteArray; out Size, Format, Frequency : LongWord );
  var
    wavMemory : zglTMemory;
begin
  mem_LoadFromFile( wavMemory, FileName );
  wav_LoadFromMemory( wavMemory, Data, Size, Format, Frequency );
  mem_Free( wavMemory );
end;

procedure wav_LoadFromMemory( const Memory : zglTMemory; out Data : PByteArray; out Size, Format, Frequency : LongWord );
  var
    wavMemory : zglTMemory;
    wavHeader : zglTWAVHeader;
    chunkName : array[ 0..3 ] of AnsiChar;
    skip      : Integer;
begin
  wavMemory := Memory;
  mem_Read( wavMemory, wavHeader, SizeOf( zglTWAVHeader ) );

  Frequency := wavHeader.SampleRate;

  if wavHeader.ChannelNumber = 1 Then
    case WavHeader.BitsPerSample of
      8:  format := SND_FORMAT_MONO8;
      16: format := SND_FORMAT_MONO16;
    end;

  if WavHeader.ChannelNumber = 2 then
    case WavHeader.BitsPerSample of
      8:  format := SND_FORMAT_STEREO8;
      16: format := SND_FORMAT_STEREO16;
    end;

  mem_Seek( wavMemory, ( 8 - 44 ) + 12 + 4 + wavHeader.FormatHeaderSize + 4, FSM_CUR );
  repeat
    mem_Read( wavMemory, chunkName, 4 );
    if chunkName = 'data' then
      begin
        mem_Read( wavMemory, Size, 4 );

        zgl_GetMem( Pointer( Data ), Size );
        mem_Read( wavMemory, Data[ 0 ], Size );

        if wavHeader.FormatCode = WAV_IMA_ADPCM Then log_Add( 'Unsupported wav format - IMA ADPCM' );
        if wavHeader.FormatCode = WAV_MP3 Then       log_Add( 'Unsupported wav format - MP3' );
        wavHeader.FormatCode := WAV_STANDARD; // just for case, because some wav-encoders write here garbage...
      end else
        begin
          mem_Read( wavMemory, skip, 4 );
          mem_Seek( wavMemory, skip, FSM_CUR );
        end;
  until wavMemory.Position >= wavMemory.Size;
end;

{$IFDEF USE_WAV}
initialization
  zgl_Reg( SND_FORMAT_EXTENSION,   @WAV_EXTENSION[ 1 ] );
  zgl_Reg( SND_FORMAT_FILE_LOADER, @wav_LoadFromFile );
  zgl_Reg( SND_FORMAT_MEM_LOADER,  @wav_LoadFromMemory );
{$ENDIF}

end.

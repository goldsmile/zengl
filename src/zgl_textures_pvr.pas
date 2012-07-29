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
unit zgl_textures_pvr;

{$I zgl_config.cfg}

interface

uses
  zgl_types,
  zgl_memory;

const
  PVR_EXTENSION : UTF8String = 'PVR';

procedure pvr_LoadFromFile( const FileName : UTF8String; out Data : PByteArray; out W, H, Format : Word );
procedure pvr_LoadFromMemory( const Memory : zglTMemory; out Data : PByteArray; out W, H, Format : Word );

implementation
uses
  CFByteOrders,
  zgl_main,
  zgl_file,
  zgl_textures;

const
  PVR_RGBA_4444 = $10;
  PVR_RGBA_5551 = $11;
  PVR_RGBA_8888 = $12;
  PVR_RGB_565   = $13;
  PVR_RGB_555   = $14;
  PVR_RGB_888   = $15;
  PVR_I_8       = $16;
  PVR_AI_88     = $17;
  PVR_PVRTC2    = $18;
  PVR_PVRTC4    = $19;

type
  zglTPVRHeader = record
    HeaderLength : LongWord;
    Height       : LongWord;
    Width        : LongWord;
    NumMipmaps   : LongWord;
    Flags        : LongWord;
    DataLength   : LongWord;
    BPP          : LongWord;
    BitmaskRed   : LongWord;
    BitmaskGreen : LongWord;
    BitmaskBlue  : LongWord;
    BitmaskAlpha : LongWord;
    PVRTag       : LongWord;
    NumSurfs     : LongWord;
  end;

procedure pvr_LoadFromFile( const FileName : UTF8String; out Data : PByteArray; out W, H, Format : Word );
  var
    pvrMem : zglTMemory;
begin
  mem_LoadFromFile( pvrMem, FileName );
  pvr_LoadFromMemory( pvrMem, Data, W, H, Format );
  mem_Free( pvrMem );
end;

procedure pvr_LoadFromMemory( const Memory : zglTMemory; out Data : PByteArray; out W, H, Format : Word );
  var
    pvrMem    : zglTMemory;
    pvrHeader : zglTPVRHeader;
    size      : LongWord;
    flags     : LongWord;
begin
  pvrMem := Memory;
  mem_Read( pvrMem, pvrHeader, SizeOf( zglTPVRHeader ) );
  W     := CFSwapInt32LittleToHost( pvrHeader.Width );
  H     := CFSwapInt32LittleToHost( pvrHeader.Height );
  size  := CFSwapInt32LittleToHost( pvrHeader.DataLength );
  flags := CFSwapInt32LittleToHost( pvrHeader.Flags ) and $FF;
  case flags of
    PVR_RGBA_4444: Format := TEX_FORMAT_RGBA_4444;
    PVR_RGBA_8888: Format := TEX_FORMAT_RGBA;
    PVR_PVRTC2: Format := TEX_FORMAT_RGBA_PVR2;
    PVR_PVRTC4: Format := TEX_FORMAT_RGBA_PVR4;
  else
    Data := nil;
    exit;
  end;

  GetMem( Data, size );
  Move( PByteArray( pvrMem.Memory )[ pvrMem.Position ], Data[ 0 ], size );
end;

{$IFDEF USE_PVR}
initialization
  zgl_Reg( TEX_FORMAT_EXTENSION,   @PVR_EXTENSION[ 1 ] );
  zgl_Reg( TEX_FORMAT_FILE_LOADER, @pvr_LoadFromFile );
  zgl_Reg( TEX_FORMAT_MEM_LOADER,  @pvr_LoadFromMemory );
{$ENDIF}

end.

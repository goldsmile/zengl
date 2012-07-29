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
unit zgl_types;

{$I zgl_config.cfg}

interface

{$IFNDEF FPC}
type
  QWORD = UInt64;
{$ENDIF}

type
  Ptr = {$IFDEF CPU64} QWORD {$ELSE} LongWord {$ENDIF};

  PByteArray     = ^TByteArray;
  TByteArray     = array[ 0..65535 ] of Byte;
  PWordArray     = ^TWordArray;
  TWordArray     = array[ 0..32767 ] of Word;
  PLongWordArray = ^TLongWordArray;
  TLongWordArray = array[ 0..16383 ] of LongWord;

type
  zglPTexCoordIndex = ^zglTTexCoordIndex;
  zglTTexCoordIndex = array[ 0..3 ] of Integer;

type
  zglTStringList = record
    Count : Integer;
    Items : array of UTF8String;
end;

{***********************************************************************}
{                       POSIX TYPE DEFINITIONS                          }
{***********************************************************************}
type
  cint8   = shortint; pcint8   = ^cint8;
  cuint8  = byte;     pcuint8  = ^cuint8;
  cchar   = cint8;    pcchar   = ^cchar;
  cschar  = cint8;    pcschar  = ^cschar;
  cuchar  = cuint8;   pcuchar  = ^cuchar;
  cint16  = smallint; pcint16  = ^cint16;
  cuint16 = word;     pcuint16 = ^cuint16;
  cint32  = longint;  pcint32  = ^cint32;
  cuint32 = longword; pcuint32 = ^cuint32;
  cint    = cint32;   pcint    = ^cint;
  csint   = cint32;   pcsint   = ^csint;
  cuint   = cuint32;  pcuint   = ^cuint;
  cuint64 = qword;    pcuint64 = ^cuint64;
  cint64  = int64;    pcint64  = ^cint64;
  cbool   = longbool; pcbool   = ^cbool;
{$IFDEF CPUx86_64}
  clong   = int64;    pclong   = ^clong;
  cslong  = int64;    pcslong  = ^cslong;
  culong  = qword;    pculong  = ^culong;
{$ELSE}
  clong   = longint;  pclong   = ^clong;
  cslong  = longint;  pcslong  = ^cslong;
  culong  = cardinal; pculong  = ^culong;
{$ENDIF}
  cfloat  = single;   pcfloat  = ^cfloat;
  cdouble = double;   pcdouble = ^cdouble;

  csize_t = culong;

implementation

end.


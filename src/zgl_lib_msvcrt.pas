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
unit zgl_lib_msvcrt;

{$I zgl_config.cfg}

interface
uses
  Windows,
  zgl_types;

{$IFDEF FPC}
  {$LINKLIB libmsvcrt.a}
{$ENDIF}

  // I hate hacks >_<
  {$IFDEF FPC}
  procedure __chkstk_ms; cdecl; public name '___chkstk_ms';
  function kernel32_MoveFileExA( lpExistingFileName : PAnsiChar; lpNewFileName : PAnsiChar; dwFlags : DWORD ) : Boolean; stdcall; public name '_MoveFileExA@12'; public name '__imp_MoveFileExA';
  {$IFDEF NAME_MANGLING}
  {$IFDEF WIN64}
  function msvcrt_pow( x, y : Double ) : Double; cdecl; public name 'pow';
  function msvcrt_ldexp( x : Double; exp : cint ) : Double; cdecl; public name 'ldexp';
  {$ENDIF}
  function msvcrt_memcmp( ptr1 : Pointer; ptr2 : Pointer; num : csize_t ) : cint; cdecl; public name '_memcmp';
  {$ENDIF}

  {$ELSE}
  procedure _llmul; cdecl;
  procedure errno; cdecl; external 'msvcrt.dll' name '_errno';
  procedure fstat; cdecl; external 'msvcrt.dll' name '_fstat';
  procedure dup; cdecl; external 'msvcrt.dll' name '_dup';
  {$ENDIF}

  function MoveFileExA( lpExistingFileName : PAnsiChar; lpNewFileName : PAnsiChar; dwFlags : DWORD ) : Boolean; stdcall; external 'kernel32.dll';
  function fseek( stream : Pointer; offset : clong; origin : cint ) : cint; cdecl; external 'msvcrt.dll';
  function ftell( stream : Pointer ) : clong; cdecl; external 'msvcrt.dll';
  function _stat( path : PAnsiChar; var buffer ) : cint; cdecl; external 'msvcrt.dll';
  function _fstat( handle : cint; var buffer ) : cint; cdecl; external 'msvcrt.dll';
  function pow( x, y : Double ) : Double; cdecl; external 'msvcrt.dll';
  function ldexp( x : Double; exp : cint ) : Double; cdecl; external 'msvcrt.dll';

  procedure memcpy; cdecl; external 'msvcrt.dll';
  procedure memset; cdecl; external 'msvcrt.dll';
  procedure malloc; cdecl; external 'msvcrt.dll';
  procedure free; cdecl; external 'msvcrt.dll';
  procedure _creat; cdecl; external 'msvcrt.dll';
  procedure _mktemp; cdecl; external 'msvcrt.dll';
  procedure _errno; cdecl; external 'msvcrt.dll';
  procedure _strdup; cdecl; external 'msvcrt.dll';
  procedure strlen; cdecl; external 'msvcrt.dll';
  procedure remove; cdecl; external 'msvcrt.dll';
  procedure qsort; cdecl; external 'msvcrt.dll';
  procedure fclose; cdecl; external 'msvcrt.dll';
  procedure fopen; cdecl; external 'msvcrt.dll';
  procedure fread; cdecl; external 'msvcrt.dll';
  procedure fwrite; cdecl; external 'msvcrt.dll';
  procedure _snprintf; cdecl; external 'msvcrt.dll';
  procedure sprintf; cdecl; external 'msvcrt.dll';
  procedure _fdopen; cdecl; external 'msvcrt.dll';
  procedure _fileno cdecl; external 'msvcrt.dll';
  procedure _close; cdecl; external 'msvcrt.dll';
  procedure _setmode; cdecl; external 'msvcrt.dll';
  procedure _stricmp; external 'msvcrt.dll';
  procedure realloc; cdecl; external 'msvcrt.dll';
  function memcmp( ptr1 : Pointer; ptr2 : Pointer; num : csize_t ) : cint; cdecl; external 'msvcrt.dll';
  procedure mktime; cdecl; external 'msvcrt.dll';
  procedure fputc; cdecl; external 'msvcrt.dll';
  procedure localtime; cdecl; external 'msvcrt.dll';
  procedure strerror; cdecl; external 'msvcrt.dll';
  procedure _dup; cdecl; external 'msvcrt.dll';
  procedure strcmp; cdecl; external 'msvcrt.dll';
  procedure strrchr; cdecl; external 'msvcrt.dll';
  procedure clearerr; cdecl; external 'msvcrt.dll';
  procedure strncmp; cdecl; external 'msvcrt.dll';
  procedure strtoul; cdecl; external 'msvcrt.dll';
  procedure memchr; cdecl; external 'msvcrt.dll';
  procedure time; cdecl; external 'msvcrt.dll';

implementation

{$IFDEF FPC}

procedure __chkstk_ms;
begin
end;

function kernel32_MoveFileExA( lpExistingFileName : PAnsiChar; lpNewFileName : PAnsiChar; dwFlags : DWORD ) : Boolean;
begin
  Result := MoveFileExA( lpExistingFileName, lpNewFileName, dwFlags );
end;

{$IFDEF NAME_MANGLING}

{$IFDEF WIN64}
function msvcrt_pow( x, y : Double ) : Double;
begin
  Result := pow( x, y );
end;

function msvcrt_ldexp( x : Double; exp : cint ) : Double;
begin
  Result := ldexp( x, exp );
end;
{$ENDIF}

function msvcrt_memcmp( ptr1 : Pointer; ptr2 : Pointer; num : csize_t ) : cint;
begin
  Result := memcmp( ptr1, ptr2, num );
end;
{$ENDIF}

{$ELSE}

procedure _llmul; cdecl;
asm
  push  edx
  push  eax

  mov   eax, [esp+16]
  mul   dword ptr [esp]
  mov   ecx, eax

  mov   eax, [esp+4]
  mul   dword ptr [esp+12]
  add   ecx, eax

  mov   eax, [esp]
  mul   dword ptr [esp+12]
  add   edx, ecx

  pop   ecx
  pop   ecx

  ret     8
end;

{$ENDIF}

end.

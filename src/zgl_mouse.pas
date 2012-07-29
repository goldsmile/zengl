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
unit zgl_mouse;

{$I zgl_config.cfg}

interface
{$IFDEF USE_X11}
  uses X, XLib;
{$ENDIF}
{$IFDEF WINDOWS}
  uses Windows;
{$ENDIF}
{$IFDEF MACOSX}
  uses MacOSAll;
{$ENDIF}

const
  M_BLEFT   = 0;
  M_BMIDDLE = 1;
  M_BRIGHT  = 2;
  M_WUP     = 0;
  M_WDOWN   = 1;

function mouse_X : Integer;
function mouse_Y : Integer;
function mouse_DX : Integer;
function mouse_DY : Integer;
function mouse_Down( Button : Byte ) : Boolean;
function mouse_Up( Button : Byte ) : Boolean;
function mouse_Click( Button : Byte ) : Boolean;
function mouse_DblClick( Button : Byte ) : Boolean;
function mouse_Wheel( Axis : Byte ) : Boolean;
procedure mouse_ClearState;
procedure mouse_Lock( X : Integer = -1; Y : Integer = -1 );

var
  mouseX        : Integer;
  mouseY        : Integer;
  mouseDX       : Integer;
  mouseDY       : Integer;
  mouseLX       : Integer;
  mouseLY       : Integer;
  mouseDown     : array[ 0..2 ] of Boolean;
  mouseUp       : array[ 0..2 ] of Boolean;
  mouseClick    : array[ 0..2 ] of Boolean;
  mouseCanClick : array[ 0..2 ] of Boolean;
  mouseDblClick : array[ 0..2 ] of Boolean;
  mouseDblCTime : array[ 0..2 ] of Double;
  mouseDblCInt  : Integer = 250;
  mouseWheel    : array[ 0..1 ] of Boolean;
  mouseLock     : Boolean;

  // callback
  mouse_PMove    : procedure( X, Y : Integer );
  mouse_PPress   : procedure( Button : Byte );
  mouse_PRelease : procedure( Button : Byte );
  mouse_PWheel   : procedure( Axis : Byte );

implementation
uses
  zgl_window,
  zgl_screen
  {$IF DEFINED(iOS) or DEFINED(ANDROID)}
  , zgl_touch
  {$IFEND}
  ;

function mouse_X : Integer;
begin
  Result := mouseX;
end;

function mouse_Y : Integer;
begin
  Result := mouseY;
end;

function mouse_DX : Integer;
begin
  Result := mouseDX;
end;

function mouse_DY : Integer;
begin
  Result := mouseDY;
end;

function mouse_Down( Button : Byte ) : Boolean;
begin
  {$IFDEF UNIX}
  Result := mouseDown[ Button ];
  {$ENDIF}
  {$IFDEF WINDOWS}
  if GetSystemMetrics( SM_SWAPBUTTON ) = 0 Then
    begin
      case Button of
        M_BLEFT:   Result := GetAsyncKeyState( VK_LBUTTON ) and $8000 <> 0;
        M_BMIDDLE: Result := GetAsyncKeyState( VK_MBUTTON ) and $8000 <> 0;
        M_BRIGHT:  Result := GetAsyncKeyState( VK_RBUTTON ) and $8000 <> 0;
      else
        Result := FALSE;
      end;
    end else
      case Button of
        M_BLEFT:   Result := GetAsyncKeyState( VK_RBUTTON ) and $8000 <> 0;
        M_BMIDDLE: Result := GetAsyncKeyState( VK_MBUTTON ) and $8000 <> 0;
        M_BRIGHT:  Result := GetAsyncKeyState( VK_LBUTTON ) and $8000 <> 0;
      else
        Result := FALSE;
      end;
  {$ENDIF}
end;

function mouse_Up( Button : Byte ) : Boolean;
begin
  Result := mouseUp[ Button ];
end;

function mouse_Click( Button : Byte ) : Boolean;
begin
  Result := mouseClick[ Button ];
end;

function mouse_DblClick( Button : Byte ) : Boolean;
begin
  Result := mouseDblClick[ Button ];
end;

function mouse_Wheel( Axis : Byte ) : Boolean;
begin
  Result := mouseWheel[ Axis ];
end;

procedure mouse_ClearState;
begin
  FillChar( mouseUp[ 0 ], 3, 0 );
  FillChar( mouseClick[ 0 ], 3, 0 );
  FillChar( mouseDblClick[ 0 ], 3, 0 );
  FillChar( mouseCanClick[ 0 ], 3, 1 );
  FillChar( mouseWheel[ 0 ], 2, 0 );
  {$IF DEFINED(iOS) or DEFINED(ANDROID)}
  touch_ClearState();
  {$IFEND}
end;

procedure mouse_Lock( X : Integer = -1; Y : Integer = -1 );
  {$IFDEF MACOSX}
  var
    Point : CGPoint;
  {$ENDIF}
begin
{$IFDEF USE_X11}
  if ( X = -1 ) and ( Y = -1 ) Then
    begin
      X := wndWidth div 2;
      Y := wndHeight div 2;
    end;

  XWarpPointer( scrDisplay, None, wndHandle, 0, 0, 0, 0, X, Y );
{$ENDIF}
{$IFDEF WINDOWS}
  if ( X = -1 ) and ( Y = -1 ) Then
    begin
      if wndFullScreen Then
        begin
          X := wndWidth div 2;
          Y := wndHeight div 2;
        end else
          begin
            X := wndX + wndBrdSizeX + wndWidth div 2;
            Y := wndY + wndBrdSizeY + wndCpnSize + wndHeight div 2;
          end;
    end else
      begin
        X := wndX + X;
        Y := wndY + Y;
      end;

  SetCursorPos( X, Y );
{$ENDIF}
{$IFDEF MACOSX}
  if ( X = -1 ) and ( Y = -1 ) Then
    begin
      Point.X := wndX + wndWidth / 2;
      Point.Y := wndY + wndHeight / 2;
    end else
      begin
        Point.X := wndX + X;
        Point.Y := wndY + Y;
      end;
  CGWarpMouseCursorPosition( Point );
{$ENDIF}
end;

end.

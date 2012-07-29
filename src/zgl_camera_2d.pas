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
unit zgl_camera_2d;

{$I zgl_config.cfg}

interface
uses
  zgl_math_2d;

type
  zglPCamera2D = ^zglTCamera2D;
  zglTCamera2D = record
    X, Y   : Single;
    Angle  : Single;
    Zoom   : zglTPoint2D;
    Center : zglTPoint2D;
  end;

type
  zglPCameraSystem = ^zglTCameraSystem;
  zglTCameraSystem = record
    Global : zglPCamera2D;
    Apply  : Boolean;
    OnlyXY : Boolean;
    CX     : Single;
    CY     : Single;
    ZoomX  : Single;
    ZoomY  : Single;
  end;

procedure cam2d_Init( out Camera : zglTCamera2D );
procedure cam2d_Set( Camera : zglPCamera2D );
function  cam2d_Get : zglPCamera2D;

var
  constCamera2D : zglTCamera2D = ( X: 0; Y: 0; Angle: 0; Zoom: ( X: 1; Y: 1 ); Center: ( X: 0; Y: 0 ) );
  cam2d         : zglPCameraSystem;
  cam2dTarget   : array[ 1..2 ] of zglTCameraSystem;

implementation
uses
  zgl_screen,
  {$IFNDEF USE_GLES}
  zgl_opengl,
  zgl_opengl_all,
  {$ELSE}
  zgl_opengles,
  zgl_opengles_all,
  {$ENDIF}
  zgl_render_2d;

procedure cam2d_Init( out Camera : zglTCamera2D );
begin
  Camera.X        := 0;
  Camera.Y        := 0;
  Camera.Angle    := 0;
  Camera.Zoom.X   := 1;
  Camera.Zoom.Y   := 1;
  Camera.Center.X := ( oglWidth - scrSubCX ) / 2;
  Camera.Center.Y := ( oglHeight - scrSubCY ) / 2;
end;

procedure cam2d_Set( Camera : zglPCamera2D );
begin
  batch2d_Flush();

  if cam2d.Apply Then
    glPopMatrix();

  if Assigned( Camera ) Then
    begin
      cam2d.Global := Camera;
      cam2d.Apply  := TRUE;
      cam2d.OnlyXY := ( cam2d.Global.Angle = 0 ) and ( cam2d.Global.Zoom.X = 1 ) and ( cam2d.Global.Zoom.Y = 1 );
      if ( cam2d.ZoomX <> cam2d.Global.Zoom.X ) or ( cam2d.ZoomY <> cam2d.Global.Zoom.Y ) Then
        render2dClipR := Round( sqrt( sqr( ( oglWidth - scrSubCX ) / cam2d.Global.Zoom.X ) + sqr( ( oglHeight - scrSubCY ) / cam2d.Global.Zoom.Y ) ) / 1.5 );
      cam2d.CX     := cam2d.Global.X + Camera.Center.X;
      cam2d.CY     := cam2d.Global.Y + Camera.Center.Y;
      cam2d.ZoomX  := cam2d.Global.Zoom.X;
      cam2d.ZoomY  := cam2d.Global.Zoom.Y;

      glPushMatrix();
      if not cam2d.OnlyXY Then
        begin
          glTranslatef( Camera.Center.X, Camera.Center.Y, 0 );
          if ( Camera.Zoom.X <> 1 ) or ( Camera.Zoom.Y <> 1 ) Then
            glScalef( Camera.Zoom.X, Camera.Zoom.Y, 1 );
          if Camera.Angle <> 0 Then
            glRotatef( Camera.Angle, 0, 0, 1 );
          glTranslatef( -Camera.Center.X, -Camera.Center.Y, 0 );
        end;
      if ( Camera.X <> 0 ) or ( Camera.Y <> 0 ) Then
        glTranslatef( -Camera.X, -Camera.Y, 0 );

      sprite2d_InScreen := sprite2d_InScreenCamera;
    end else
      begin
        cam2d.Global := @constCamera2D;
        cam2d.Apply  := FALSE;
        sprite2d_InScreen := sprite2d_InScreenSimple;
      end;
end;

function cam2d_Get : zglPCamera2D;
begin
  Result := cam2d.Global;
end;

initialization
  cam2d := @cam2dTarget[ TARGET_SCREEN ];
  with cam2dTarget[ TARGET_SCREEN ] do
    begin
      Global := @constCamera2D;
      OnlyXY := TRUE;
    end;
  with cam2dTarget[ TARGET_TEXTURE ] do
    begin
      Global := @constCamera2D;
      OnlyXY := TRUE;
    end;

end.

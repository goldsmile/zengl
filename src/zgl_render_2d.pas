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
unit zgl_render_2d;

{$I zgl_config.cfg}

interface
uses
  zgl_textures;

procedure batch2d_Begin;
procedure batch2d_End;
procedure batch2d_Flush;
function  batch2d_Check( Mode, FX : LongWord; Texture : zglPTexture ) : Boolean;

function sprite2d_InScreenSimple( X, Y, W, H, Angle : Single ) : Boolean;
function sprite2d_InScreenCamera( X, Y, W, H, Angle : Single ) : Boolean;

var
  render2dClip      : Boolean;
  render2dClipX     : Integer;
  render2dClipY     : Integer;
  render2dClipW     : Integer;
  render2dClipH     : Integer;
  render2dClipXW    : Integer;
  render2dClipYH    : Integer;
  render2dClipR     : Integer;
  b2dStarted        : Boolean;
  b2dNew            : Boolean;
  b2dBatches        : LongWord;
  b2dCurMode        : LongWord;
  b2dCurFX          : LongWord;
  b2dCurBlend       : LongWord;
  b2dCurBlendMode   : LongWord;
  b2dCurColorMode   : LongWord;
  b2dCurColorMask   : LongWord;
  b2dCurTex         : zglPTexture;
  b2dCurSmooth      : LongWord;
  sprite2d_InScreen : function( X, Y, W, H, Angle : Single ) : Boolean;

implementation
uses
  {$IFNDEF USE_GLES}
  zgl_opengl,
  zgl_opengl_all,
  {$ELSE}
  zgl_opengles,
  zgl_opengles_all,
  {$ENDIF}
  zgl_fx,
  zgl_camera_2d,
  zgl_primitives_2d;

procedure batch2d_Begin;
begin
  b2dNew     := TRUE;
  b2dStarted := TRUE;
  b2dBatches := 0;
end;

procedure batch2d_End;
begin
  batch2d_Flush();
  b2dStarted := FALSE;
end;

procedure batch2d_Flush;
begin
  if b2dStarted and ( not b2dNew ) Then
    begin
      INC( b2dBatches );
      b2dNew := TRUE;
      glEnd();

      glDisable( GL_TEXTURE_2D );
      if b2dCurBlend = 0 Then
        glDisable( GL_ALPHA_TEST )
      else
        glDisable( GL_BLEND );

      if b2dCurSmooth > 0 Then
        begin
          b2dCurSmooth := 0;
          glDisable( GL_LINE_SMOOTH    );
          {$IFNDEF USE_GLES}
          glDisable( GL_POLYGON_SMOOTH );
          {$ENDIF}
        end;
    end;
end;

function batch2d_Check( Mode, FX : LongWord; Texture : zglPTexture ) : Boolean;
begin
  if ( b2dCurMode <> Mode ) or ( b2dCurTex <> Texture ) or ( b2dCurBlend <> FX and FX_BLEND ) or ( b2dCurSmooth <> FX and PR2D_SMOOTH ) Then
    begin
      if not b2dNew Then
        batch2d_Flush();
      b2dNew := TRUE;
    end;

  b2dCurMode   := Mode;
  b2dCurTex    := Texture;
  b2dCurFX     := FX;
  b2dCurBlend  := FX and FX_BLEND;
  b2dCurSmooth := FX and PR2D_SMOOTH;

  Result := b2dNew;
  b2dNew := FALSE;
end;

function sprite2d_InScreenSimple( X, Y, W, H, Angle : Single ) : Boolean;
begin
  if Angle <> 0 Then
    Result := ( ( X + W + H / 2 > render2dClipX ) and ( X - W - H / 2 < render2dClipXW ) and
                ( Y + H + W / 2 > render2dClipY ) and ( Y - W - H / 2 < render2dClipYH ) )
  else
    Result := ( ( X + W > render2dClipX ) and ( X < render2dClipXW ) and
                ( Y + H > render2dClipY ) and ( Y < render2dClipYH ) );
end;

function sprite2d_InScreenCamera( X, Y, W, H, Angle : Single ) : Boolean;
  var
    sx, sy, srad : Single;
begin
  if not cam2d.OnlyXY Then
    begin
      sx   := X + W / 2;
      sy   := Y + H / 2;
      srad := ( W + H ) / 2;

      Result := sqr( sx - cam2d.CX ) + sqr( sy - cam2d.CY ) < sqr( srad + render2dClipR );
    end else
      if Angle <> 0 Then
        Result := ( ( X + W + H / 2 > render2dClipX + cam2d.Global.X ) and ( X - W - H / 2 < render2dClipXW + cam2d.Global.X ) and
                    ( Y + H + W / 2 > render2dClipY + cam2d.Global.Y ) and ( Y - W - H / 2 < render2dClipYH + cam2d.Global.Y ) )
      else
        Result := ( ( X + W > render2dClipX + cam2d.Global.X ) and ( X < render2dClipXW + cam2d.Global.X ) and
                    ( Y + H > render2dClipY + cam2d.Global.Y ) and ( Y < render2dClipYH + cam2d.Global.Y ) );
end;

initialization
  sprite2d_InScreen := sprite2d_InScreenSimple;

end.

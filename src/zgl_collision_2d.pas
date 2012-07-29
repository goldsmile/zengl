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
// Некоторые функции здесь записаны весьма страшно и в лоб :)
unit zgl_collision_2d;

{$I zgl_config.cfg}

interface

uses
  zgl_math_2d;

// point 2d
function col2d_PointInRect( X, Y : Single; const Rect : zglTRect ) : Boolean;
function col2d_PointInTriangle( X, Y : Single; const P1, P2, P3 : zglTPoint2D ) : Boolean;
function col2d_PointInCircle( X, Y : Single; const Circle : zglTCircle ) : Boolean;
// line 2d
function col2d_Line( const A, B : zglTLine; ColPoint : zglPPoint2D ) : Boolean;
function col2d_LineVsRect( const Line : zglTLine; const Rect : zglTRect; ColPoint : zglPPoint2D ) : Boolean;
function col2d_LineVsCircle( const Line : zglTLine; const Circle : zglTCircle ) : Boolean;
function col2d_LineVsCircleXY( const Line : zglTLine; const Circle : zglTCircle; Precision : Byte; ColPoint : zglPPoint2D ) : Boolean;
// rect
function col2d_Rect( const Rect1, Rect2 : zglTRect ) : Boolean;
function col2d_ClipRect( const Rect1, Rect2 : zglTRect ) : zglTRect;
function col2d_RectInRect( const Rect1, Rect2 : zglTRect ) : Boolean;
function col2d_RectInCircle( const Rect : zglTRect; const Circle : zglTCircle ) : Boolean;
function col2d_RectVsCircle( const Rect : zglTRect; const Circle : zglTCircle ) : Boolean;
// circle
function col2d_Circle( const Circle1, Circle2 : zglTCircle ) : Boolean;
function col2d_CircleInCircle( const Circle1, Circle2 : zglTCircle ) : Boolean;
function col2d_CircleInRect( const Circle : zglTCircle; const Rect : zglTRect ) : Boolean;

implementation

function Dist( Ax, Ay, Bx, By, Cx, Cy : Single ) : Single;
  var
    d, dx, dy : Single;
begin
  dx := Ax - Bx;
  dy := Ay - By;
  d  := dx * ( Cy - Ay ) - dy * ( Cx - Ax );

  if d > 0 Then
    Result := sqr( sqr( d ) / ( sqr( dx ) + sqr( dy ) ) )
  else
    Result := -sqr( sqr( d ) / ( sqr( dx ) + sqr( dy ) ) );
end;

function col2d_PointInRect( X, Y : Single; const Rect : zglTRect ) : Boolean;
begin
  Result := ( X >= Rect.X ) and ( X < Rect.X + Rect.W ) and ( Y >= Rect.Y ) and ( Y < Rect.Y + Rect.H );
end;

function col2d_PointInTriangle( X, Y : Single; const P1, P2, P3 : zglTPoint2D ) : Boolean;
  var
    o1 : Integer;
    o2 : Integer;
    o3 : Integer;
begin
  o1 := m_Orientation( X, Y, P1.x, P1.y, P2.x, P2.y );
  o2 := m_Orientation( X, Y, P2.x, P2.y, P3.x, P3.y );

  Result := FALSE;
  if ( o1 * o2 ) <> -1 Then
    begin
      o3 := m_Orientation( X, Y, P3.x, P3.y, P1.x, P1.y );
      if ( o1 = o3 ) or ( o3 = 0 ) Then
        Result := TRUE
      else
        if o1 = 0 Then
          Result := ( o2 * o3 ) >= 0
        else
          if o2 = 0 Then
            Result := ( o1 * o3 ) >= 0;
    end;
end;

function col2d_PointInCircle( X, Y : Single; const Circle : zglTCircle ) : Boolean;
begin
  Result := sqr( Circle.cX - X ) + sqr( Circle.cY - Y ) < sqr( Circle.Radius );
end;

function col2d_Line( const A, B : zglTLine; ColPoint : zglPPoint2D ) : Boolean;
  var
    s, t, tmp : Single;
    s1, s2    : array[ 0..1 ] of Single;
begin
  Result := FALSE;

  s1[ 0 ] := A.x1 - A.x0;
  s1[ 1 ] := A.y1 - A.y0;
  s2[ 0 ] := B.x1 - B.x0;
  s2[ 1 ] := B.y1 - B.y0;

  s := ( s2[ 0 ] * ( -s1[ 1 ] ) - ( -s1[ 0 ] ) * s2[ 1 ] );
  if s <> 0 Then
    tmp := 1 / ( s2[ 0 ] * ( -s1[ 1 ] ) - ( -s1[ 0 ] ) * s2[ 1 ] )
  else
    exit;

  s := ( ( A.x0 - B.x0 ) * ( -s1[ 1 ] ) - ( -s1[ 0 ] ) * ( A.y0 - B.y0 ) ) * tmp;
  t := ( s2[ 0 ] * ( A.y0 - B.y0 ) - ( A.x0 - B.x0 ) * s2[ 1 ] ) * tmp;

  Result := ( s >= 0 ) and ( s <= 1 ) and ( t >= 0 ) and ( t <= 1 );

  if Assigned( ColPoint ) Then
    begin
      ColPoint.X := A.x0 + t * s1[ 0 ];
      ColPoint.Y := A.y0 + t * s1[ 1 ];
    end;
end;

function col2d_LineVsRect( const Line : zglTLine; const Rect : zglTRect; ColPoint : zglPPoint2D ) : Boolean;
  var
    line0 : zglTLine;
begin
  if not Assigned( ColPoint ) Then
    begin
      Result := col2d_PointInRect( Line.x0, Line.y0, Rect ) or col2d_PointInRect( Line.x1, Line.y1, Rect );
      if not Result Then
        begin
          line0.x0 := Rect.X;
          line0.y0 := Rect.Y;
          line0.x1 := Rect.X + Rect.W;
          line0.y1 := Rect.Y + Rect.H;
          Result   := col2d_Line( Line, line0, ColPoint );
          if not Result Then
            begin
              line0.x0 := Rect.X;
              line0.y0 := Rect.Y + Rect.H;
              line0.x1 := Rect.X + Rect.W;
              line0.y1 := Rect.Y;
              Result   := col2d_Line( Line, line0, ColPoint );
            end;
        end;
    end else
      begin
        line0.x0 := Rect.X;
        line0.y0 := Rect.Y;
        line0.x1 := Rect.X + Rect.W;
        line0.y1 := Rect.Y;
        Result   := col2d_Line( Line, line0, ColPoint );
        if Result Then exit;

        line0.x0 := Rect.X + Rect.W;
        line0.y0 := Rect.Y;
        line0.x1 := Rect.X + Rect.W;
        line0.y1 := Rect.Y + Rect.H;
        Result   := col2d_Line( Line, line0, ColPoint );
        if Result Then exit;

        line0.x0 := Rect.X + Rect.W;
        line0.y0 := Rect.Y + Rect.H;
        line0.x1 := Rect.X;
        line0.y1 := Rect.Y + Rect.H;
        Result   := col2d_Line( Line, line0, ColPoint );
        if Result Then exit;

        line0.x0 := Rect.X;
        line0.y0 := Rect.Y;
        line0.x1 := Rect.X;
        line0.y1 := Rect.Y + Rect.H;
        Result   := col2d_Line( Line, line0, ColPoint );
      end;
end;

function col2d_LineVsCircle( const Line : zglTLine; const Circle : zglTCircle ) : Boolean;
  var
    p1, p2  : array[ 0..1 ] of Single;
    dx, dy  : Single;
    a, b, c : Single;
begin
  p1[ 0 ] := Line.x0 - Circle.cX;
  p1[ 1 ] := Line.y0 - Circle.cY;
  p2[ 0 ] := Line.x1 - Circle.cX;
  p2[ 1 ] := Line.y1 - Circle.cY;

  dx := p2[ 0 ] - p1[ 0 ];
  dy := p2[ 1 ] - p1[ 1 ];

  a := sqr( dx ) + sqr( dy );
  b := ( p1[ 0 ] * dx + p1[ 1 ] * dy ) * 2;
  c := sqr( p1[ 0 ] ) + sqr( p1[ 1 ] ) - sqr( Circle.Radius );

  if -b < 0 Then
    Result := c < 0
  else
    if -b < a * 2 Then
      Result := a * c * 4 - sqr( b )  < 0
    else
      Result := a + b + c < 0;
end;

function col2d_LineVsCircleXY( const Line : zglTLine; const Circle : zglTCircle; Precision : Byte; ColPoint : zglPPoint2D ) : Boolean;
  var
    p1      : array of zglTPoint2D;
    line0   : zglTLine;
    i, t, k : Integer;
begin
  if not col2d_LineVsCircle( Line, Circle ) Then
    begin
      Result := FALSE;
      exit;
    end;
  Result := TRUE;

  t := 0;
  k := Round( 360 / Precision );
  SetLength( p1, Precision + 1 );
  for i := 0 to Precision - 1 do
    begin
      p1[ i ].X := Circle.cX + m_Cos( k * i ) * Circle.Radius;
      p1[ i ].Y := Circle.cY + m_Sin( k * i ) * Circle.Radius;
    end;
  p1[ Precision ].X := p1[ 0 ].X;
  p1[ Precision ].Y := p1[ 0 ].Y;


  for i := 0 to Precision - 1 do
    begin
      line0.x0 := p1[ i     ].X;
      line0.y0 := p1[ i     ].Y;
      line0.x1 := p1[ i + 1 ].X;
      line0.y1 := p1[ i + 1 ].Y;
      if col2d_Line( Line, line0, ColPoint ) Then
        begin
          INC( t );
          if t = 2 Then exit;
        end;
    end;
end;

function col2d_Rect( const Rect1, Rect2 : zglTRect ) : Boolean;
begin
  Result := ( Rect1.X + Rect1.W >= Rect2.X ) and ( Rect1.X <= Rect2.X + Rect2.W ) and ( Rect1.Y + Rect1.H >= Rect2.Y ) and ( Rect1.Y <= Rect2.Y + Rect2.H );
end;

function col2d_ClipRect( const Rect1, Rect2 : zglTRect ) : zglTRect;
begin
  if ( Rect1.X < Rect2.X ) or ( Rect1.X > Rect2.X + Rect2.W ) Then
    Result.X := Rect2.X
  else
    Result.X := Rect1.X;
  if ( Rect1.Y < Rect2.Y ) or ( Rect1.Y > Rect2.Y + Rect2.H ) Then
    Result.Y := Rect2.Y
  else
    Result.Y := Rect1.Y;

  Result.W := ( Rect1.X + Rect1.W ) - Result.X;
  Result.H := ( Rect1.Y + Rect1.H ) - Result.Y;

  if Result.X + Result.W > Rect2.X + Rect2.W Then
    Result.W := ( Rect2.X + Rect2.W ) - Result.X - 1;
  if Result.Y + Result.H > Rect2.Y + Rect2.H Then
    Result.H := ( Rect2.Y + Rect2.H ) - Result.Y - 1;
end;

function col2d_RectInRect( const Rect1, Rect2 : zglTRect ) : Boolean;
begin
  Result := ( Rect1.X > Rect2.X ) and ( Rect1.X + Rect1.W < Rect2.X + Rect2.W ) and ( Rect1.Y > Rect2.Y ) and ( Rect1.Y + Rect1.H < Rect2.Y + Rect2.H );
end;

function col2d_RectInCircle( const Rect : zglTRect; const Circle : zglTCircle ) : Boolean;
begin
  Result := col2d_PointInCircle( Rect.X, Rect.Y, Circle ) and col2d_PointInCircle( Rect.X + Rect.W, Rect.Y, Circle ) and
            col2d_PointInCircle( Rect.X + Rect.W, Rect.Y + Rect.H, Circle ) and col2d_PointInCircle( Rect.X, Rect.Y + Rect.H, Circle );
end;

function col2d_RectVsCircle( const Rect : zglTRect; const Circle : zglTCircle ) : Boolean;
begin
  Result := ( col2d_PointInCircle( Rect.X, Rect.Y, Circle ) or col2d_PointInCircle( Rect.X + Rect.W, Rect.Y, Circle ) or
              col2d_PointInCircle( Rect.X + Rect.W, Rect.Y + Rect.H, Circle ) or col2d_PointInCircle( Rect.X, Rect.Y + Rect.H, Circle ) ) or
            ( col2d_PointInRect( Circle.cX - Circle.Radius, Circle.cY, Rect ) or col2d_PointInRect( Circle.cX + Circle.Radius, Circle.cY, Rect ) or
              col2d_PointInRect( Circle.cX, Circle.cY - Circle.Radius, Rect ) or col2d_PointInRect( Circle.cX, Circle.cY + Circle.Radius, Rect ) );
end;

function col2d_Circle( const Circle1, Circle2 : zglTCircle ) : Boolean;
begin
  Result := sqr( Circle1.cX - Circle2.cX ) + sqr( Circle1.cY - Circle2.cY ) <= sqr( Circle1.Radius + Circle2.Radius );
end;

function col2d_CircleInCircle( const Circle1, Circle2 : zglTCircle ) : Boolean;
begin
  Result := sqr( Circle1.cX - Circle2.cX ) + sqr( Circle1.cY - Circle2.cY ) < sqr( Circle1.Radius - Circle2.Radius );
end;

function col2d_CircleInRect( const Circle : zglTCircle; const Rect : zglTRect ) : Boolean;
begin
  Result := col2d_PointInRect( Circle.cX + Circle.Radius, Circle.cY + Circle.Radius, Rect ) and
            col2d_PointInRect( Circle.cX - Circle.Radius, Circle.cY + Circle.Radius, Rect ) and
            col2d_PointInRect( Circle.cX - Circle.Radius, Circle.cY - Circle.Radius, Rect ) and
            col2d_PointInRect( Circle.cX + Circle.Radius, Circle.cY - Circle.Radius, Rect );
end;

end.

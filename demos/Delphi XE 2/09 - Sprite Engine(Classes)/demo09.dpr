program demo09;

{$I zglCustomConfig.cfg}

{$R *.res}

uses
  zglSpriteEngine,
  {$IFDEF USE_ZENGL_STATIC}
  zgl_main,
  zgl_screen,
  zgl_window,
  zgl_timers,
  zgl_keyboard,
  zgl_render_2d,
  zgl_fx,
  zgl_textures,
  zgl_textures_png,
  zgl_textures_jpg,
  zgl_sprite_2d,
  zgl_primitives_2d,
  zgl_font,
  zgl_text,
  zgl_math_2d,
  zgl_utils
  {$ELSE}
  zglHeader
  {$ENDIF}
  ;

type
  CMiku = class(zglCSprite2D)
  protected
    FSpeed : zglTPoint2D;
  public
    procedure OnInit( _Texture : zglPTexture; _Layer : Integer ); override;
    procedure OnDraw; override;
    procedure OnProc; override;
    procedure OnFree; override;
  end;

var
  dirRes    : UTF8String {$IFNDEF MACOSX} = '../data/' {$ENDIF};
  fntMain   : zglPFont;
  texLogo   : zglPTexture;
  texMiku   : zglPTexture;
  time      : Integer;
  sengine2d : zglCSEngine2D;

// Miku
procedure CMiku.OnInit( _Texture : zglPTexture; _Layer : Integer );
begin
  // RU: ������ ���� �������� � ���� ��� �������, ������ ����������� ����������� ��������� ����� ������ � ������ �� ������ ������ � ����� � ��������.
  // EN: Set own texture and layer for sprite, also standard parameters will be set automatically(like width and height from info about texture).
  inherited OnInit( texMiku, random( 10 ) );

  X := 800 + random( 800 );
  Y := random( 600 - 128 );
  // RU: ������ �������� ��������.
  // EN: Set the moving speed.
  FSpeed.X := -random( 10 ) / 5 - 0.5;
  FSpeed.Y := ( random( 10 ) - 5 ) / 5;
end;

procedure CMiku.OnDraw;
begin
  // RU: �.�. �� ���� ��� ��������� ��������� ������ ��� �������, �� ������� �������� ����� OnDraw ������ zglCSprite2D.
  // EN: Because in fact, this procedure is declared only for example, so call to the main method OnDraw of class zglCSprite2D.
  inherited;
end;

procedure CMiku.OnProc;
begin
  inherited;
  X := X + FSpeed.X;
  Y := Y + FSpeed.Y;
  Frame := Frame + ( abs( FSpeed.X ) + abs( FSpeed.Y ) ) / 25;
  if Frame > 8 Then
    Frame := 1;

  // RU: ���� ������ ������� �� ������� �� X, ����� �� ������� ���.
  // EN: Delete the sprite if it goes beyond X.
  if X < -128 Then sengine2d.DelSprite( ID );

  // RU: ���� ������ ������� �� ������� �� Y, ������ ��� � ������� �� ��������
  // EN: Add sprite to queue for delete if it goes beyond Y.
  if Y < -128 Then Kill := TRUE;
  if Y > 600  Then Kill := TRUE;
end;

procedure CMiku.OnFree;
begin
  inherited;
end;

// RU: �������� 100 ��������.
// EN: Add 100 sprites.
procedure AddMiku;
  var
    i, ID : Integer;
begin
  for i := 1 to 100 do
    begin
      // RU: ����������� � ����������� ��������� ����� "�����" ��� ������ :)
      // EN: Request a "place" for a new sprite :)
      ID := sengine2d.AddSprite();
      // RU: ������� ��������� ������� CMiku. ����������� ������������ �������� ��� �������� � ������� ID ��� �������.
      // EN: Create a new CMiku. Constructor arguments must be current sprite engine and future ID for sprite.
      sengine2d.List[ ID ] := CMiku.Create( sengine2d, ID );
    end;
end;

// RU: ������� 100 ��������.
// EN: Delete 100 sprites.
procedure DelMiku;
  var
    i : Integer;
begin
  // RU: ������ 100 �������� �� ��������� ID.
  // EN: Delete 100 sprites with random ID.
  for i := 1 to 100 do
    sengine2d.DelSprite( random( sengine2d.Count ) );
end;

procedure Init;
  var
    i : Integer;
begin
  texLogo := tex_LoadFromFile( dirRes + 'zengl.png' );

  texMiku := tex_LoadFromFile( dirRes + 'miku.png' );
  tex_SetFrameSize( texMiku, 128, 128 );

  // RU: ������� ��������� zglCSEngine2D.
  // EN: Create zglCSEngine2D object.
  sengine2d := zglCSEngine2D.Create();

  // RU: �������� 1000 �������� Miku-chan :)
  // EN: Create 1000 sprites of Miku-chan :)
  for i := 0 to 9 do
    AddMiku();

  fntMain := font_LoadFromFile( dirRes + 'font.zfi' );
end;

procedure Draw;
begin
  batch2d_Begin();
  // RU: ������ ��� ������� ����������� � ������� ���������� ���������.
  // EN: Render all sprites contained in current sprite engine.
  if time > 255 Then
    sengine2d.Draw();

  if time <= 255 Then
    ssprite2d_Draw( texLogo, 400 - 256, 300 - 128, 512, 256, 0, time )
  else
    if time < 510 Then
      begin
        pr2d_Rect( 0, 0, 800, 600, $000000, 510 - time, PR2D_FILL );
        ssprite2d_Draw( texLogo, 400 - 256, 300 - 128, 512, 256, 0, 510 - time );
      end;

  if time > 255 Then
    begin
      pr2d_Rect( 0, 0, 256, 64, $000000, 200, PR2D_FILL );
      text_Draw( fntMain, 0, 0, 'FPS: ' + u_IntToStr( zgl_Get( RENDER_FPS ) ) );
      text_Draw( fntMain, 0, 20, 'Sprites: ' + u_IntToStr( sengine2d.Count ) );
      text_Draw( fntMain, 0, 40, 'Up/Down - Add/Delete Miku :)' );
    end;
  batch2d_End();
end;

procedure Timer;
begin
  INC( time, 2 );

  // RU: ��������� ��������� ���� �������� � ������� ���������� ���������.
  // EN: Process all sprites contained in current sprite engine.
  sengine2d.Proc();

  // RU: �� ������� ������� �������� ��� �������.
  // EN: Delete all sprites if space was pressed.
  if key_Press( K_SPACE ) Then sengine2d.ClearAll();
  if key_Press( K_UP ) Then AddMiku();
  if key_Press( K_DOWN ) Then DelMiku();

  if key_Press( K_ESCAPE ) Then zgl_Exit();

  key_ClearState();
end;

procedure Quit;
begin
  // RU: ������� ������ �� ��������� ��������.
  // EN: Free allocated memory for sprites.
  sengine2d.Destroy();
end;

Begin
  {$IFNDEF USE_ZENGL_STATIC}
  if not zglLoad( libZenGL ) Then exit;
  {$ENDIF}

  randomize();

  timer_Add( @Timer, 16 );
  timer_Add( @AddMiku, 1000 );

  zgl_Reg( SYS_LOAD, @Init );
  zgl_Reg( SYS_DRAW, @Draw );
  zgl_Reg( SYS_EXIT, @Quit );

  wnd_SetCaption( '09 - Sprite Engine(Classes)' );

  wnd_ShowCursor( TRUE );

  scr_SetOptions( 800, 600, REFRESH_MAXIMUM, FALSE, FALSE );

  zgl_Init();
End.

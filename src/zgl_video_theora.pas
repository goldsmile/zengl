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
unit zgl_video_theora;

{$I zgl_config.cfg}

{$IFDEF FPC}
  {$DEFINE YUV2RGBA_Robin_Watts}
{$ENDIF}

{$IFDEF YUV2RGBA_Robin_Watts}
  {$L yuv2bgr16tab.o}
  {$L yuv420rgb8888.o}
{$ENDIF}

interface

const
  THEORA_EXTENSIONS : array[ 0..1 ] of UTF8String = ( 'OGV', 'OGG' );

implementation
uses
  zgl_types,
  zgl_main,
  zgl_file,
  zgl_memory,
  zgl_lib_ogg,
  zgl_lib_theora,
  zgl_textures,
  zgl_video,
  zgl_log;

type
  zglTTheoraData = record
    File_       : zglTFile;
    Memory      : zglTMemory;
    SyncState   : ogg_sync_state;
    StreamState : ogg_stream_state;
    TheoraInfo  : th_info;
    DecoderCtx  : pth_dec_ctx;
    Time        : Double;
  end;

{$IFDEF YUV2RGBA_Robin_Watts}
procedure yuv420_2_rgb8888( dst_ptr, y_ptr, u_ptr, v_ptr : pcuint8; width, height, y_span, uv_span, dst_span : cint32; tables : pcuint32; dither : cint32 ); cdecl; external;
function get_yuv2bgr565_table : pcuint32; cdecl; external;
{$ENDIF}

var
  theoraDecoderOGV : zglTVideoDecoder;
  theoraDecoderOGG : zglTVideoDecoder;

function buffer_ReadData( var TheoraData : zglTTheoraData; Size : LongWord = 4096 ) : LongWord;
  var
    buffer : pcchar;
begin
  buffer := ogg_sync_buffer( @TheoraData.SyncState, Size );
  if Assigned( TheoraData.Memory.Memory ) Then
    Result := mem_Read( TheoraData.Memory, buffer^, Size )
  else
    Result := file_Read( TheoraData.File_, buffer^, Size );
  ogg_sync_wrote( @TheoraData.SyncState, Result );
end;

function buffer_Seek( var TheoraData : zglTTheoraData; Offset, Mode : Integer ) : LongWord;
begin
  if Assigned( TheoraData.Memory.Memory ) Then
    Result := mem_Seek( TheoraData.Memory, Offset, Mode )
  else
    Result := file_Seek( TheoraData.File_, Offset, Mode );
end;

function theora_Open( var TheoraData : zglTTheoraData; var Width, Height : Word; var FrameRate : Single; var Duration : Double; var Frames : Integer ) : Boolean;
  var
    state      : Boolean;
    gotPacket  : cint;
    test       : ogg_stream_state;
    i, ret     : cint;
    page       : ogg_page;
    packet     : ogg_packet;
    comment    : th_comment;
    setupInfo  : pth_setup_info;
    isTheora   : Boolean;
    headerin   : cint;
    granulePos : ogg_int64_t;
begin
  TheoraData.Time := 0;

  ogg_sync_init( @TheoraData.SyncState );
  th_comment_init( @comment );
  th_info_init( @TheoraData.TheoraInfo );
  setupInfo := nil;
  isTheora  := FALSE;
  headerin  := 0;

  state := FALSE;
  while not state do
    begin
      if buffer_ReadData( TheoraData ) = 0 Then break;

      while ogg_sync_pageout( @TheoraData.SyncState, @page ) > 0 do
        begin
          if ogg_page_bos( @page ) = 0 Then
            begin
              if isTheora Then
                ogg_stream_pagein( @TheoraData.StreamState, @page );
              state := TRUE;
              break;
            end;

          ogg_stream_init( @test, ogg_page_serialno( @page ) );
          ogg_stream_pagein( @test, @page );
          gotPacket := ogg_stream_packetpeek( @test, @packet );

          headerin := th_decode_headerin( @TheoraData.TheoraInfo, @comment, @setupInfo, @packet );
          if ( gotPacket = 1 ) and ( not isTheora ) and ( headerin >= 0 ) Then
            begin
              TheoraData.StreamState := test;
              isTheora               := TRUE;
              if headerin <> 0 Then
                ogg_stream_packetout( @TheoraData.StreamState, nil );
            end else
              ogg_stream_clear( @test );
        end;
    end;

  while isTheora and ( headerin <> 0 ) do
    begin
      if headerin <> 0 Then
        ret := ogg_stream_packetpeek( @TheoraData.StreamState, @packet );
      while ( headerin <> 0 ) and ( ret <> 0 ) do
        begin
          if ret < 0 Then continue;

          headerin := th_decode_headerin( @TheoraData.TheoraInfo, @comment, @setupInfo, @packet );
          if headerin < 0 Then
            begin
              log_Add( 'Theora: Error parsing Theora stream headers' );
              Result := FALSE;
              exit;
            end else
              if headerin > 0 Then
                ogg_stream_packetout( @TheoraData.StreamState, nil );

          ret := ogg_stream_packetpeek( @TheoraData.StreamState, @packet );
        end;

      if not ( isTheora and ( headerin <> 0 ) ) Then break;

      if ogg_sync_pageout( @TheoraData.SyncState, @page ) <= 0 Then
        begin
          if buffer_ReadData( TheoraData ) = 0 Then
            begin
              log_Add( 'Theora: End of file while searching for codec headers' );
              Result := FALSE;
              exit;
            end;
        end else
          ogg_stream_pagein( @TheoraData.StreamState, @page );
    end;

  if isTheora Then
    begin
      TheoraData.DecoderCtx := th_decode_alloc( @TheoraData.TheoraInfo, setupInfo );

      Width     := TheoraData.TheoraInfo.frame_width;
      Height    := TheoraData.TheoraInfo.frame_height;
      FrameRate := TheoraData.TheoraInfo.fps_numerator / TheoraData.TheoraInfo.fps_denominator;
      Duration  := 0;
      Frames    := 0;
      Result    := TRUE;

      for i := 1 to 64 do
        begin
          ogg_sync_reset( @TheoraData.SyncState );
          buffer_Seek( TheoraData, -4096 * i, FSM_END );

          buffer_ReadData( TheoraData, 4096 * i );
          ogg_sync_pageseek( @TheoraData.SyncState, @page );

          while TRUE do
            begin
              ret := ogg_sync_pageout( @TheoraData.SyncState, @page );
              if ret = 0 Then break;
              if ogg_page_serialno( @page ) <> TheoraData.StreamState.serialno Then continue;

              granulePos := ogg_page_granulepos( @page );
              if granulePos >= 0 Then
                Frames := th_granule_frame( TheoraData.DecoderCtx, granulePos )
              else
                if Frames > 0 Then
                  INC( Frames );
            end;

          if Frames > 0 Then break;
        end;

      if Frames >= 0 Then
        Duration := ( Frames + 1 ) / FrameRate * 1000;

      ogg_sync_reset( @TheoraData.SyncState );
      ogg_stream_reset( @TheoraData.StreamState );
      buffer_Seek( TheoraData, 0, FSM_SET );

      th_setup_free( setupInfo );
    end else
      Result := FALSE;

  if TheoraData.TheoraInfo.pixel_fmt <> TH_PF_420 Then
    begin
      log_Add( 'Theora: Pixel format is not supported(YUV 4:2:0 is needed)' );
      Result := FALSE;
    end;

  th_comment_clear( @comment );
end;

function clamp( value : Integer ) : Integer; {$IFDEF USE_INLINE} inline; {$ENDIF}
begin
  if value > 2088960 Then
    Result := 2088960
  else
    if value < 0 Then
      Result := 0
    else
      Result := value;
end;

function theora_Update( var TheoraData : zglTTheoraData; Time : Double; Data : PByteArray ) : Integer;
  var
    ycbcr      : th_ycbcr_buffer;
    {$IFDEF YUV2RGBA_Robin_Watts}
    dataOrig   : PByteArray;
    {$ELSE}
    Y, Cb, Cr  : Integer;
    {$ENDIF}
    i, j       : Integer;
    videoReady : Boolean;
    granulePos : ogg_int64_t;
    page       : ogg_page;
    packet     : ogg_packet;
begin
  Result := 0;
  if TheoraData.Time > Time Then exit;

  videoReady := FALSE;
  granulePos := -1;

  while not videoReady do
    if ogg_stream_packetout( @TheoraData.StreamState, @packet ) > 0 Then
      begin
        if th_decode_packetin( TheoraData.DecoderCtx, @packet, @granulePos ) >= 0 Then
          begin
            TheoraData.Time := th_granule_time( TheoraData.DecoderCtx, granulePos ) * 1000;
            videoReady      := Time < TheoraData.Time;
            INC( Result );
          end;
      end else
        break;

  if not videoReady Then
    begin
      if buffer_ReadData( TheoraData ) = 0 Then exit;

      while ogg_sync_pageout( @TheoraData.SyncState, @page ) > 0 do
        ogg_stream_pagein( @TheoraData.StreamState, @page );

      INC( Result, theora_Update( TheoraData, Time, Data ) );
      exit;
    end else
      begin
        th_decode_ycbcr_out( TheoraData.DecoderCtx, @ycbcr );

      {$IFDEF YUV2RGBA_Robin_Watts}
        dataOrig := Data;
        INC( PByte( Data ), ( ycbcr[ 0 ].height - 1 ) * ycbcr[ 0 ].width * 4 );

        yuv420_2_rgb8888( pcuint8( Data ), ycbcr[ 0 ].data, ycbcr[ 1 ].data, ycbcr[ 2 ].data,
                          ycbcr[ 0 ].width, ycbcr[ 0 ].height,
                          ycbcr[ 0 ].stride, ycbcr[ 1 ].stride, -ycbcr[ 0 ].width * 4,
                          get_yuv2bgr565_table(), 0 );

        {$IFDEF ANDROID}
        INC( PByte( dataOrig ), 3 );
        INC( PByte( Data ), 3 );
        while ( Data <> dataOrig ) do
          begin
            PByte( Data )^ := 255;
            DEC( PByte( Data ), 4 );
          end;
        DEC( PByte( Data ), 3 );
        {$ELSE}
        Data := dataOrig;
        {$ENDIF}
      {$ELSE}
        INC( PByte( Data ), ( ycbcr[ 0 ].height - 1 ) * ycbcr[ 0 ].width * 4 );
        for j := 0 to ycbcr[ 0 ].height - 1 do
          begin
            for i := 0 to ycbcr[ 0 ].width - 1 do
              begin
                Y  := 9535 * ( PByteArray( ycbcr[ 0 ].data )[ i ] - 16 );
                Cb := PByteArray( ycbcr[ 1 ].data )[ i shr 1 ] - 128;
                Cr := PByteArray( ycbcr[ 2 ].data )[ i shr 1 ] - 128;

                Data[ 0 ] := clamp( Y + 13074 * Cr ) shr 13;
                Data[ 1 ] := clamp( Y - 6660 * Cr - 3203 * Cb ) shr 13;
                Data[ 2 ] := clamp( Y + 16531 * Cb ) shr 13;
                INC( PByte( Data ), 4 );
              end;

            DEC( PByte( Data ), ycbcr[ 0 ].width * 8 );
            INC( ycbcr[ 0 ].data, ycbcr[ 0 ].stride );

            if j and 1 > 0 Then
              begin
                INC( ycbcr[ 1 ].data, ycbcr[ 1 ].stride );
                INC( ycbcr[ 2 ].data, ycbcr[ 2 ].stride );
              end;
          end;
      {$ENDIF}
      end;
end;

function theora_SeekToFrame( var TheoraData : zglTTheoraData; Frame : Integer; Keyframe : Boolean ) : Integer;
  var
    i, l, r    : Integer;
    ret        : Integer;
    _frame     : Integer;
    granulePos : ogg_int64_t;
    page       : ogg_page;
begin
  l := 0;
  if Assigned( TheoraData.Memory.Memory ) Then
    r := TheoraData.Memory.Size
  else
    r := file_GetSize( TheoraData.File_ );

  if Frame = 0 Then
    begin
      buffer_Seek( TheoraData, 0, FSM_SET );
      i := 100;
    end else
      i := 0;

  granulePos := 0;
  while i < 100 do
    begin
		  ogg_sync_reset( @TheoraData.SyncState );
      buffer_Seek( TheoraData, ( l + r ) div 2, FSM_SET );
      FillChar( page, SizeOf( page ), 0 );
      ogg_sync_pageseek( @TheoraData.SyncState, @page );

      while i < 1000 do
        begin
          ret := ogg_sync_pageout( @TheoraData.SyncState, @page );
          if ret = 1 Then
            begin
              if ogg_page_serialno( @page ) = TheoraData.StreamState.serialno Then
                begin
                  granulePos := ogg_page_granulepos( @page );
                  if granulePos >= 0 Then
                    begin
                      _frame := th_granule_frame( TheoraData.DecoderCtx, granulePos );
                      if ( _frame < Frame ) and ( Frame - _frame < 10 ) Then
                        begin
                          i := 1000;
                          break;
                        end;
                      if Frame - 1 > _frame Then
                        l := ( l + r ) div 2
                      else
                        r := ( l + r ) div 2;
                      break;
                    end;
                end;
            end else
              buffer_ReadData( TheoraData );
        end;
      INC( i );
    end;

  if Keyframe Then
    begin
      Result := granulePos shr TheoraData.TheoraInfo.keyframe_granule_shift;
      exit;
    end;

  ogg_sync_reset( @TheoraData.SyncState );
  FillChar( page, SizeOf( ogg_page ), 0 );
  ogg_sync_pageseek( @TheoraData.SyncState, @page );
  if Frame = 0 Then
    begin
      Result := -1;
      exit;
    end;
  buffer_Seek( TheoraData, ( l + r ) div 2, FSM_SET );
  Result := -1;
end;

procedure theora_Seek( var TheoraData : zglTTheoraData; Frame : Integer );
  var
    keyframe   : Integer;
    packet     : ogg_packet;
    page       : ogg_page;
    granulePos : ogg_int64_t;
    granuleSet : Boolean;
    ret        : cint;
begin
  ogg_stream_reset( @TheoraData.StreamState );

  keyframe := theora_SeekToFrame( TheoraData, Frame, TRUE );
  if keyframe - 1 > 0 Then
    theora_SeekToFrame( TheoraData, keyframe - 1, FALSE )
  else
    theora_SeekToFrame( TheoraData, 0, FALSE );

  granuleSet := FALSE;
  if keyframe <= 1 Then
    begin
      if ( TheoraData.TheoraInfo.version_major= 3 ) and ( TheoraData.TheoraInfo.version_minor = 2 ) and ( TheoraData.TheoraInfo.version_subminor = 0 ) Then
        granulePos := 0
      else
        granulePos := 1;
      th_decode_ctl( TheoraData.DecoderCtx, TH_DECCTL_SET_GRANPOS, @granulePos, SizeOf( granulePos ) );
      granuleSet := TRUE;
    end;

  while TRUE do
    begin
      ret := ogg_stream_packetout( @TheoraData.StreamState, @packet );
      if ret > 0 Then
        begin
          if not granuleSet Then
            begin
              if packet.granulepos >= 0 Then
                begin
                  th_decode_ctl( TheoraData.DecoderCtx, TH_DECCTL_SET_GRANPOS, @packet.granulepos, SizeOf( packet.granulepos ) );
                  granuleSet := TRUE;
                end else
                  continue;
            end;
          if th_decode_packetin( TheoraData.DecoderCtx, @packet, @granulePos ) <> 0 Then continue;
          keyframe := th_granule_frame( TheoraData.DecoderCtx, granulePos );
          if keyframe >= Frame - 1 Then break;
        end else
          begin
            if buffer_ReadData( TheoraData ) = 0 Then exit;
            while ogg_sync_pageout( @TheoraData.SyncState, @page ) > 0 do
              if ogg_page_serialno( @page ) = TheoraData.StreamState.serialno Then
                ogg_stream_pagein( @TheoraData.StreamState, @page );
          end;
    end;
end;

function theora_DecoderOpen( var Stream : zglTVideoStream; const FileName : UTF8String ) : Boolean;
begin
  if not Assigned( Stream._private.Data ) Then
    zgl_GetMem( Stream._private.Data, SizeOf( zglTTheoraData ) );

  file_Open( zglTTheoraData( Stream._private.Data^ ).File_, FileName, FOM_OPENR );
  Result := theora_Open( zglTTheoraData( Stream._private.Data^ ), Stream.Info.Width, Stream.Info.Height, Stream.Info.FrameRate, Stream.Info.Duration, Stream.Info.Frames );
end;

function theora_DecoderOpenMem( var Stream : zglTVideoStream; const Memory : zglTMemory ) : Boolean;
begin
  if not Assigned( Stream._private.Data ) Then
    zgl_GetMem( Stream._private.Data, SizeOf( zglTTheoraData ) );

  zglTTheoraData( Stream._private.Data^ ).Memory := Memory;
  Result := theora_Open( zglTTheoraData( Stream._private.Data^ ), Stream.Info.Width, Stream.Info.Height, Stream.Info.FrameRate, Stream.Info.Duration, Stream.Info.Frames );
end;

procedure theora_DecoderUpdate( var Stream : zglTVideoStream; Milliseconds : Double; Data : PByteArray );
begin
  INC( Stream.Frame, theora_Update( zglTTheoraData( Stream._private.Data^ ), Stream.Time, Data ) );
end;

procedure theora_DecoderSeek( var Stream : zglTVideoStream; Milliseconds : Double );
begin
  zglTTheoraData( Stream._private.Data^ ).Time := Milliseconds;
  theora_Seek( zglTTheoraData( Stream._private.Data^ ), Stream.Frame );
end;

procedure theora_DecoderLoop( var Stream : zglTVideoStream );
  var
    granulePos : ogg_int64_t;
begin
  with zglTTheoraData( Stream._private.Data^ ) do
    begin
      Time := 0;
      ogg_sync_reset( @SyncState );
      ogg_stream_reset( @StreamState );
      if ( TheoraInfo.version_major= 3 ) and ( TheoraInfo.version_minor = 2 ) and ( TheoraInfo.version_subminor = 0 ) Then
        granulePos := 0
      else
        granulePos := 1;
      th_decode_ctl( DecoderCtx, TH_DECCTL_SET_GRANPOS, @granulePos, SizeOf( granulePos ) );
      if Assigned( Memory.Memory ) Then
        mem_Seek( Memory, 0, FSM_SET )
      else
        file_Seek( File_, 0, FSM_SET );
    end;
end;

procedure theora_DecoderClose( var Stream : zglTVideoStream );
begin
  with zglTTheoraData( Stream._private.Data^ ) do
    begin
      th_info_clear( @TheoraInfo );
      ogg_stream_clear( @StreamState );
      th_decode_free( DecoderCtx );
      ogg_sync_clear( @SyncState );

      if not Assigned( Memory.Memory ) Then
        file_Close( File_ );
    end;

  FreeMem( Stream._private.Data );
end;

initialization
{$IFDEF USE_THEORA}
  theoraDecoderOGV.Extension := THEORA_EXTENSIONS[ 0 ];
  theoraDecoderOGV.Open      := theora_DecoderOpen;
  theoraDecoderOGV.OpenMem   := theora_DecoderOpenMem;
  theoraDecoderOGV.Update    := theora_DecoderUpdate;
  theoraDecoderOGV.Seek      := theora_DecoderSeek;
  theoraDecoderOGV.Loop      := theora_DecoderLoop;
  theoraDecoderOGV.Close     := theora_DecoderClose;
  zgl_Reg( VIDEO_FORMAT_DECODER, @theoraDecoderOGV );
  theoraDecoderOGG           := theoraDecoderOGV;
  theoraDecoderOGG.Extension := THEORA_EXTENSIONS[ 1 ];
  zgl_Reg( VIDEO_FORMAT_DECODER, @theoraDecoderOGG );
{$ENDIF}

end.

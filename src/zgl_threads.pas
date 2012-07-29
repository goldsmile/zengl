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
unit zgl_threads;

{$I zgl_config.cfg}

interface
{$IFDEF UNIX}
  uses cthreads;
{$ENDIF}
{$IFNDEF FPC}
  uses Windows;
{$ENDIF}

type
  zglTThreadCallback = TThreadFunc;

  zglTThread = record
    ID     : {$IFDEF FPC} TThreadID {$ELSE} LongWord {$ENDIF};
    Handle : {$IFDEF FPC} TThreadID {$ELSE} THandle {$ENDIF};
  end;

  zglTCriticalSection = TRTLCriticalSection;
  zglTEvent           = Pointer;

procedure thread_Create( var Thread : zglTThread; Callback : zglTThreadCallback; Data : Pointer = nil );
procedure thread_Close( var Thread : zglTThread );
procedure thread_CSInit( var CS : TRTLCriticalSection );
procedure thread_CSDone( var CS : TRTLCriticalSection );
procedure thread_CSEnter( var CS : TRTLCriticalSection );
procedure thread_CSLeave( var CS : TRTLCriticalSection );
procedure thread_EventCreate( var Event : zglTEvent );
procedure thread_EventDestroy( var Event : zglTEvent );
procedure thread_EventSet( var Event : zglTEvent );
procedure thread_EventReset( var Event : zglTEvent );
procedure thread_EventWait( var Event : zglTEvent; Milliseconds : LongWord = $FFFFFFFF );

implementation

procedure thread_Create( var Thread : zglTThread; Callback : zglTThreadCallback; Data : Pointer = nil );
begin
  {$IFDEF FPC}
  Thread.Handle := BeginThread( Callback, Data, Thread.ID );
  {$ELSE}
  Thread.Handle := BeginThread( nil, 0, Callback, Data, 0, Thread.ID );
  {$ENDIF}
end;

procedure thread_Close( var Thread : zglTThread );
begin
  {$IFDEF FPC}
  CloseThread( Thread.Handle );
  {$ELSE}
  CloseHandle( Thread.Handle );
  {$ENDIF}
end;

procedure thread_CSInit( var CS : TRTLCriticalSection );
begin
  {$IFDEF FPC}
  InitCriticalSection( CS );
  {$ELSE}
  InitializeCriticalSection( CS );
  {$ENDIF}
end;

procedure thread_CSDone( var CS : TRTLCriticalSection );
begin
  {$IFDEF FPC}
  DoneCriticalSection( CS );
  {$ELSE}
  DeleteCriticalSection( CS );
  {$ENDIF}
end;

procedure thread_CSEnter( var CS : TRTLCriticalSection );
begin
  EnterCriticalSection( CS );
end;

procedure thread_CSLeave( var CS : TRTLCriticalSection );
begin
  LeaveCriticalSection( CS );
end;

procedure thread_EventCreate( var Event : zglTEvent );
begin
  {$IFDEF FPC}
  Event := Pointer( RTLEventCreate() );
  {$ELSE}
  Event := Pointer( CreateEvent( nil, TRUE, FALSE, nil ) );
  {$ENDIF}
end;

procedure thread_EventDestroy( var Event : zglTEvent );
begin
  {$IFDEF FPC}
  RTLEventDestroy( PRTLEvent( Event ) );
  {$ELSE}
  CloseHandle( LongWord( Event ) );
  {$ENDIF}
  Event := nil;
end;

procedure thread_EventSet( var Event : zglTEvent );
begin
  {$IFDEF FPC}
  RTLEventSetEvent( PRTLEvent( Event ) );
  {$ELSE}
  SetEvent( LongWord( Event ) );
  {$ENDIF}
end;

procedure thread_EventReset( var Event : zglTEvent );
begin
  {$IFDEF FPC}
  RTLEventResetEvent( PRTLEvent( Event ) );
  {$ELSE}
  ResetEvent( LongWord( Event ) );
  {$ENDIF}
end;

procedure thread_EventWait( var Event : zglTEvent; Milliseconds : LongWord = $FFFFFFFF );
begin
  {$IFDEF FPC}
  if Milliseconds = $FFFFFFFF Then
    RTLeventWaitFor( PRTLEvent( Event ) )
  else
    RTLeventWaitFor( PRTLEvent( Event ), Milliseconds );
  {$ELSE}
  WaitForSingleObject( LongWord( Event ), Milliseconds );
  {$ENDIF}
end;

end.

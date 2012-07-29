program demo01VCL;

uses
  Forms,
  demo01 in 'demo01.pas' {Form1};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.

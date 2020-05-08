program Starter;

uses
  Forms,
  main in 'main.pas' {Form1},
  tlo in 'tlo.pas' {TloForm},
  Instruction in 'Instruction.pas' {InstructionForm};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TTloForm, TloForm);
  Application.CreateForm(TInstructionForm, InstructionForm);
  Application.Run;
end.

program Starter;

uses
  Forms,
  main in 'main.pas' {MainForm},
  tlo in 'tlo.pas' {TloForm},
  Instruction in 'Instruction.pas' {InstructionForm};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TTloForm, TloForm);
  Application.CreateForm(TInstructionForm, InstructionForm);
  Application.Run;
end.

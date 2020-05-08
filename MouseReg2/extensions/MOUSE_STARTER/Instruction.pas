unit Instruction;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls;

type
  TInstructionForm = class(TForm)
    InstructionRichEdit: TRichEdit;
    GoButton: TButton;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  InstructionForm: TInstructionForm;

implementation

{$R *.dfm}

end.

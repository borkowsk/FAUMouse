unit Instruction;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls;

type
  TInstructionForm = class(TForm)
    InstructionRichEdit: TRichEdit;
    GoButton: TButton;
    procedure GoButtonClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  InstructionForm: TInstructionForm;

implementation

{$R *.dfm}

procedure TInstructionForm.GoButtonClick(Sender: TObject);
begin
 ModalResult=mrOk;
end;

end.
unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, Spin, Tlo, Instruction, ShellAPI;

var ExpName:string='AZ+LBW';
    Instrukcja0rtf:string='Instrukcja0.rtf';
    MultiLog:string='Eksperymenty.log';
    BatchFile:string='Eksperyment.bat';
    ParPrefix:string='';//"Bad"? Chyba prefix niepotrzebny. Niech sobie Bat coœ z tym robi
    InstructionHeight:integer=-1;

type
  TMainForm = class(TForm)
    GrupaSpinEdit: TSpinEdit;
    BadanySpinEdit: TSpinEdit;
    WiekSpinEdit: TSpinEdit;
    PlecRadioGroup: TRadioGroup;
    Me¿czyznaRadioButton: TRadioButton;
    Label1: TLabel;
    Label2: TLabel;
    KobietaRadioButton: TRadioButton;
    Label3: TLabel;
    StartButton: TButton;
    procedure StartButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.dfm}

procedure TMainForm.FormCreate(Sender: TObject);
var inifile:textFile;
    intpom:integer;
begin
if FileExists('starter.ini') then
begin
  AssignFile(inifile,'starter.ini');
  Reset(inifile);
  Readln(inifile,intpom);
  self.GrupaSpinEdit.Value:=intpom;
  Readln(inifile,intpom);
  self.BadanySpinEdit.Value:=intpom;
  Readln(inifile,intpom);
  self.WiekSpinEdit.Value:=intpom;
  intpom:=-1;
  Readln(inifile,intpom);
  if intpom > -1 then
    InstructionHeight:=intpom;
  CloseFile(inifile);
end
end;

procedure TMainForm.StartButtonClick(Sender: TObject);
//http://stackoverflow.com/questions/6381478/run-a-batch-file-from-button-click-in-delphi7
//http://delphi.about.com/od/windowsshellapi/a/executeprogram.htm
var blad:boolean;
    BatParam:string;
    tekstowy:TextFile;
BEGIN
self.Hide;
Application.ProcessMessages;

//Próba wyœwietlenia instrukcji
blad:=false;
try
if FileExists(Instrukcja0rtf) then
  InstructionForm.InstructionRichEdit.Lines.LoadFromFile(Instrukcja0rtf)
  else blad:=true; //Tu siê obejdzie bez Exception, ale te¿ siê okno nie wyœwietli
except
  on E: Exception do  //Ignoruje te¿ inne b³êdy ale o nich iunformuje
      blad:=true;
end;

if not blad then
begin
  InstructionForm.Position:=poDesktopCenter;//poScreenCenter;
  if InstructionHeight>0 then
    InstructionForm.ClientHeight:=InstructionHeight;
  InstructionForm.ShowModal;
  Application.ProcessMessages;
end;

//Zapis do logu eksperymentów
AssignFile(tekstowy,MultiLog);
if FileExists(MultiLog) then
    Append(tekstowy)
    else
    Rewrite(tekstowy);
writeln(tekstowy,'#',#9,'Grupa',#9,'Badany',#9,'P³eæ',#9,'Wiek',#9,'Czas');
write(tekstowy,ExpName,#9);
write(tekstowy,GrupaSpinEdit.Value,#9);
write(tekstowy,BadanySpinEdit.Value,#9);
if self.KobietaRadioButton.Checked then
      write(tekstowy,'Kob');
if self.Me¿czyznaRadioButton.Checked then
      write(tekstowy,'Mez');
write(tekstowy,#9);
write(tekstowy,self.WiekSpinEdit.Value,#9);
writeln(tekstowy,DateToStr(Date),' ',TimeToStr(Time));
closeFile(tekstowy);

//Tworzenie pliku metryczka
AssignFile(tekstowy,'Metryczka'+inttostr(GrupaSpinEdit.Value)+'_'+inttostr(BadanySpinEdit.Value)+'.txt');
Rewrite(tekstowy);
writeln(tekstowy,'#',#9,'Grupa',#9,'Badany',#9,'P³eæ',#9,'Wiek',#9,'Czas');
write(tekstowy,ExpName,#9);
write(tekstowy,GrupaSpinEdit.Value,#9);
write(tekstowy,BadanySpinEdit.Value,#9);
if self.KobietaRadioButton.Checked then
      write(tekstowy,'Kob');
if self.Me¿czyznaRadioButton.Checked then
      write(tekstowy,'Mez');
write(tekstowy,#9);
write(tekstowy,self.WiekSpinEdit.Value,#9);
writeln(tekstowy,DateToStr(Date),' ',TimeToStr(Time));
closeFile(tekstowy);

//Zapamiêtywanie ostatnich wartoœci formularza
AssignFile(tekstowy,'Starter.ini');
Rewrite(tekstowy);
writeln(tekstowy,GrupaSpinEdit.Value);
writeln(tekstowy,BadanySpinEdit.Value);
writeln(tekstowy,self.WiekSpinEdit.Value);
writeln(tekstowy,self.StartButton.Caption);
writeln(tekstowy,Instrukcja0rtf);
writeln(tekstowy,InstructionForm.ClientWidth);
writeln(tekstowy,InstructionForm.ClientHeight);
writeln(tekstowy,InstructionForm.GoButton.Caption);

closeFile(tekstowy);

//Przygotowanie parametrów wywo³ania i start bat'cha z w³aœciwym eksperymentem
BatParam:=ParPrefix+inttostr(GrupaSpinEdit.Value)+'_'+inttostr(BadanySpinEdit.Value)
        +' '+inttostr(GrupaSpinEdit.Value)+' '+inttostr(BadanySpinEdit.Value);
//TloForm.Show;    // To nie dzia³a bo ShellExecute nie czeka na zakoñczenie
//Application.ProcessMessages;
//sleep(1000);
ShellExecute(
    self.Handle,
    'open',
    'Eksperyment.bat',
    PWideChar(BatParam),
    '.',
    SW_MINIMIZE//SW_SHOW
);

self.Close; //Konczymy dzia³anie startera, bat sobie dzia³a dalej
End;

end.

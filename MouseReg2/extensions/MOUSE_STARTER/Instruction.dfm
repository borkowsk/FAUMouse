object InstructionForm: TInstructionForm
  Left = 0
  Top = 0
  Caption = 'Instruction'
  ClientHeight = 802
  ClientWidth = 805
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object InstructionRichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 809
    Height = 769
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'InstructionRichEdit')
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
  end
  object GoButton: TButton
    Left = 336
    Top = 775
    Width = 161
    Height = 25
    Caption = 'Gotowy, start!'
    TabOrder = 1
  end
end

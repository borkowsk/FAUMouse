object InstructionForm: TInstructionForm
  Left = 0
  Top = 0
  Caption = 'Instrukcja'
  ClientHeight = 670
  ClientWidth = 805
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    805
    670)
  PixelsPerInch = 96
  TextHeight = 13
  object InstructionRichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 809
    Height = 632
    Anchors = [akLeft, akTop, akRight, akBottom]
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
    ExplicitHeight = 665
  end
  object GoButton: TButton
    Left = 312
    Top = 638
    Width = 161
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Gotowy, start!'
    TabOrder = 1
    OnClick = GoButtonClick
    ExplicitTop = 671
  end
end

object InstructionForm: TInstructionForm
  Left = 0
  Top = 0
  Caption = 'Instrukcja'
  ClientHeight = 490
  ClientWidth = 749
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    749
    490)
  PixelsPerInch = 96
  TextHeight = 13
  object InstructionRichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 753
    Height = 452
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
    ExplicitWidth = 809
    ExplicitHeight = 632
  end
  object GoButton: TButton
    Left = 0
    Top = 452
    Width = 753
    Height = 39
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Gotowy, start!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = GoButtonClick
    ExplicitTop = 632
    ExplicitWidth = 809
  end
end

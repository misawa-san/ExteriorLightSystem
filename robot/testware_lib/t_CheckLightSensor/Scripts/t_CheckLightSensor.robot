*** Settings ***
Resource  s_keywords.robot

*** Variables ***

*** Test Cases ***
エンジンの運転状態が検出された場合に、夜間の駐車灯から通過灯への自動移行することを確認(松モデル)
    [Tags]   HILS  SILS  req1  vari_matsu
    待ち時間[s]　＝  0.5
    イグニッション状態　＝  「キー未挿入」
    環境光　＝  「夜」

    待ち時間[s]　＝  0.5
    イグニッション状態　＝  「IG OFF」

    待ち時間[s]　＝  0.5
    イグニッション状態　＝  「IG ON」

    待ち時間[s]　＝  0.5
    Given 駐車灯状態　＝＝  「ON」
    And 通過灯状態　＝＝  「OFF」

    When イグニッション状態　＝  「エンジン始動」
    And 待ち時間[s]　＝  2

    Then 駐車灯状態　＝＝  「OFF」
    And 通過灯状態　＝＝  「ON」
    [Teardown]  テスト環境初期化

エンジンの運転状態が検出された場合に、夜間の駐車灯から通過灯への自動移行することを確認(竹モデル)
    [Tags]   HILS  SILS  req1  vari_take
    イグニッション状態　＝  「IG OFF」
    環境光　＝  「夜」
    待ち時間[s]　＝  0.2
    [Teardown]  テスト環境初期化

エンジンの運転状態が検出された場合に、夜間の駐車灯から通過灯への自動移行することを確認(梅モデル)
    [Tags]   HILS  SILS  req1  vari_ume
    イグニッション状態　＝  「IG OFF」
    環境光　＝  「夜」
    待ち時間[s]　＝  0.3
    [Teardown]  テスト環境初期化

*** Settings ***
Resource  s_keywords.robot

*** Variables ***

*** Test Cases ***
エンジンの運転状態が検出された場合に、夜間の駐車灯から通過灯への自動移行することを確認３
    [Setup]  テスト環境初期化
    [Tags]   HILS  req1  vali_ume
    イグニッション状態　＝  「IG OFF」
    環境光　＝  「夜」
    待ち時間[s]　＝  0.9
    [Teardown]  テスト環境初期化

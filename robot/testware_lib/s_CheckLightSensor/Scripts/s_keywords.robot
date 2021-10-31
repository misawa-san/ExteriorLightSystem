*** Settings ***
Library  s_lib.s_lib_class  ${xils}  WITH NAME  slib
Library  BuiltIn

*** Variables ***
${wtime}   0.5

*** Keywords ***
テスト環境初期化
    slib.init_testing_env

テスト終了処理
    slib.fin_testing_env

イグニッション状態　＝
    [Arguments]         @{igstat}
    slib.set_ig_stat    @{igstat}

環境光　＝
    [Arguments]         @{envlight}
    slib.set_env_light  @{envlight}

待ち時間[s]　＝
    [Arguments]    @{waitime}
    ${wtime} =  Convert To Number  @{waitime}
    slib.wait_time  ${wtime}

駐車灯状態　＝＝
    [Arguments]    @{expected_status}
    ${parkinglight_stat} =  slib.get_parkinglight_status
    Should be Equal as Strings  ${parkinglight_stat}  @{expected_status}

通過灯状態　＝＝
    [Arguments]    @{expected_status}
    ${passinglight_stat} =  slib.get_passinglight_status
    Should be Equal as Strings  ${passinglight_stat}  @{expected_status}

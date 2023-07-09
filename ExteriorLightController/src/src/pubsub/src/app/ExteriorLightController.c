#include "pina.h"
#include "adc.h"
#include "calc_v.h"
#include "ExteriorLightController.h"

volatile uint8_t payload[8] = {0,0,0,0,0,0,0,0};

/* 初期化処理 */
void ExterliorLightControllerInit(void)
{
  payload[0] = 0x30;
  payload[1] = 0x00;
  payload[2] = 0x00;
  payload[3] = 0x00;
  payload[4] = 0x00;
  payload[5] = 0x05;
  payload[6] = 0x06;
  payload[7] = 0x07;
}

/* メイン処理 */
void ExterliorLightControllerMain(void)
{
    uint16_t env_light = 0U;
    uint16_t igv       = 0U;

    /* 外部入力 */
    adc_step();
    calc_v_step();

    /* メイン処理 */
    env_light = calc_v_get_env_light() / CALC_V_ENV_LIGHT_LSBGAIN;
    igv       = calc_v_get_igv()       / CALC_V_IGV_LSBGAIN;

    if( env_light > 2500)  /* 環境光(電圧)＝「夜」の場合 *//* 0.0V-2.5V:昼、2.5V-5.0V:夜 */
    {
        if(igv > 1500)     /* イグニッション状態(電圧)＝「エンジン始動」の場合 *//* 1.5V-2.0V:エンジン始動 */
        {
           payload[1] = 0x00;  /* 駐車灯指示 */
           payload[2] = 0x55;  /* 通過灯指示 */
        }
        else
        {
           payload[1] = 0x55;  /* 駐車灯指示 */
           payload[2] = 0x00;  /* 通過灯指示 */
        }
        payload[3] = (uint8_t)(env_light>>8);
        payload[4] = (uint8_t)env_light;
    }
    else
    {
      /* 前回値保持 */
    }
}

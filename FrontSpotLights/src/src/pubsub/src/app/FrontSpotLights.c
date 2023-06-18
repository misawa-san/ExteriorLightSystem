#include "pina.h"
#include <Arduino.h>
#include "FrontSpotLights.h"

/* メイン周期 */
#define MAIN_CYCLE  (uint32_t)(10UL)

volatile uint8_t icnt1    = 0U;
volatile uint8_t icnt2    = 0U;

uint16_t test_tm = 0U;
uint16_t per = 0U;

volatile uint8_t payload[8];

/* 初期化処理 */
void FrontSpotLightsInit(void)
{
  payload[0] = 0x00;
  payload[1] = 0x00;
  payload[2] = 0x00;
  payload[3] = 0x00;
  payload[4] = 0x00;
  payload[5] = 0x00;
  payload[6] = 0x00;
  payload[7] = 0x00;
}

/* メイン処理 */
void FrontSpotLightsMain(void)
{
  uint8_t parkinglightreq;
  uint8_t passinglightreq;

  parkinglightreq = 0x00;
  passinglightreq = 0x00;

  /* 外部入力 */

  /* メイン処理 */
  switch(payload[0]){
    case 0x30:  /* 駐車灯・通過灯指示コマンド */
      parkinglightreq = payload[1];
      passinglightreq = payload[2];
      break;
    default:
      break;
  }
  /* 外部出力 */
  if( test_tm == 0U ){
      test_tm = 200/MAIN_CYCLE;

      if(parkinglightreq>0){
        icnt1=250;
        icnt2=0;
      }else if(passinglightreq>0){
        icnt1=0;
        icnt2=250;
      }

      analogWrite(PARKINGLIGHT, icnt1);
      analogWrite(PASSINGLIGHT, icnt2);          
  }else{
      test_tm--;
  }
}

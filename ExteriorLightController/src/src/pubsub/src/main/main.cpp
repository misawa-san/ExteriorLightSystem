#include <Arduino.h>
#include <Wire.h>
#include <MsTimer2.h>
#include "ExteriorLightController.h"
#include "pina.h"

volatile boolean main_tm = false;

/* タイマ割り込み */
void timer2int(void)
{
  main_tm = true;
}

/* I2C送信ハンドラ */
void i2c_send(void)
{
  uint8_t i;

  payload[0] = 0x30;

  payload[5] = 0x05;
  payload[6] = 0x06;
  payload[7] = 0x07;

  for(i=0;i<8;i++)
  {
      Wire.write(payload[i]);
  }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(MAIN_TRG, OUTPUT);             /* main_trg   */
    pinMode(ENV_LIGHT, INPUT);             /* env_light  */
    pinMode(IGV, INPUT);                   /* IGV        */

    ExterliorLightControllerInit();

    Serial.begin(9600);
    Wire.begin(0x18);
    Wire.onRequest(i2c_send);

    MsTimer2::set(MAIN_CYCLE, timer2int); // 20ms period
    MsTimer2::start();
}

void loop()
{
    while(true==main_tm)
    {
        // put your main code here, to run repeatedly:
        noInterrupts();
            main_tm = false;
        interrupts();

        digitalWrite(MAIN_TRG, HIGH);

        // メイン処理
        ExterliorLightControllerMain();
 
        digitalWrite(MAIN_TRG, LOW);
  }
}

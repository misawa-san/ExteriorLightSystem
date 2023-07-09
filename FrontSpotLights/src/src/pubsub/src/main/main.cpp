#include <Arduino.h>
#include <Wire.h>
#include <MsTimer2.h>
#include "FrontSpotLights.h"
#include "pina.h"

volatile boolean main_tm = false;

/* タイマ割り込み */
void timer2int(void)
{
  main_tm = true;
}

void setup() {
    // put your setup code here, to run once:
    pinMode(MAIN_TRG, OUTPUT);             /* main_trg   */
    pinMode(PARKINGLIGHT,  OUTPUT);        /* 駐車灯      */
    pinMode(PASSINGLIGHT,  OUTPUT);        /* 通過灯      */

    FrontSpotLightsInit();

    Serial.begin(9600);
    Wire.begin();

    MsTimer2::set(MAIN_CYCLE, timer2int); // 10ms period
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

        Wire.requestFrom(0x18,8);
        while(Wire.available() >= 8)
        {
            for(int i=0;i<8;i++)
            {
                payload[i] = Wire.read();
            }
        }

        // メイン処理
        FrontSpotLightsMain();
 
        digitalWrite(MAIN_TRG, LOW);
  }
}

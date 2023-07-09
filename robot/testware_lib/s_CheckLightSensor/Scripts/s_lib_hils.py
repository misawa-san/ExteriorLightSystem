# coding:utf-8

# ライブラリのインポート
from robot.api  import logger
import pigpio
import time
import spidev

DPATH = '../../../testsuites/CheckLightSensor/d_CheckLightSensor/Data/'

# GPIOピン定義
SPI_CS   =  5   # GPIO5  Pin29

class s_lib_hils_class():

    def __init__(self):
        """初期化"""
        self.pi = pigpio.pi()

    def init_testing_env(self, req):
        """テスト環境を初期化"""

        logger.info("ブレークアウト基盤を初期化します・・")

        # ピン入出力設定
        for pin in req:
            pinnum = req[pin]["ピン番号"]
            inout  = req[pin]["入出力"]
            logger.info(f"ピン番号 　:　{pinnum}　　　:　入出力　：　{inout}")
            if "pigpio.OUTPUT" == inout:
                self.pi.set_mode( pinnum, pigpio.OUTPUT )
            else:
                self.pi.set_mode( pinnum, pigpio.INPUT )   

        # 出力初期値設定
        for pin in req:
            pinnum = req[pin]["ピン番号"]
            inout  = req[pin]["入出力"]
            out    = req[pin]["初期値"]
            if "pigpio.OUTPUT" == inout:
                logger.info(f"ピン番号 　:　{pinnum}　　　:　初期値　：　{out}")
                self.pi.write( pinnum, out)

    def fin_testing_env(self,req):
        """テスト環境の片づけ"""

        self.init_testing_env(req)

    def set_ig_stat(self, key, req):
        """イグニッション状態を設定"""

        # PWM出力
        self.pi.hardware_PWM( req['Port'], req['freq[Hz]'], req['duty[%]']*10000 )

        logger.info(f"IG設定値　:　{key}　Port:{req['Port']}　freq:{req['freq[Hz]']}[Hz]　duty:{req['duty[%]']}[%]")

    def get_adcval_ch0(self, req):
        """MCP3002 CH0からAD値を取得"""

        # SPI設定
        spi = spidev.SpiDev()
        spi.open(0, 0)
        spi.max_speed_hz = 1000000

        # 0x68から読み出し
        self.pi.write( SPI_CS, 0 )
        spival = spi.xfer2([0x68, 0x00])
        self.pi.write( SPI_CS, 1 )
        adval  = ((spival[0] << 8) + spival[1]) & 0x3FF

        logger.info(f"CH0　:　AD値　:　{adval}")

        return adval

    def get_adcval_ch1(self, req):
        """MCP3002 CH1からAD値を取得"""

        # SPI設定
        spi = spidev.SpiDev()
        spi.open(0, 1)
        spi.max_speed_hz = 1000000

        # 0x78から読み出し
        self.pi.write( SPI_CS, 0 )
        spival = spi.xfer2([0x78, 0x00])
        self.pi.write( SPI_CS, 1 )
        adval  = ((spival[0] << 8) + spival[1]) & 0x3FF

        logger.info(f"CH1　:　AD値　:　{adval}")

        return adval

    def get_light_status(self, key, req):
        """ライト点灯状態を取得（「ON」、「OFF」）を取得"""

        # AD値を取得
        ch     = req['ADCチャネル']
        maxval = (2 ** req['ADCビット数']) -1
        logger.info(f"{ch}　:　maxval:　{maxval}  adcbit:  {req['ADCビット数']}")
        if ch == "CH0":
            adval = self.get_adcval_ch0(req)
        elif ch == "CH1":
            adval = self.get_adcval_ch1(req)
        else:
            adval = maxval + 1

        if adval <= maxval:
            # AD電圧値[mV]を算出
            mv = req['1LSB[mV] / LSBゲイン'] * adval
            mv = mv / req['LSBゲイン']
            if mv > req['AD最大電圧[mV]']:
                mv = req['AD最大電圧[mV]']
            logger.info(f"{ch}　:　{key}（電圧）: {mv:.1f} [mV]　ON閾値:{req['ON閾値[mV]']:.1f}[mV]　AD最大電圧:{req['AD最大電圧[mV]']:.1f}[mV]")

            # AD電圧値[mV]がON閾値[mV]未満の場合、ライト点灯と判断
            if mv < req['ON閾値[mV]']:
                return "「ON」"
            # AD電圧値[mV]がON閾値[mV]以上の場合、ライト消灯と判断
            else:
                return "「OFF」"
        else:
            logger.info("AD値異常：")
            return "「UNEXPECTED_ADVALUE」"

    def get_parkinglight_status(self, key, req):
        """駐車灯状態（「ON」、「OFF」）を取得"""

        stat = self.get_light_status(key, req)
        logger.info(f"{key}取得結果　:　{stat}")

        return stat

    def get_passinglight_status(self, key, req):
        """通過灯状態を取得（「ON」、「OFF」）を取得"""

        stat = self.get_light_status(key, req)
        logger.info(f"{key}取得結果　:　{stat}")

        return stat

    def set_env_light(self, key, req):
        """環境光を設定"""

        # PWM出力
        self.pi.hardware_PWM( req['Port'], req['freq[Hz]'], req['duty[%]']*10000 )

        logger.info(f"環境光設定値　:　{key}　Port:{req['Port']}　freq:{req['freq[Hz]']}[Hz]　duty:{req['duty[%]']}[%]")

    def wait_time(self, waitime):
        """時間[s]経過を待つ"""

        time.sleep(waitime)

        logger.info(f"{waitime}[s]待ち")

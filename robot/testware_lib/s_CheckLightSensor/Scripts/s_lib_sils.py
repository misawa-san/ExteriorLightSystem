# coding:utf-8

# ライブラリのインポート
from robot.api  import logger
import time
import json
import requests
from urllib3.util import Retry
from requests.adapters import HTTPAdapter

DPATH = '../../../testsuites/CheckLightSensor/d_CheckLightSensor/Data/'

class s_lib_sils_class():

    def __init__(self):
        """初期化"""
        self.req_time = 0.0

    def init_testing_env(self, req):
        """テスト環境を初期化"""

        logger.info("SILS環境を初期化します・・")
        logger.info(f"init req　:　{req}")

        # ECU時刻の初期化
        self.req_time = 0.0
        self.set_ecu_value("exte_c", "res_ecu_time", 0)
        self.set_ecu_value("fsl_c",  "res_ecu_time", 0)

    def fin_testing_env(self, req):
        """テスト環境の片づけ"""

        logger.info("SILS環境を片づけます・・")

        # ECUを強制終了
        self.set_ecu_value("exte_c", "fin", 1)
        self.set_ecu_value("fsl_c",  "fin", 1)

    def set_ecu_value(self, hostname, key, w_val):

        # mmap情報を読み込み
        file = open(DPATH + 'd_mmap.json')
        reqs = json.load(file)

        # Web API URLを取得する
        url = reqs[hostname]["url"]

        # Web APIの mem_idxを取得する
        mem_idx = reqs[hostname]["mmap"][key]["mem_idx"]

        # Web APIの paramsを作成する
        url_params = {
            'mem_idx' : mem_idx,            # mem_idx
            'req'     : 'write',            # req=write
            'w_val'   : str(w_val),         # w_val
            'req_time': str(self.req_time)  # req_time
        }

        session = requests.Session()
        retries = Retry(total=12, backoff_factor=15, status_forcelist=[500, 502, 503, 504])
        session.mount("http://", HTTPAdapter(max_retries=retries))
        response = session.put(url, params=url_params, timeout=(15, 30)).json()

        logger.info(f"Web API Write Response　:　{response}　　　:　memname　：　{key}")

        return response

    def get_ecu_value(self, hostname, key):

        # mmap情報を読み込み
        file = open(DPATH + 'd_mmap.json')
        reqs = json.load(file)

        # Web API URLを取得する
        url = reqs[hostname]["url"]

        # Web APIの mem_idxを取得する
        mem_idx = reqs[hostname]["mmap"][key]["mem_idx"]

        # Web APIの paramsを作成する
        url_params = {
            'mem_idx' : mem_idx,            # mem_idx
            'req'     : 'read',             # req=read
            'req_time': str(self.req_time)  # req_time
        }

        session = requests.Session()
        retries = Retry(total=12, backoff_factor=15, status_forcelist=[500, 502, 503, 504])
        session.mount("http://", HTTPAdapter(max_retries=retries))
        response = session.get(url, params=url_params, timeout=(15, 30)).json()

        logger.info(f"Web API Read Response　 :　{response}　:　memname　：　{key}")

        return response

    def set_ecu_time(self, hostname, time):

        # mmap情報を読み込み
        file = open(DPATH + 'd_mmap.json')
        reqs = json.load(file)

        # Web API URLを取得する
        url = reqs[hostname]["url"]

        # Web APIの paramsを作成する
        url_params = {
            'req'     : 'time',             # req=read
            'req_time': str(time)           # req_time
        }

        session = requests.Session()
        retries = Retry(total=12, backoff_factor=15, status_forcelist=[500, 502, 503, 504])
        session.mount("http://", HTTPAdapter(max_retries=retries))
        response = session.put(url, params=url_params, timeout=(15, 30)).json()

        logger.info(f"Web API Time Response　:　{response}")

        return response

    def set_ig_stat(self, key, req):
        """イグニッション状態を設定"""

        # Exterior Light ECUの adc_data[IGV]にA/D値を設定
        w_val = int(1024 * (req['duty[%]']/100))
        response = self.set_ecu_value(req["hostname"], req["memname"], w_val)

        # Web APIの戻り値が正常(0)の場合、
        if "0" == response["err"]:
            logger.info(f"IG設定値　:　{key}　duty:{req['duty[%]']}[%]  memname:{req['memname']}")
        # Web APIの戻り値が正常(0)以外の場合、A/D範囲外を設定
        else:
            logger.info("Web API response Error：")

    def get_adcval_ch0(self, req):
        """FrontSpotLight ECUのpwm_data[PARKINGLIGHT])からAD値を算出し取得"""

        # FrontSpotLight ECUから pwm_data[PARKINGLIGHT]の値を取得
        response = self.get_ecu_value(req["hostname"], req["memname"])

        # Web APIの戻り値が正常(0)の場合、AD値を算出
        if "0" == response["err"]:
            pwm_data = int(response["readres"])
            duty     = pwm_data/255
            maxval   = (2 ** req['ADCビット数']) -1
            adval    = int(maxval - (maxval * duty))
            if maxval < adval:
                adval = maxval
        # Web APIの戻り値が正常(0)以外の場合、A/D範囲外を設定
        else:
            logger.info(f"API response error")
            maxval  = (2 ** req['ADCビット数']) -1
            adval   = maxval+1   # A/D範囲外を設定

        logger.info(f"CH0　:　AD値　:　{adval}  memname:{req['memname']}")

        return adval

    def get_adcval_ch1(self, req):
        """FrontSpotLight ECUのpwm_data[PASSINGLIGHT])からAD値を算出し取得"""

        # FrontSpotLight ECUから pwm_data[PASSINGLIGHT]の値を取得
        response = self.get_ecu_value(req["hostname"], req["memname"])

        # Web APIの戻り値が正常(0)の場合、AD値を算出
        if "0" == response["err"]:
            pwm_data = int(response["readres"])
            duty     = pwm_data/255
            maxval   = (2 ** req['ADCビット数']) -1
            adval    = int(maxval - (maxval * duty))
            if maxval < adval:
                adval = maxval
        # Web APIの戻り値が正常(0)以外の場合、A/D範囲外を設定
        else:
            logger.info(f"API response error")
            maxval  = (2 ** req['ADCビット数']) -1
            adval   = maxval+1   # A/D範囲外を設定

        logger.info(f"CH1　:　AD値　:　{adval}  memname:{req['memname']}")
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

        # Exterior Light ECUの adc_data[ENV_LIGHT]にA/D値を設定
        w_val = 1024 - int(1024 * (req['duty[%]']/100))
        response = self.set_ecu_value(req["hostname"], req["memname"], w_val)

        # Web APIの戻り値が正常(0)の場合、
        if "0" == response["err"]:
            logger.info(f"環境光設定値　:　{key}　duty:{req['duty[%]']}[%]  memname:{req['memname']}")
        # Web APIの戻り値が正常(0)以外の場合、A/D範囲外を設定
        else:
            logger.info("Web API response Error：")

    def forced_ecu_com(self, r_memname, w_memname):
        """ECU間通信を強制実行する"""

        # ExteriorLightController ECUから r_memnameの値を取得
        response = self.get_ecu_value("exte_c", r_memname)

         # Web APIの戻り値が正常(0)の場合、r_memnameの値で、FrontLight ECUの w_memnameの値を更新
        if "0" == response["err"]:
            r_val = int(response["readres"])
            response = self.set_ecu_value("fsl_c", w_memname, r_val)
        # Web APIの戻り値が正常(0)以外の場合、A/D範囲外を設定
        else:
            logger.info(f"API response error")

    def wait_time(self, waitime):
        """時間[s]経過を待つ"""

        # ECU request timeを算出する
        req_time = self.req_time + waitime
        self.req_time = req_time

        # ExterirorLightController ECUの時間を waittime進める
        response = self.set_ecu_time("exte_c", self.req_time)

        # FrontSpotLight ECUの時間を waittime進める
        response = self.set_ecu_time("fsl_c", self.req_time)

        logger.info(f"{waitime}[s]待ち")

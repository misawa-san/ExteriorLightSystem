 # coding:utf-8

# ライブラリのインポート
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

        # ECU時刻の初期化
        self.req_time = 0.0
        # self.set_ecu_value("exte_c", "res_ecu_time", 0)
        self.set_ecu_value("fsl_c",  "res_ecu_time", 0)

    def fin_testing_env(self, req):
        """テスト環境の片づけ"""

        # ECUを強制終了
        #self.set_ecu_value("exte_c", "fin", 1)
        self.set_ecu_value("fsl_c",  "fin", 1)

    def set_ecu_value(self, hostname, key, w_val):

        # mmap情報を読み込み
        file = open(DPATH + 'd_mmap_manu.json')
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

        response = requests.put(url, params=url_params).json()

        return response

    def get_ecu_value(self, hostname, key):

        # mmap情報を読み込み
        file = open(DPATH + 'd_mmap_manu.json')
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

        response = requests.get(url, params=url_params).json()

        return response

    def set_ecu_time(self, hostname, time):

        # mmap情報を読み込み
        file = open(DPATH + 'd_mmap_manu.json')
        reqs = json.load(file)

        # Web API URLを取得する
        url = reqs[hostname]["url"]

        # Web APIの paramsを作成する
        url_params = {
            'req'     : 'time',             # req=time
            'req_time': str(time)           # req_time
        }

        response = requests.put(url, params=url_params).json()

        return response


    def wait_time(self, waitime):
        """時間[s]経過を待つ"""

        # ECU request timeを算出する
        req_time = self.req_time + waitime
        self.req_time = req_time

        # ExterirorLightController ECUの時間を waittime進める
        #response = self.set_ecu_time("exte_c", self.req_time)

        # FrontSpotLight ECUの時間を waittime進める
        response = self.set_ecu_time("fsl_c", self.req_time)

        
m_test = s_lib_sils_class()

result = m_test.wait_time(0.04)
print(result)

result = m_test.get_ecu_value("fsl_c", "res_ecu_time")
print(result)

result = m_test.set_ecu_value( "fsl_c", "pwm_data[PARKINGLIGHT]", 0x1234)
print(result)

result = m_test.wait_time(0.02)
print(result)

result = m_test.get_ecu_value("fsl_c", "pwm_data[PARKINGLIGHT]")
print(result)

result = m_test.fin_testing_env("none")
print(result)


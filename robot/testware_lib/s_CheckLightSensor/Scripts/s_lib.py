# coding:utf-8

# ライブラリのインポート
#from s_lib_hils import s_lib_hils_class
#from s_lib_sils import s_lib_sils_class
import importlib
import json
import sys

DPATH = '../../../testsuites/CheckLightSensor/d_CheckLightSensor/Data/'

class s_lib_class():

    def __init__(self, tenv):
        """初期化"""

        if tenv == "HILS":
            self.t = getattr(importlib.import_module('s_lib_hils'), 's_lib_hils_class')()
            self.tenv = tenv
        elif tenv == 'SILS':
            self.t = getattr(importlib.import_module('s_lib_sils'), 's_lib_sils_class')()
            self.tenv = tenv

    def init_testing_env(self):
        """テスト環境を初期化"""

        file = open(DPATH + 'd_init_testing_env.json')
        reqs = json.load(file)

        req = reqs[self.tenv]
        self.t.init_testing_env(req)

    def fin_testing_env(self):
        """テスト環境の片づけ"""

        file = open(DPATH + 'd_init_testing_env.json')
        reqs = json.load(file)

        req = reqs[self.tenv]
        self.t.fin_testing_env(req)

    def set_ig_stat(self, key):
        """イグニッション状態を設定"""

        file = open(DPATH + 'd_set_ig_stat.json')
        reqs = json.load(file)

        req = reqs[self.tenv][key]
        self.t.set_ig_stat(key, req)

    def get_parkinglight_status(self):
        """駐車灯状態（「ON」、「OFF」）を取得"""

        file = open(DPATH + 'd_get_light_status.json')
        reqs = json.load(file)

        key = "駐車灯状態"
        req = reqs[self.tenv][key]
        return self.t.get_parkinglight_status(key, req)

    def get_passinglight_status(self):
        """通過灯状態を取得（「ON」、「OFF」）を取得"""

        file = open(DPATH + 'd_get_light_status.json')
        reqs = json.load(file)

        key = "通過灯状態"
        req = reqs[self.tenv][key]
        return self.t.get_passinglight_status(key, req)

    def set_env_light(self, key):
        """環境光を設定"""

        file = open(DPATH + 'd_set_env_light.json')
        reqs = json.load(file)

        req = reqs[self.tenv][key]
        self.t.set_env_light(key, req)

    def wait_time(self, waitime):
        """時間[s]経過を待つ"""

        self.t.wait_time(waitime)

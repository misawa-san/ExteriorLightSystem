import shutil
from distutils import dir_util

shutil.copyfile('../../../testware_lib/s_CheckLightSensor/Scripts/__init__.robot',   '../../../testsuites/CheckLightSensor/__init__.robot')
dir_util.copy_tree('../../../testware_lib/s_CheckLightSensor',  '../../../testsuites/CheckLightSensor/s_CheckLightSensor')
dir_util.copy_tree('../../../testware_lib/d_CheckLightSensor',  '../../../testsuites/CheckLightSensor/d_CheckLightSensor')
dir_util.copy_tree('../../../testware_lib/t_CheckLightSensor',  '../../../testsuites/CheckLightSensor/t_CheckLightSensor')

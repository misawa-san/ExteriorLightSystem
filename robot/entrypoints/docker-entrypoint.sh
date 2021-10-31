#!/bin/bash

cd result/SILS/reslt_CheckLightSensor/
python3 make_testsuite.py

sleep 20

robot  --xunit xunit_report.xml -i SILS  -v xils:SILS -P ../../../testsuites/CheckLightSensor/s_CheckLightSensor/Scripts  -P ../../../testsuites/CheckLightSensor/t_CheckLightSensor/Scripts ../../../testsuites/CheckLightSensor

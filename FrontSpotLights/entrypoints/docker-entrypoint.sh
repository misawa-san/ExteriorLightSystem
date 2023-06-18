#!/bin/bash

source "/opt/ros/$ROS_DISTRO/setup.bash"
cd src
rm -rf ./build/*
rm -rf ./coverage*
rm -rf ./install
rm -rf ./log

rm -rf app_server2.log
rm -rf publog.txt
rm -rf server.txt

colcon build --cmake-clean-cache
colcon build --symlink-install
source ./install/local_setup.bash
ros2 run pubsub ros_task > publog.txt &
ros_pid=$!

# launch server
python3 -u ../server2.py > server.txt &

# wait until ros task is finished
wait $ros_pid

# create coverage report
basename="coverage_"
dirname=`date "+%Y%m%d-%H%M%S"`
cvrdir="$basename$dirname"
mkdir $cvrdir
cd build/pubsub/src/app/CMakeFiles/libapp.dir
gcov *.gcda > ../../../../../../$cvrdir/coverage.txt

cd ../../../../../../
gcovr  --filter src/pubsub/src/app . --html --html-details -o ./$cvrdir/coverage.html
cp ./build/pubsub/src/app/CMakeFiles/libapp.dir/* ./$cvrdir
cp ./publog.txt ./$cvrdir/simout.csv
cp ../view-vcd.sh ./$cvrdir


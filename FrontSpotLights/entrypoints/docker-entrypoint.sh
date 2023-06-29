#!/bin/bash

source "/opt/ros/$ROS_DISTRO/setup.bash"
cd src
rm -rf ./build/*
rm -rf ./coverage
rm -rf ./install
rm -rf ./log

rm -rf app_server2.log
rm -rf publog.txt
rm -rf server.txt
rm -rf sanitizerlog.txt

colcon build --cmake-clean-cache > clear.txt
export ASAN_OPTIONS="verbosity=2"
colcon build --cmake-args -DCMAKE_CXX_FLAGS="-g -O0 -fsanitize=address -fno-omit-frame-pointer" > build.txt
colcon build --symlink-install

source ./install/local_setup.bash
ros2 run pubsub ros_task > publog.txt 2> sanitizerlog.txt &
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
cp ./sanitizerlog.txt ./$cvrdir/
cp ../view-vcd.sh ./$cvrdir


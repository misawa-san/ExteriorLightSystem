FROM ubuntu:18.04
ENV DEBIAN_FRONTEND noninteractive

ENV ROS_DISTRO dashing

# Set locale
RUN apt-get update \
    && apt-get install -y locales  \
    && dpkg-reconfigure locales \
    && locale-gen ja_JP ja_JP.UTF-8 \
    && update-locale LC_ALL=ja_JP.UTF-8 LANG=ja_JP.UTF-8
ENV LC_ALL   ja_JP.UTF-8
ENV LANG     ja_JP.UTF-8
ENV LANGUAGE ja_JP.UTF-8

# Setup Sources
RUN apt-get update \
    && apt-get install -y sudo \
    && apt-get install build-essential -y \
    && apt-get install -y gcc \
    && apt-get install -y g++ \
    && apt-get install -y cmake \
    && apt-get install -y clang \
    && apt-get install -y llvm \
    && sudo apt-get install -y python3-pip \
    && pip3 install gcovr \
    && pip3 install -U argcomplete \
    && pip3 install flask==1.1.1 \
    && sudo pip3 install sysv_ipc

RUN apt-get update \
    && apt-get install -y curl gnupg2 lsb-release \
    && curl http://repo.ros2.org/repos.key | apt-key add - \
    && sh -c 'echo "deb [arch=amd64,arm64] http://packages.ros.org/ros2/ubuntu \
    && `lsb_release -cs` main" > /etc/apt/sources.list.d/ros2-latest.list' \
    && apt-get update

# Install ROS 2 packages
RUN export ROS_DISTRO=dashing \
    && apt-get install -y ros-$ROS_DISTRO-desktop \
    && sudo python3-colcon-common-extensions python3-rosdep python3-argcomplete \
    && sudo apt install -y ros-dashing-launch* \
    && apt-get install -y ros-$ROS_DISTRO-osrf-testing-tools-cpp \
    && apt-get install -y ros-$ROS_DISTRO-test-msgs \
    && rosdep init \
    && rosdep update

## 環境設定
RUN echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> ~/.bashrc

WORKDIR /usr/ecu_src/FrontSpotLights
COPY ./ /usr/ecu_src/FrontSpotLights
RUN ["chmod", "+x", "./entrypoints/docker-entrypoint.sh"]
ENTRYPOINT [ "./entrypoints/docker-entrypoint.sh" ]

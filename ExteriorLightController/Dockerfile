FROM ros:humble
ENV DEBIAN_FRONTEND noninteractive
ENV ROS_DISTRO humble

# Set locale
RUN apt-get update && apt-get install -y locales && \
    dpkg-reconfigure locales && \
    locale-gen ja_JP ja_JP.UTF-8 && \
    update-locale LC_ALL=ja_JP.UTF-8 LANG=ja_JP.UTF-8

ENV LC_ALL   ja_JP.UTF-8
ENV LANG     ja_JP.UTF-8
ENV LANGUAGE ja_JP.UTF-8

# Setup Sources
RUN apt-get update && \
    apt-get install -y sudo && \
    apt-get install build-essential -y && \
    apt-get install -y gcc && \
    apt-get install -y g++ && \
    apt-get install -y cmake && \
    apt-get install -y clang && \
    apt-get install -y llvm && \
    apt-get install -y lcov && \
    apt-get install -y gdb && \
    apt-get install -y iputils-ping && \
    apt-get install -y net-tools && \
    apt-get install -y netcat && \
    sudo apt-get install -y python3-pip && \
    pip3 install gcovr && \
    pip3 install -U argcomplete && \
    pip3 install flask==2.3.2 && \
    pip3 install requests && \
    sudo pip3 install sysv_ipc

# Install ROS 2 packages
RUN apt-get update && \
    apt-get install -y \
        python3-colcon-common-extensions \
        python3-argcomplete \
        ros-$ROS_DISTRO-example-interfaces \
        ros-$ROS_DISTRO-launch* \
        ros-$ROS_DISTRO-osrf-testing-tools-cpp \
        ros-$ROS_DISTRO-test-msgs && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

## 環境設定
RUN echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> ~/.bashrc
WORKDIR /usr/ecu_src/ExteriorLightController
COPY ./ /usr/ecu_src/ExteriorLightController
RUN groupadd sils
RUN useradd -m -g sils sils
RUN chown -R sils:sils /usr/ecu_src/ExteriorLightController
USER sils
RUN ["chmod", "+x", "./entrypoints/docker-entrypoint.sh"]
ENTRYPOINT [ "./entrypoints/docker-entrypoint.sh" ]

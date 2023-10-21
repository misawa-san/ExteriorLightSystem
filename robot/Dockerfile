FROM ubuntu:22.04

RUN apt-get update \
    && apt-get install -y sudo \
    && sudo apt-get install -y python3-pip \
    && apt-get install -y jq \
    && pip3 install robotframework \
    && pip3 install requests \
    && pip3 install retrying \
    && pip3 install Retry \
    && pip3 install http-adapter \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && useradd -m github \
    && usermod -aG sudo github \
    && echo "%sudo ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /usr/ecu_src/robot
COPY ./ /usr/ecu_src/robot
RUN groupadd sils
RUN useradd -m -g sils sils
RUN chown -R sils:sils /usr/ecu_src/robot
USER sils

RUN ["chmod", "+x", "./entrypoints/docker-entrypoint.sh"]
ENTRYPOINT [ "./entrypoints/docker-entrypoint.sh" ]

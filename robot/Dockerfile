From python:3.8-buster

RUN apt-get update \
    && apt-get install -y sudo \
    && apt-get install -y jq \
    && pip install robotframework \
    && pip install requests \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && useradd -m github \
    && usermod -aG sudo github \
    && echo "%sudo ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /usr/ecu_src/robot
COPY ./ /usr/ecu_src/robot
RUN ["chmod", "+x", "./entrypoints/docker-entrypoint.sh"]
ENTRYPOINT [ "./entrypoints/docker-entrypoint.sh" ]

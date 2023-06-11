#!/bin/sh

sudo chmod -R 777 .
rm -rf ./src/build
rm -rf ./src/coverage*
rm -rf ./src/install
rm -rf ./src/log
rm -rf ./src/app_server2.log
rm -rf ./src/publog.txt
rm -rf ./src/server.txt
docker image prune
docker rm -f `docker ps -a -q`

docker image build -t c_fsl:v1 .
docker run --name c_fsl -e PORT=8080 -v ./:/usr/ecu_src/FrontSpotLights -it c_fsl:v1 /bin/bash


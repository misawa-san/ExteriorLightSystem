#!/bin/sh

sudo chmod -R 777 .
python3 -u ../../../Timeseries2VCDMakerInput/csvConv/src/csvConv.py ./simout.csv
vcdMaker -t us -o ./output.vcd -u ../../../Timeseries2VCDMakerInput/mysample/simout.xml ./output_simout.csv
gtkwave ./output.vcd 


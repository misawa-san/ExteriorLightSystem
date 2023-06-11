# -Timeseries2VCDMakerInput
時系列データをVcdMakerの入力形式ファイルに変換する。(This tool translates time series datas to the input format of VcdMaker)

# how to translate a time serias data to the input format of VcdMaker
> ./csvConv.exe input-time-series-datas.csv

 -> input-time-series-datas.txt will be created!
 
 ex) ./csvConv.exe simout.csv
 
     -> simout.txt will be created by csvConv.exe !
        simout.csv : Please check sample folder.
        
# VcdMaker
You can download the VcdMaker the below URL.
Thank you very much!
https://vcdmaker.org/

# how to translate the input format of vcdMaker
> .\vcdMaker -t ms -o .\output\output.vcd -u simout.xml .\input\simout.csv

  -> output.vcd will be created by vcdMaker!

# gtkWave
You can download the gtkWave the below URL.
Thank you very much!
https://gtkwave.sourceforge.net/

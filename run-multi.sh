#!/bin/bash

# ソースディレクトリとターゲットディレクトリのパスを指定
source_dir="./robot/testware_lib/s_CheckLightSensor/Scripts/ready"
target_dir="./robot/testware_lib/s_CheckLightSensor/Scripts/"

# ソースディレクトリ内のすべての.pyファイルを処理
for source_file in "$source_dir"/*.py; do
    if [ -f "$source_file" ]; then
        # ファイル名を取得
        file_name=$(basename "$source_file")

        # ファイルをターゲットディレクトリにコピー
        cp "$source_file" "$target_dir/s_lib_sils.py"

        # コマンドを実行
        ./run-single.sh
        result=$?
        if [ "$result" -eq 0 ]; then
            echo
        else
            exit 1
        fi
    fi
done

basename="artifact_"
postfix=$(date "+%Y%m%d-%H%M%S")
artifact_name="$basename$postfix.zip"
zip -r "$artifact_name" .

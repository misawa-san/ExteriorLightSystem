#!/bin/bash

echo y | docker image prune
echo y | docker builder prune
docker compose -f docker-compose.yml up -d --build

# モニタリングするディレクトリのパス
monitor_dir="./FrontSpotLights/src"

# タイムアウトの秒数
timeout_seconds=1800000

# タイムアウト時のメッセージ
timeout_message="timeout occured. check your source code"


# タイムアウト処理用の関数
timeout_function() {
    sleep "$timeout_seconds"

    cat <<EOF
            +=====================================+
            |                                     |
            |        タイムアウトが発生しました   |
            |                                     |
            |      ヾ(＾-＾)ﾉ  バグバグバグ!      |
            |                                     |
            +=====================================+
EOF
    echo; echo  "$timeout_message"

    local saved_self_pid=$(cat /tmp/self_pid)
    kill  "$saved_self_pid" 2>/dev/null
    rm -rf /tmp/self_pid
    exit 1
}

echo $$ > /tmp/self_pid

# タイムアウト処理のバックグラウンド実行
timeout_function &
timeout_pid=$!

# inotifywait をループ内で実行
while true; do
    # inotifywait コマンドでイベントを監視
    event=$(inotifywait -q -e create "$monitor_dir")
    inotify_pid=$!

    # イベントを解析
    path=$(echo "$event"   | awk '{print $1}')
    action=$(echo "$event" | awk '{print $2}')
    file=$(echo "$event"   | awk '{print $3}')

    #echo ; echo "path $path action $action file $file"

    if [[ "$action" == "CREATE,ISDIR" && "$file" == coverage_* ]]; then
        cd $monitor_dir
        cd $file
  
        while true; do
            if [ -f ./simout.csv ]; then
                python3 -u ../../../Timeseries2VCDMakerInput/csvConv/src/csvConv.py ./simout.csv
                vcdMaker -t us -o ./output.vcd -u ../../../Timeseries2VCDMakerInput/mysample/simout.xml ./output_simout.csv
                mkdir ../"$file"-1
                cp ./output.vcd ../"$file"-1

                kill "$timeout_pid"
                kill "$inotify_pid"
                rm -rf /tmp/self_pid
                cat <<EOF
            +=====================================+
            |                                     |
            |        成功しました                 |
            |                                     |
            |      ╰(° ▽ °)╯  よかったね!         |
            |                                     |
            +=====================================+
EOF
                echo
                exit # 条件が満たされたらループを抜ける
            fi
        done
    fi

    if [ -f "$monitor_dir/log/latest_build/pubsub/stdout_stderr.log" ]; then
        result=$(grep error "$monitor_dir/log/latest_build/pubsub/stdout_stderr.log")
        if [[ -n "$result" ]]; then
            cat <<EOF
            +=====================================+
            |                                     |
            |        バグが発生しました           |
            |                                     |
            |      ヾ(＾-＾)ﾉ  バグバグバグ!      |
            |                                     |
            +=====================================+
EOF

            echo; echo Your source code might have a bug code. check the code below.
            echo $result; echo
            kill "$timeout_pid"
            kill "$inotify_pid"
            rm -rf /tmp/self_pid
            exit 1
        fi
    fi

    if [ -f "$monitor_dir/sanitizerlog.txt" ]; then
        result=$(grep ====== "$monitor_dir/sanitizerlog.txt")
        if [[ -n "$result" ]]; then
            cat <<EOF
            +=====================================+
            |                                     |
            |        メモリが破壊されました       |
            |                                     |
            |      ヾ(＾-＾)ﾉ  バグバグバグ!      |
            |                                     |
            +=====================================+
EOF

            echo; echo Sanitizer detects the memory has been broken. check the code below.
            echo $result; echo
            kill "$timeout_pid"
            kill "$inotify_pid"
            rm -rf /tmp/self_pid
            exit 1
        fi
    fi

    echo; docker ps

done

kill "$timeout_pid"
kill "$inotify_pid"
rm -rf /tmp/self_pid
echo "Finish"

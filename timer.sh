#!/bin/bash
start_time=`date --date='0 days ago' "+%Y-%m-%d %H:%M:%S"`
python3 test/system/system_test.py ./shell large
finish_time=`date --date='0 days ago' "+%Y-%m-%d %H:%M:%S"`
duration=$(($(($(date +%s -d "$finish_time")-$(date +%s -d "$start_time")))))
echo "this shell script execution duration: $duration"

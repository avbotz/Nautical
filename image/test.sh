#!/bin/bash
mkdir images
#dir="/home/user/Aquastorm/logs"
#var="2018_07_22"
#server="user@192.168.2.3"
#while true; do
#file=$(ssh $server 'ls -t /home/user/Aquastorm/logs/2018_07_22/ | head -1')
file=modeling_img.png
#echo $file
#while true; do
#scp $server:$dir/$file images
#expect "user@192.168.2.3's password:"
#sleep 1
#send "marlin123\r"
#./showimg.py images/$file
#done

while true; do
./imagetransfer.py
./showimg.py images/modeling_img.png
done
#!/bin/bash
while true;do
	fio basic-verify.fio
	sleep 1
	hdparm -I /dev/sda4
done
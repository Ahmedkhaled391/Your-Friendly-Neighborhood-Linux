#!/bin/bash
echo " OS Info "
lsb_release -a 2>/dev/null
echo
echo  " Kernel "
uname -r
echo
echo " CPU "
lscpu | grep -E "Model name|CPU\\(s\\)|MHz"
echo
echo " Memory "
free -h
echo
echo " Disk "
df -h /
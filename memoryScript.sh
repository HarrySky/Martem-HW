# BASH Memory Usage Parse Script:
# bash -f memoryScript.sh
# bash -f memoryScript.sh /proc/meminfo

if [ "$1" = "" ]
then
	TOTAL=$((`awk '/MemTotal/ {print $2 / 1024 }' /proc/meminfo`))
	FREE=$((`awk '/MemFree/ {print $2 / 1024 }' /proc/meminfo`))
else
	TOTAL=$((`awk '/MemTotal/ {print $2 / 1024 }' $1`))
	FREE=$((`awk '/MemFree/ {print $2 / 1024 }' $1`))
fi

USED=$((TOTAL - FREE))
printf "RAM: %d MB used / %d MB total ( %d MB free )\n" $USED $TOTAL $FREE
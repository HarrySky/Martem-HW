# BASH Memory Usage Parse Script:
# bash -f memoryScript.sh
# bash -f memoryScript.sh /proc/meminfo

if [ "$1" = "" ]
then
	TOTAL=$((`awk '/MemTotal/ {print $2}' /proc/meminfo | sed s/...$//`))
	FREE=$((`awk '/MemFree/ {print $2}' /proc/meminfo | sed s/...$//`))
else
	TOTAL=$((`awk '/MemTotal/ {print $2}' $1 | sed s/...$//`))
	FREE=$((`awk '/MemFree/ {print $2}' $1 | sed s/...$//`))
fi

USED=$((TOTAL - FREE))
printf "RAM: %d MB used / %d MB total ( %d MB free )\n" $USED $TOTAL $FREE

#!/bin/sh


export LD_LIBRARY_PATH=`ls -d /home/holger/Projekte/Titania/*/bin/Debug/ | perl -p -e  's/\s+/:/'`
export XDG_DATA_DIRS=$XDG_DATA_DIRS":/home/holger/Projekte/Titania/Puck/share"

#zenity --info --text=$1

if test "$1" = "-t"
then
	gnome-terminal -x /home/holger/Projekte/Titania/Puck/bin/Release/puck "$@"
else
	/home/holger/Projekte/Titania/Puck/bin/Debug/puck "$@" | /home/holger/Projekte/Titania/miscellaneous/benchmarks/save.pl
fi

#!/bin/sh

CONFIG_NAME="Debug"
PROJECT_DIR=$2
FILE=$3

if [$PROJECT_DIR = ""]
then
	PROJECT_DIR="/home/holger/Projekte/Titania"
fi

export LD_LIBRARY_PATH=`ls -d "$PROJECT_DIR/"*"/bin/$CONFIG_NAME/" | perl -p -e  's/\s+/:/'`
export XDG_DATA_DIRS=$XDG_DATA_DIRS":$PROJECT_DIR/Puck/share"

#zenity --info --text=$1

echo "Starting Puck Test Solution"

if test "$1" = "-t"
then
	gnome-terminal -x "$PROJECT_DIR/Puck/bin/$CONFIG_NAME/puck" "$3"
else
	"$PROJECT_DIR/Puck/bin/$CONFIG_NAME/puck" "$3" | "$PROJECT_DIR/miscellaneous/benchmarks/save.pl"
fi

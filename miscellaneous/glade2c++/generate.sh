#!/bin/sh

echo "Generating Interfaces\n"

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-v \
-n titania -n puck \
-p X3D \
-s UI \
-b /home/holger/Projekte/Titania/Puck/puck/Base/X3DUserInterface.h \
-o /home/holger/Projekte/Titania/Puck/puck/UserInterfaces \
`find /home/holger/Projekte/Titania/Puck/share/titania/puck/ui/ -name \*.ui;`

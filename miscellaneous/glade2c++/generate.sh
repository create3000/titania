#!/bin/sh

echo "Generating Interfaces\n"

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-v \
-n titania -n puck \
-p X3D \
-s UI \
-b /home/holger/Projekte/Titania/Titania/puck/Base/X3DUserInterface.h \
-o /home/holger/Projekte/Titania/Titania/puck/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/puck/ui/ -name \*.ui;`

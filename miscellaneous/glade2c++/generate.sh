#!/bin/sh

echo "Generating Interfaces\n"

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DUserInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/ -name \*.ui;`

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DDialogInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Dialogs/ -name \*.ui;`

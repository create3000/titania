#!/bin/sh

echo "Generating Interfaces\n"

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DUserInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/ -maxdepth 1 -name \*.glade;`

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DDialogInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Dialogs/ -maxdepth 1 -name \*.glade;`

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DEditorInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Editors/ -maxdepth 1 -name \*.glade;`

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DOverlayInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Overlays/ -maxdepth 1 -name \*.glade;`

#!/bin/sh

echo "Generating Interfaces\n"

echo "\nUserInterfaces\n";

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DUserInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/ -maxdepth 1 -name \*.glade;`

echo "\nDialogs\n";

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DDialogInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Dialogs/ -maxdepth 1 -name \*.glade;`

echo "\nEditors\n";

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DEditorInterface.h \
-d /home/holger/Projekte/Titania/Titania/Titania/Editors/ProjectsEditor \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Editors/ -maxdepth 1 -name \*.glade;`

echo "\nPanels\n";

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DPanelInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Panels/ -maxdepth 1 -name \*.glade;`

echo "\nRevealer\n";

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DRevealerInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Revealer/ -maxdepth 1 -name \*.glade;`

echo "\nWidgets\n";

/home/holger/Projekte/Titania/miscellaneous/glade2c++/glade2c++.pl \
-e \
-v \
-n titania -n puck \
-p X3D \
-s Interface \
-b /home/holger/Projekte/Titania/Titania/Titania/Base/X3DUserInterface.h \
-o /home/holger/Projekte/Titania/Titania/Titania/UserInterfaces \
`find /home/holger/Projekte/Titania/Titania/share/titania/ui/Widgets/ -maxdepth 1 -name \*.glade;`

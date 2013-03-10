<?php header('Content-type: x-world/x-vrml') ?>
<?php if (!@readfile('index.wrl')): ?>
#VRML V2.0 utf8

Transform {
children	Shape {
geometry	Text {
string	[ "ERROR", "index.wrl not found" ]
fontStyle	FontStyle {
size	6
family	"SANS"
style	"PLAIN"
justify	"MIDDLE"
}
}
}
translation	0 0.5 -4.5
scale	0.05 0.05 0.05
}
DEF VP1 Viewpoint {
position	0 0 1
}
<?php endif ?>

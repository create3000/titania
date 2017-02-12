[VRML]

The Virtual Reality Modeling Language

   Version 1.0 Specification

26-MAY-95

     Gavin Bell, Silicon Graphics, Inc.
     Anthony Parisi, Intervista Software
     Mark Pesce, VRML List Moderator

-------------------------------------------------------------------------------

Acknowledgements

     I want to thank three people who have been absolutely instrumental in
     the design process: Brian Behlendorf, whose drive (and disk space)
     made this process happen; and Tony Parisi and Gavin Bell, the final
     authors of this specification, who have put in a great deal of design
     work, ensuring that we have a satisfactory product. My hat goes off
     to all of them, and to all of you who have made this process a
     success.

Mark Pesce

     I would like to add a personal note of thanks to Jan Hardenbergh of
     Oki Advanced Products for his diligent efforts to keep the
     specification process on track, and his invaluable editing
     assistance. I would also like to acknowledge Chris Marrin of Silicon
     Graphics for his timely contributions to the final design.

Tony Parisi

Revision History

   * First Draft - November 2, 1994
   * Second Draft - May 8, 1995
   * Third Draft - May 26, 1995

Table of Contents

   *  Introduction

        o  VRML Mission Statement
        o  History
        o  Version 1.0 Requirements

   *  Language Specification

        o  Language Basics
        o  Coordinate System
        o  Fields
        o  Nodes
        o  Instancing
        o  Extensibility
        o  An Example

   *  Browser Considerations

        o  File Extensions
        o  MIME Types

-------------------------------------------------------------------------------

   Introduction

The Virtual Reality Modeling Language (VRML) is a language for describing
multi-participant interactive simulations -- virtual worlds networked via the
global Internet and hyperlinked with the World Wide Web. All aspects of virtual
world display, interaction and internetworking can be specified using VRML. It
is the intention of its designers that VRML become the standard language for
interactive simulation within the World Wide Web.

The first version of VRML allows for the creation of virtual worlds with
limited interactive behavior. These worlds can contain objects which have
hyperlinks to other worlds, HTML documents or other valid MIME types. When the
user selects an object with a hyperlink, the appropriate MIME viewer is
launched. When the user selects a link to a VRML document from within a
correctly configured WWW browser, a VRML viewer is launched. Thus VRML viewers
are the perfect companion applications to standard WWW browsers for navigating
and visualizing the Web. Future versions of VRML will allow for richer
behaviors, including animations, motion physics and real-time multi-user
interaction.

This document specifies the features and syntax of Version 1.0 of VRML.

   VRML Mission Statement

The history of the development of the Internet has had three distinct phases;
first, the development of the TCP/IP infrastructure which allowed documents and
data to be stored in a proximally independent way; that is, Internet provided a
layer of abstraction between data sets and the hosts which manipulated them.
While this abstraction was useful, it was also confusing; without any clear
sense of "what went where", access to Internet was restricted to the class of
sysops/net surfers who could maintain internal cognitive maps of the data
space.

Next, Tim Berners-Lee's work at CERN, where he developed the hypermedia system
known as World Wide Web, added another layer of abstraction to the existing
structure. This abstraction provided an "addressing" scheme, a unique
identifier (the Universal Resource Locator), which could tell anyone "where to
go and how to get there" for any piece of data within the Web. While useful, it
lacked dimensionality; there's no there there within the web, and the only type
of navigation permissible (other than surfing) is by direct reference. In other
words, I can only tell you how to get to the VRML Forum home page by saying,
"http://www.wired.com/", which is not human-centered data. In fact, I need to
make an effort to remember it at all. So, while the World Wide Web provides a
retrieval mechanism to complement the existing storage mechanism, it leaves a
lot to be desired, particularly for human beings.

Finally, we move to "perceptualized" Internetworks, where the data has been
sensualized, that is, rendered sensually. If something is represented
sensually, it is possible to make sense of it. VRML is an attempt (how
successful, only time and effort will tell) to place humans at the center of
the Internet, ordering its universe to our whims. In order to do that, the most
important single element is a standard that defines the particularities of
perception. Virtual Reality Modeling Language is that standard, designed to be
a universal description language for multi-participant simulations.

These three phases, storage, retrieval, and perceptualization are analogous to
the human process of consciousness, as expressed in terms of semantics and
cognitive science. Events occur and are recorded (memory); inferences are drawn
from memory (associations), and from sets of related events, maps of the
universe are created (cognitive perception). What is important to remember is
that the map is not the territory, and we should avoid becoming trapped in any
single representation or world-view. Although we need to design to avoid
disorientation, we should always push the envelope in the kinds of experience
we can bring into manifestation!

This document is the living proof of the success of a process that was
committed to being open and flexible, responsive to the needs of a growing Web
community. Rather than re-invent the wheel, we have adapted an existing
specification (Open Inventor) as the basis from which our own work can grow,
saving years of design work and perhaps many mistakes. Now our real work can
begin; that of rendering our noospheric space.

   History

VRML was conceived in the spring of 1994 at the first annual World Wide Web
Conference in Geneva, Switzerland. Tim Berners-Lee and Dave Raggett organized a
Birds-of-a-Feather (BOF) session to discuss Virtual Reality interfaces to the
World Wide Web. Several BOF attendees described projects already underway to
build three dimensional graphical visualization tools which interoperate with
the Web. Attendees agreed on the need for these tools to have a common language
for specifying 3D scene description and WWW hyperlinks -- an analog of HTML for
virtual reality. The term Virtual Reality Markup Language (VRML) was coined,
and the group resolved to begin specification work after the conference. The
word 'Markup' was later changed to 'Modeling' to reflect the graphical nature
of VRML.

Shortly after the Geneva BOF session, the www-vrml mailing list was created to
discuss the development of a specification for the first version of VRML. The
response to the list invitation was overwhelming: within a week, there were
over a thousand members. After an initial settling-in period, list moderator
Mark Pesce of Labyrinth Group announced his intention to have a draft version
of the specification ready by the WWW Fall 1994 conference, a mere five months
away. There was general agreement on the list that, while this schedule was
aggressive, it was achievable provided that the requirements for the first
version were not too ambitious and that VRML could be adapted from an existing
solution. The list quickly agreed upon a set of requirements for the first
version, and began a search for technologies which could be adapted to fit the
needs of VRML.

The search for existing technologies turned up a several worthwhile candidates.
After much deliberation the list came to a consensus: the Open Inventor ASCII
File Format from Silicon Graphics, Inc. The Inventor File Format supports
complete descriptions of 3D scenes with polygonally rendered objects, lighting,
materials, ambient properties and realism effects. A subset of the Inventor
File Format, with extensions to support networking, forms the basis of VRML.
Gavin Bell of Silicon Graphics has adapted the Inventor File Format for VRML,
with design input from the mailing list. SGI has publicly stated that the file
format is available for use in the open market, and have contributed a file
format parser into the public domain to bootstrap VRML viewer development.

   Version 1.0 Requirements

VRML 1.0 is designed to meet the following requirements:

   *  Platform independence
   *  Extensibility
   *  Ability to work well over low-bandwidth connections

As with HTML, the above are absolute requirements for a network language
standard; they should need little explanation here.

Early on the designers decided that VRML would not be an extension to HTML.
HTML is designed for text, not graphics. Also, VRML requires even more finely
tuned network optimizations than HTML; it is expected that a typical VRML scene
will be composed of many more "inline" objects and served up by many more
servers than a typical HTML document. Moreover, HTML is an accepted standard,
with existing implementations that depend on it. To impede the HTML design
process with VRML issues and constrain the VRML design process with HTML
compatibility concerns would be to do both languages a disservice. As a network
language, VRML will succeed or fail independent of HTML.

It was also decided that, except for the hyperlinking feature, the first
version of VRML would not support interactive behaviors. This was a practical
decision intended to streamline design and implementation. Design of a language
for describing interactive behaviors is a big job, especially when the language
needs to express behaviors of objects communicating on a network. Such
languages do exist; if we had chosen one of them, we would have risked getting
into a "language war." People don't get excited about the syntax of a language
for describing polygonal objects; people get very excited about the syntax of
real languages for writing programs. Religious wars can extend the design
process by months or years. In addition, networked inter-object operation
requires brokering services such as those provided by CORBA or OLE, services
which don't exist yet within WWW; we would have had to invent them. Finally, by
keeping behaviors out of Version 1, we have made it a much smaller task to
implement a viewer. We acknowledge that support for arbitrary interactive
behaviors is critical to the long-term success of VRML; they will be included
in Version 2.

-------------------------------------------------------------------------------

   Language Specification

The language specification is divided into the following sections:

   *  Language Basics
   *  Coordinate System
   *  Fields
   *  Nodes
   *  Instancing
   *  Extensibility
   *  An Example

   Language Basics

At the highest level of abstraction, VRML is just a way for objects to read and
write themselves. Theoretically, the objects can contain anything -- 3D
geometry, MIDI data, JPEG images, anything. VRML defines a set of objects
useful for doing 3D graphics. These objects are called Nodes.

Nodes are arranged in hierarchical structures called scene graphs. Scene graphs
are more than just a collection of nodes; the scene graph defines an ordering
for the nodes. The scene graph has a notion of state -- nodes earlier in the
scene can affect nodes that appear later in the scene. For example, a Rotation
or Material node will affect the nodes after it in the scene. A mechanism is
defined to limit the effects of properties (separator nodes), allowing parts of
the scene graph to be functionally isolated from other parts.

A node has the following characteristics:

   *  What kind of object it is. A node might be a cube, a sphere, a texture
     map, a transformation, etc.

   *  The parameters that distinguish this node from other nodes of the same
     type. For example, each Sphere node might have a different radius, and
     different texture maps nodes will certainly contain different images to
     use as the texture maps. These parameters are called Fields. A node can
     have 0 or more fields.

   *  A name to identify this node. Being able to name nodes and refer to them
     elsewhere is very powerful; it allows a scene's author to give hints to
     applications using the scene about what is in the scene, and creates
     possibilities for very powerful scripting extensions. Nodes do not have to
     be named, but if they are named, they can have only one name. However,
     names do not have to be unique-- several different nodes may be given the
     same name.

   *  Child nodes. Object hierarchy is implemented by allowing some types of
     nodes to contain other nodes. Parent nodes traverse their children in
     order during rendering. Nodes that may have children are referred to as
     group nodes. Group nodes can have zero or more children.

The syntax chosen to represent these pieces of information is straightforward:

DEF objectname objecttype { fields  children }

Only the object type and curly braces are required; nodes may or may not have a
name, fields, and children.

Node names must not begin with a digit, and must not contain spaces or control
characters, single or double quote characters, backslashes, curly braces, the
plus character or the period character.

For example, this file contains a simple scene defining a view of a red cone
and a blue sphere, lit by a directional light:

#VRML V1.0 ascii
Separator {
    DirectionalLight {
        direction 0 0 -1  # Light shining from viewer into scene
    }
    PerspectiveCamera {
        position    -8.6 2.1 5.6
        orientation -0.1352 -0.9831 -0.1233  1.1417
        focalDistance       10.84
    }
    Separator {   # The red sphere
        Material {
            diffuseColor 1 0 0   # Red
        }
        Translation { translation 3 0 1 }
        Sphere { radius 2.3 }
    }
    Separator {  # The blue cube
        Material {
            diffuseColor 0 0 1  # Blue
        }
        Transform {
            translation -2.4 .2 1
            rotation 0 1 1  .9
        }
        Cube {}
    }


General Syntax

For easy identification of VRML files, every VRML file must begin with the
characters:

#VRML V1.0 ascii

Any characters after these on the same line are ignored. The line is terminated
by either the ASCII newline or carriage-return characters.

The '#' character begins a comment; all characters until the next newline or
carriage return are ignored. The only exception to this is within string
fields, where the '#' character will be part of the string.

Note: Comments and whitespace may not be preserved; in particular, a VRML
document server may strip comments and extraneous whitespace from a VRML file
before transmitting it. Info nodes should be used for persistent information
like copyrights or author information. Info nodes could also be used for object
descriptions.

Blanks, tabs, newlines and carriage returns are whitespace characters wherever
they appear outside of string fields. One or more whitespace characters
separates the syntactical entities in VRML files, where necessary.

After the required header, a VRML file contains exactly one VRML node. That
node may of course be a group node, containing any number of other nodes.

   Coordinate System

VRML uses a cartesian, right-handed, 3-dimensional coordinate system. By
default, objects are projected onto a 2-dimensional device by projecting them
in the direction of the positive Z axis, with the positive X axis to the right
and the positive Y axis up. A camera or modeling transformation may be used to
alter this default projection.

The standard unit for lengths and distances specified is meters. The standard
unit for angles is radians.

   Fields

There are two general classes of fields; fields that contain a single value
(where a value may be a single number, a vector, or even an image), and fields
that contain multiple values. Single-valued fields all have names that begin
with "SF", multiple-valued fields have names that begin with "MF". Each field
type defines the format for the values it writes.

Multiple-valued fields are written as a series of values separated by commas,
all enclosed in square brackets. If the field has zero values then only the
square brackets ("[]") are written. The last may optionally be followed by a
comma. If the field has exactly one value, the brackets may be omitted and just
the value written. For example, all of the following are valid for a
multiple-valued field containing the single integer value 1:


[1,]
[ 1 ]

SFBitMask

A single-value field that contains a mask of bit flags. Nodes that use this
field class define mnemonic names for the bit flags. SFBitMasks are written to
file as one or more mnemonic enumerated type names, in this format:

( flag1 | flag2 | ... )

If only one flag is used in a mask, the parentheses are optional. These names
differ among uses of this field in various node classes.

SFBool

A field containing a single boolean (true or false) value. SFBools may be
written as 0 (representing FALSE), 1, TRUE, or FALSE.

SFColor

A single-value field containing a color. SFColors are written to file as an RGB
triple of floating point numbers in standard scientific notation, in the range
0.0 to 1.0.

SFEnum

A single-value field that contains an enumerated type value. Nodes that use
this field class define mnemonic names for the values. SFEnums are written to
file as a mnemonic enumerated type name. The name differs among uses of this
field in various node classes.

SFFloat

A field that contains one single-precision floating point number. SFFloats are
written to file in standard scientific notation.

SFImage

A field that contain an uncompressed 2-dimensional color or greyscale image.

SFImages are written to file as three integers representing the width, height
and number of components in the image, followed by width*height hexadecimal
values representing the pixels in the image, separated by whitespace. A
one-component image will have one-byte hexadecimal values representing the
intensity of the image. For example, 0xFF is full intensity, 0x00 is no
intensity. A two-component image puts the intensity in the first (high) byte
and the transparency in the second (low) byte. Pixels in a three-component
image have the red component in the first (high) byte, followed by the green
and blue components (so 0xFF0000 is red). Four-component images put the
transparency byte after red/green/blue (so 0x0000FF80 is semi-transparent
blue). A value of 1.0 is completely transparent, 0.0 is completely opaque.
Note: each pixel is actually read as a single unsigned number, so a 3-component
pixel with value "0x0000FF" can also be written as "0xFF" or "255" (decimal).
Pixels are specified from left to right, bottom to top. The first hexadecimal
value is the lower left pixel of the image, and the last value is the upper
right pixel.

For example,

1 2 1 0xFF 0x00

is a 1 pixel wide by 2 pixel high greyscale image, with the bottom pixel white
and the top pixel black. And:

2 4 3 0xFF0000 0xFF00 0 0 0 0 0xFFFFFF 0xFFFF00

is a 2 pixel wide by 4 pixel high RGB image, with the bottom left pixel red,
the bottom right pixel green, the two middle rows of pixels black, the top left
pixel white, and the top right pixel yellow.

SFLong

A field containing a single long (32-bit) integer. SFLongs are written to file
as an integer in decimal, hexadecimal (beginning with '0x') or octal (beginning
with '0') format.

SFMatrix

A field containing a transformation matrix. SFMatrices are written to file in
row-major order as 16 floating point numbers separated by whitespace. For
example, a matrix expressing a translation of 7.3 units along the X axis is
written as:

1 0 0 0  0 1 0 0  0 0 1 0  7.3 0 0 1

SFRotation

A field containing an arbitrary rotation. SFRotations are written to file as
four floating point values separated by whitespace. The 4 values represent an
axis of rotation followed by the amount of right-handed rotation about that
axis, in radians. For example, a 180 degree rotation about the Y axis is:

0 1 0  3.14159265

SFString

A field containing an ASCII string (sequence of characters). SFStrings are
written to file as a sequence of ASCII characters in double quotes (optional if
the string doesn't contain any whitespace). Any characters (including newlines)
may appear within the quotes. To include a double quote character within the
string, precede it with a backslash. For example:

Testing
"One, Two, Three"
"He said, \"Immel did it!\""

are all valid strings.

SFVec2f

Field containing a two-dimensional vector. SFVec2fs are written to file as a
pair of floating point values separated by whitespace.

SFVec3f

Field containing a three-dimensional vector. SFVec3fs are written to file as
three floating point values separated by whitespace.

MFColor

A multiple-value field that contains any number of RGB colors. MFColors are
written to file as one or more RGB triples of floating point numbers in
standard scientific notation. When more than one value is present, all of the
values must be enclosed in square brackets and separated by commas. For
example:

[ 1.0 0.0 0.0, 0 1 0, 0 0 1 ]

represents the three colors red, green, and blue.

MFLong

A multiple-value field that contains any number of long (32-bit) integers.
MFLongs are written to file as one or more integer values, in decimal,
hexadecimal or octal format. When more than one value is present, all the
values are enclosed in square brackets and separated by commas; for example:

[ 17, -0xE20, -518820 ]

MFVec2f

A multiple-value field that contains any number of two-dimensional vectors.
MFVec2fs are written to file as one or more pairs of floating point values
separated by whitespace. When more than one value is present, all of the values
are enclosed in square brackets and separated by commas; for example:

[ 0 0, 1.2 3.4, 98.6 -4e1 ]

MFVec3f

A multiple-value field that contains any number of three-dimensional vectors.
MFVec3fs are written to file as one or more triples of floating point values
separated by whitespace. When more than one value is present, all of the values
are enclosed in square brackets and separated by commas; for example:

[ 0 0 0, 1.2 3.4 5.6, 98.6 -4e1 212 ]

   Nodes

VRML defines several different classes of nodes. Most of the nodes can be
classified into one of three categories; shape, property or group. Shape nodes
define the geometry in the scene. Conceptually, they are the only nodes that
draw anything. Property nodes affect the way shapes are drawn. And grouping
nodes gather other nodes together, allowing collections of nodes to be treated
as a single object. Some group nodes also control whether or not their children
are drawn.

Nodes may contain zero or more fields. Each node type defines the type, name,
and default value for each of its fields. The default value for the field is
used if a value for the field is not specified in the VRML file. The order in
which the fields of a node are read is not important; for example, "Cube {
width 2 height 4 depth 6 }" and "Cube { height 4 depth 6 width 2 }" are
equivalent.

Here are the 36 nodes grouped by type. The first group are the shape nodes.
These specify geometry:

AsciiText, Cone, Cube, Cylinder, IndexedFaceSet, IndexedLineSet, PointSet,
Sphere,

The second group are the properties. These can be further grouped into
properties of the geometry and its appearance, matrix or transform properties,
and cameras and lights: Coordinate3, FontStyle, Info, LOD, Material,
MaterialBinding, Normal, NormalBinding, Texture2, Texture2Transform,
TextureCoordinate2, ShapeHints

MatrixTransform, Rotation, Scale, Transform, Translation

OrthographicCamera, PerspectiveCamera

DirectionalLight, PointLight, SpotLight

These are the group nodes:

Group, Separator, Switch, TransformSeparator, WWWAnchor

Finally, the WWWInline node does not fit neatly into any category.

WWWInline

AsciiText

This node represents strings of text characters from the ASCII coded character
set. The first string is rendered with its baseline at (0,0,0). All subsequent
strings advance y by -(size * spacing). See FontStyle for a description of the
size field. The justification field determines the placement of the strings in
the x dimension. LEFT (the default) places the left edge of each string at x=0.
CENTER places the center of each string at x=0. RIGHT places the right edge of
each string at x=0. Text is rendered from left to right, top to bottom in the
font set by FontStyle. The width field defines a suggested width constraint for
each string. The default is to use the natural width of each string. Setting
any value to 0 indicates the natural width should be used for that string.

The text is transformed by the current cumulative transformation and is drawn
with the current material and texture.

Textures are applied to 3D text as follows. The texture origin is at the origin
of the first string, as determined by the justification. The texture is scaled
equally in both S and T dimensions, with the font height representing 1 unit. S
increases to the right. The T origin can occur anywhere along each character,
depending on how that character's outline is defined.

JUSTIFICATION
     LEFT     Align left edge of text to origin
     CENTER   Align center of text to origin
     RIGHT    Align right edge of text to origin

FILE FORMAT/DEFAULTS
     AsciiText {
          string         ""    # MFString
          spacing        1     # SFFloat
          justification  LEFT  # SFEnum
          width          0     # MFFloat
     }

Cone

This node represents a simple cone whose central axis is aligned with the
y-axis. By default, the cone is centered at (0,0,0) and has a size of -1 to +1
in all three directions. The cone has a radius of 1 at the bottom and a height
of 2, with its apex at 1 and its bottom at -1. The cone has two parts: the
sides and the bottom.

The cone is transformed by the current cumulative transformation and is drawn
with the current texture and material.

If the current material binding is PER_PART or PER_PART_INDEXED, the first
current material is used for the sides of the cone, and the second is used for
the bottom. Otherwise, the first material is used for the entire cone.

When a texture is applied to a cone, it is applied differently to the sides and
bottom. On the sides, the texture wraps counterclockwise (from above) starting
at the back of the cone. The texture has a vertical seam at the back,
intersecting the yz-plane. For the bottom, a circle is cut out of the texture
square and applied to the cone's base circle. The texture appears right side up
when the top of the cone is rotated towards the -Z axis.

PARTS
     SIDES       The conical part
     BOTTOM      The bottom circular face
     ALL         All parts

FILE FORMAT/DEFAULTS
     Cone {
          parts         ALL     # SFBitMask
          bottomRadius  1       # SFFloat
          height        2       # SFFloat
     }

Coordinate3

This node defines a set of 3D coordinates to be used by a subsequent
IndexedFaceSet, IndexedLineSet, or PointSet node. This node does not produce a
visible result during rendering; it simply replaces the current coordinates in
the rendering state for subsequent nodes to use.

FILE FORMAT/DEFAULTS
     Coordinate3 {
          point  0 0 0  # MFVec3f
     }

Cube

This node represents a cuboid aligned with the coordinate axes. By default, the
cube is centered at (0,0,0) and measures 2 units in each dimension, from -1 to
+1. The cube is transformed by the current cumulative transformation and is
drawn with the current material and texture.

If the current material binding is PER_PART, PER_PART_INDEXED, PER_FACE, or
PER_FACE_INDEXED, materials will be bound to the faces of the cube in this
order: front (+Z), back (-Z), left (-X), right (+X), top (+Y), and bottom (-Y).

Textures are applied individually to each face of the cube; the entire texture
goes on each face. On the front, back, right, and left sides of the cube, the
texture is applied right side up. On the top, the texture appears right side up
when the top of the cube is tilted toward the camera. On the bottom, the
texture appears right side up when the top of the cube is tilted towards the -Z
axis.

FILE FORMAT/DEFAULTS
     Cube {
          width   2     # SFFloat
          height  2     # SFFloat
          depth   2     # SFFloat
     }

Cylinder

This node represents a simple capped cylinder centered around the y-axis. By
default, the cylinder is centered at (0,0,0) and has a default size of -1 to +1
in all three dimensions. The cylinder has three parts: the sides, the top (y =
+1) and the bottom (y = -1). You can use the radius and height fields to create
a cylinder with a different size.

The cylinder is transformed by the current cumulative transformation and is
drawn with the current material and texture.

If the current material binding is PER_PART or PER_PART_INDEXED, the first
current material is used for the sides of the cylinder, the second is used for
the top, and the third is used for the bottom. Otherwise, the first material is
used for the entire cylinder.

When a texture is applied to a cylinder, it is applied differently to the
sides, top, and bottom. On the sides, the texture wraps counterclockwise (from
above) starting at the back of the cylinder. The texture has a vertical seam at
the back, intersecting the yz-plane. For the top and bottom, a circle is cut
out of the texture square and applied to the top or bottom circle. The top
texture appears right side up when the top of the cylinder is tilted toward the
+Z axis, and the bottom texture appears right side up when the top of the
cylinder is tilted toward the -Z axis.

PARTS
     SIDES   The cylindrical part
     TOP     The top circular face
     BOTTOM  The bottom circular face
     ALL     All parts

FILE FORMAT/DEFAULTS
     Cylinder {
          parts   ALL   # SFBitMask
          radius  1     # SFFloat
          height  2     # SFFloat
     }

DirectionalLight

This node defines a directional light source that illuminates along rays
parallel to a given 3-dimensional vector.

A light node defines an illumination source that may affect subsequent shapes
in the scene graph, depending on the current lighting style. Light sources are
affected by the current transformation. A light node under a separator does not
affect any objects outside that separator.

FILE FORMAT/DEFAULTS
     DirectionalLight {
          on         TRUE       # SFBool
          intensity  1          # SFFloat
          color      1 1 1      # SFColor
          direction  0 0 -1     # SFVec3f
     }

FontStyle

This node defines the current font style used for all subsequent AsciiText.
Font attributes only are defined. It is up to the browser to assign specific
fonts to the various attribute combinations. The size field specifies the
height (in object space units) of glyphs rendered and determines the vertical
spacing of adjacent lines of text.

FAMILY

     SERIF       Serif style (such as TimesRoman)
     SANS        Sans Serif Style (such as Helvetica)
     TYPEWRITER  Fixed pitch style (such as Courier)

STYLE
     NONE        No modifications to family
     BOLD        Embolden family
     ITALIC      Italicize or Slant family

FILE FORMAT/DEFAULTS
     FontStyle {
          size     10      # SFFloat
          family   SERIF   # SFEnum
          style    NONE    # SFBitMask
     }

Group

This node defines the base class for all group nodes. Group is a node that
contains an ordered list of child nodes. This node is simply a container for
the child nodes and does not alter the traversal state in any way. During
traversal, state accumulated for a child is passed on to each successive child
and then to the parents of the group (Group does not push or pop traversal
state as separator does).

FILE FORMAT/DEFAULTS
     Group {
     }

IndexedFaceSet

This node represents a 3D shape formed by constructing faces (polygons) from
vertices located at the current coordinates. IndexedFaceSet uses the indices in
its coordIndex field to specify the polygonal faces. An index of -1 indicates
that the current face has ended and the next one begins.

The vertices of the faces are transformed by the current transformation matrix.

Treatment of the current material and normal binding is as follows: The
PER_PART and PER_FACE bindings specify a material or normal for each face.
PER_VERTEX specifies a material or normal for each vertex. The corresponding
_INDEXED bindings are the same, but use the materialIndex or normalIndex
indices. The DEFAULT material binding is equal to OVERALL. The DEFAULT normal
binding is equal to PER_VERTEX_INDEXED; if insufficient normals exist in the
state, vertex normals will be generated automatically.

Explicit texture coordinates (as defined by TextureCoordinate2) may be bound to
vertices of an indexed shape by using the indices in the textureCoordIndex
field. As with all vertex-based shapes, if there is a current texture but no
texture coordinates are specified, a default texture coordinate mapping is
calculated using the bounding box of the shape. The longest dimension of the
bounding box defines the S coordinates, and the next longest defines the T
coordinates. The value of the S coordinate ranges from 0 to 1, from one end of
the bounding box to the other. The T coordinate ranges between 0 and the ratio
of the second greatest dimension of the bounding box to the greatest dimension.

Be sure that the indices contained in the coordIndex, materialIndex,
normalIndex, and textureCoordIndex fields are valid with respect to the current
state, or errors will occur.

FILE FORMAT/DEFAULTS
     IndexedFaceSet {
          coordIndex         0  # MFLong
          materialIndex      -1 # MFLong
          normalIndex        -1 # MFLong
          textureCoordIndex  -1 # MFLong
     }

IndexedLineSet

This node represents a 3D shape formed by constructing polylines from vertices
located at the current coordinates. IndexedLineSet uses the indices in its
coordIndex field to specify the polylines. An index of -1 indicates that the
current polyline has ended and the next one begins.

The coordinates of the line set are transformed by the current cumulative
transformation.

Treatment of the current material and normal binding is as follows: The
PER_PART binding specifies a material or normal for each segment of the line.
The PER_FACE binding specifies a material or normal for each polyline.
PER_VERTEX specifies a material or normal for each vertex. The corresponding
_INDEXED bindings are the same, but use the materialIndex or normalIndex
indices. The DEFAULT material binding is equal to OVERALL. The DEFAULT normal
binding is equal to PER_VERTEX_INDEXED; if insufficient normals exist in the
state, the lines will be drawn unlit. The same rules for texture coordinate
generation as IndexedFaceSet are used.

FILE FORMAT/DEFAULTS
     IndexedLineSet {
          coordIndex         0  # MFLong
          materialIndex      -1 # MFLong
          normalIndex        -1 # MFLong
          textureCoordIndex  -1 # MFLong
     }

Info

This class defines an information node in the scene graph. This node has no
effect during traversal. It is used to store information in the scene graph,
typically for application-specific purposes, copyright messages, or other
strings.

     Info {
          string  "<Undefined info>"      # SFString
     }

LOD

This group node is used to allow applications to switch between various
representations of objects automatically. The children of this node typically
represent the same object or objects at varying levels of detail, from highest
detail to lowest.

The specified center point of the LOD is transformed by the current
transformation into world space, and the distance from the transformed center
to the world-space eye point is calculated. If the distance is less than the
first value in the ranges array, then the first child of the LOD group is
drawn. If between the first and second values in the ranges array, the second
child is drawn, etc. If there are N values in the ranges array, the LOD group
should have N+1 children. Specifying too few children will result in the last
child being used repeatedly for the lowest levels of detail; if too many
children are specified, the extra children will be ignored. Each value in the
ranges array should be less than the previous value, otherwise results are
undefined.

FILE FORMAT/DEFAULTS
     LOD {
          range [ ]    # MFFloat
          center 0 0 0  # SFVec3f
     }

Material

This node defines the current surface material properties for all subsequent
shapes. Material sets several components of the current material during
traversal. Different shapes interpret materials with multiple values
differently. To bind materials to shapes, use a MaterialBinding node.

FILE FORMAT/DEFAULTS
     Material {
          ambientColor   0.2 0.2 0.2    # MFColor
          diffuseColor   0.8 0.8 0.8    # MFColor
          specularColor  0 0 0          # MFColor
          emissiveColor  0 0 0          # MFColor
          shininess      0.2            # MFFloat
          transparency   0              # MFFloat
     }

MaterialBinding

This node specifies how the current materials are bound to shapes that follow
in the scene graph. Each shape node may interpret bindings differently. The
current material always has a base value, which is defined by the first value
of all material fields. Since material fields may have multiple values, the
binding determines how these values are distributed over a shape.

The bindings for faces and vertices are meaningful only for shapes that are
made from faces and vertices. Similarly, the indexed bindings are only used by
the shapes that allow indexing.

When multiple material values are bound, the values are cycled through, based
on the period of the material component with the most values. For example, the
following table shows the values used when cycling through (or indexing into) a
material with 2 ambient colors, 3 diffuse colors, and 1 of all other components
in the current material. (The period of this material cycle is 3):

Material        Ambient color   Diffuse color   Other
 0                     0               0           0
 1                     1               1           0
 2                     1               2           0
 3 (same as 0)         0               0           0

BINDINGS
     DEFAULT            Use default binding
     OVERALL            Whole object has same material
     PER_PART           One material for each part of object
     PER_PART_INDEXED   One material for each part, indexed
     PER_FACE           One material for each face of object
     PER_FACE_INDEXED   One material for each face, indexed
     PER_VERTEX         One material for each vertex of object
     PER_VERTEX_INDEXED One material for each vertex, indexed

FILE FORMAT/DEFAULTS
     MaterialBinding {
          value  DEFAULT        # SFEnum
     }

MatrixTransform

This node defines a geometric 3D transformation with a 4 by 4 matrix. Note that
some matrices (such as singular ones) may result in errors.

FILE FORMAT/DEFAULTS
     MatrixTransform {
          matrix  1 0 0 0       # SFMatrix
                  0 1 0 0
                  0 0 1 0
                  0 0 0 1
     }

Normal

This node defines a set of 3D surface normal vectors to be used by vertex-based
shape nodes (IndexedFaceSet, IndexedLineSet, PointSet) that follow it in the
scene graph. This node does not produce a visible result during rendering; it
simply replaces the current normals in the rendering state for subsequent nodes
to use. This node contains one multiple-valued field that contains the normal
vectors.

FILE FORMAT/DEFAULTS
     Normal {
          vector  0 0 1 # MFVec3f
     }

NormalBinding

This node specifies how the current normals are bound to shapes that follow in
the scene graph. Each shape node may interpret bindings differently.

The bindings for faces and vertices are meaningful only for shapes that are
made from faces and vertices. Similarly, the indexed bindings are only used by
the shapes that allow indexing. For bindings that require multiple normals, be
sure to have at least as many normals defined as are necessary; otherwise,
errors will occur.

BINDINGS
     DEFAULT            Use default binding
     OVERALL            Whole object has same normal
     PER_PART           One normal for each part of object
     PER_PART_INDEXED   One normal for each part, indexed
     PER_FACE           One normal for each face of object
     PER_FACE_INDEXED   One normal for each face, indexed
     PER_VERTEX         One normal for each vertex of object
     PER_VERTEX_INDEXED One normal for each vertex, indexed

FILE FORMAT/DEFAULTS
     NormalBinding {
          value  DEFAULT        # SFEnum
     }

OrthographicCamera

An orthographic camera defines a parallel projection from a viewpoint. This
camera does not diminish objects with distance, as an PerspectiveCamera does.
The viewing volume for an orthographic camera is a rectangular parallelepiped
(a box).

By default, the camera is located at (0,0,1) and looks along the negative
z-axis; the position and orientation fields can be used to change these values.
The height field defines the total height of the viewing volume.

A camera can be placed in a VRML world to specify the initial location of the
viewer when that world is entered. VRML browsers will typically modify the
camera to allow a user to move through the virtual world.

Cameras are affected by the current transformation, so you can position a
camera by placing a transformation node before it in the scene graph . The
default position and orientation of a camera is at (0,0,1) looking along the
negative z-axis.

FILE FORMAT/DEFAULTS
     OrthographicCamera {
          position         0 0 1        # SFVec3f
          orientation      0 0 1  0     # SFRotation
          focalDistance    5            # SFFloat
          height           2            # SFFloat
     }

PerspectiveCamera

A perspective camera defines a perspective projection from a viewpoint. The
viewing volume for a perspective camera is a truncated right pyramid.

By default, the camera is located at (0,0,1) and looks along the negative
z-axis; the position and orientation fields can be used to change these values.
The heightAngle field defines the total vertical angle of the viewing volume.

See more on cameras in the OrthographicCamera description.

FILE FORMAT/DEFAULTS
     PerspectiveCamera {
          position         0 0 1        # SFVec3f
          orientation      0 0 1  0     # SFRotation
          focalDistance    5            # SFFloat
          heightAngle      0.785398     # SFFloat
     }

PointLight

This node defines a point light source at a fixed 3D location. A point source
illuminates equally in all directions; that is, it is omni- directional.

A light node defines an illumination source that may affect subsequent shapes
in the scene graph, depending on the current lighting style. Light sources are
affected by the current transformation. A light node under a separator does not
affect any objects outside that separator.

FILE FORMAT/DEFAULTS
     PointLight {
          on         TRUE       # SFBool
          intensity  1          # SFFloat
          color      1 1 1      # SFColor
          location   0 0 1      # SFVec3f
     }

PointSet

This node represents a set of points located at the current coordinates.
PointSet uses the current coordinates in order, starting at the index specified
by the startIndex field. The number of points in the set is specified by the
numPoints field. A value of -1 for this field indicates that all remaining
values in the current coordinates are to be used as points.

The coordinates of the point set are transformed by the current cumulative
transformation. The points are drawn with the current material and texture.

Treatment of the current material and normal binding is as follows: PER_PART,
PER_FACE, and PER_VERTEX bindings bind one material or normal to each point.
The DEFAULT material binding is equal to OVERALL. The DEFAULT normal binding is
equal to PER_VERTEX. The startIndex is also used for materials or normals when
the binding indicates that they should be used per vertex.

FILE FORMAT/DEFAULTS
     PointSet {
          startIndex  0 # SFLong
          numPoints   -1        # SFLong
     }

Rotation

This node defines a 3D rotation about an arbitrary axis through the origin. The
rotation is accumulated into the current transformation, which is applied to
subsequent shapes.

FILE FORMAT/DEFAULTS
     Rotation {
          rotation  0 0 1  0    # SFRotation
     }

See rotation field description for more information.

Scale

This node defines a 3D scaling about the origin. If the components of the
scaling vector are not all the same, this produces a non-uniform scale.

FILE FORMAT/DEFAULTS
     Scale {
          scaleFactor  1 1 1    # SFVec3f
     }

Separator

This group node performs a push (save) of the traversal state before traversing
its children and a pop (restore) after traversing them. This isolates the
separator's children from the rest of the scene graph. A separator can include
lights, cameras, coordinates, normals, bindings, and all other properties.

Separators can also perform render culling. Render culling skips over traversal
of the separator's children if they are not going to be rendered, based on the
comparison of the separator's bounding box with the current view volume.
Culling is controlled by the renderCulling field. These are set to AUTO by
default, allowing the implementation to decide whether or not to cull.

CULLING ENUMS
     ON    Always try to cull to the view volume
     OFF   Never try to cull to the view volume
     AUTO  Implementation-defined culling behavior

FILE FORMAT/DEFAULTS
     Separator {
          renderCulling       AUTO      # SFEnum
     }

ShapeHints

The ShapeHints node indicates that IndexedFaceSets are solid, contain ordered
vertices, or contain convex faces.

These hints allow VRML implementations to optimize certain rendering features.
Optimizations that may be performed include enabling back-face culling and
disabling two-sided lighting. For example, if an object is solid and has
ordered vertices, an implementation may turn on backface culling and turn off
two-sided lighting. If the object is not solid but has ordered vertices, it may
turn off backface culling and turn on two-sided lighting.

The ShapeHints node also affects how default normals are generated. When an
IndexedFaceSet has to generate default normals, it uses the creaseAngle field
to determine which edges should be smoothly shaded and which ones should have a
sharp crease. The crease angle is the angle between surface normals on adjacent
polygons. For example, a crease angle of .5 radians (the default value) means
that an edge between two adjacent polygonal faces will be smooth shaded if the
normals to the two faces form an angle that is less than .5 radians (about 30
degrees). Otherwise, it will be faceted.

VERTEX ORDERING ENUMS
     UNKNOWN_ORDERING    Ordering of vertices is unknown
     CLOCKWISE           Face vertices are ordered clockwise
                          (from the outside)
     COUNTERCLOCKWISE    Face vertices are ordered counterclockwise
                          (from the outside)

SHAPE TYPE ENUMS
     UNKNOWN_SHAPE_TYPE  Nothing is known about the shape
     SOLID               The shape encloses a volume

FACE TYPE ENUMS
     UNKNOWN_FACE_TYPE   Nothing is known about faces
     CONVEX              All faces are convex

FILE FORMAT/DEFAULTS
     ShapeHints {
          vertexOrdering  UNKNOWN_ORDERING      # SFEnum
          shapeType       UNKNOWN_SHAPE_TYPE    # SFEnum
          faceType        CONVEX                # SFEnum
          creaseAngle     0.5                   # SFFloat
     }

Sphere

This node represents a sphere. By default, the sphere is centered at the origin
and has a radius of 1. The sphere is transformed by the current cumulative
transformation and is drawn with the current material and texture.

A sphere does not have faces or parts. Therefore, the sphere ignores material
and normal bindings, using the first material for the entire sphere and using
its own normals. When a texture is applied to a sphere, the texture covers the
entire surface, wrapping counterclockwise from the back of the sphere. The
texture has a seam at the back on the yz-plane.

FILE FORMAT/DEFAULTS
     Sphere {
          radius  1     # SFFloat
     }

SpotLight

This node defines a spotlight light source. A spotlight is placed at a fixed
location in 3-space and illuminates in a cone along a particular direction. The
intensity of the illumination drops off exponentially as a ray of light
diverges from this direction toward the edges of the cone. The rate of drop-off
and the angle of the cone are controlled by the dropOffRate and cutOffAngle
fields.

A light node defines an illumination source that may affect subsequent shapes
in the scene graph, depending on the current lighting style. Light sources are
affected by the current transformation. A light node under a separator does not
affect any objects outside that separator.

FILE FORMAT/DEFAULTS
     SpotLight {
          on           TRUE     # SFBool
          intensity    1        # SFFloat
          color        1 1 1    # SFVec3f
          location     0 0 1    # SFVec3f
          direction    0 0 -1   # SFVec3f
          dropOffRate  0        # SFFloat
          cutOffAngle  0.785398 # SFFloat
     }

Switch

This group node traverses one, none, or all of its children. One can use this
node to switch on and off the effects of some properties or to switch between
different properties.

The whichChild field specifies the index of the child to traverse, where the
first child has index 0.

A value of -1 (the default) means do not traverse any children. A value of -3
traverses all children, making the switch behave exactly like a regular Group.

FILE FORMAT/DEFAULTS
     Switch {
          whichChild  -1        # SFLong
     }

Texture2

This property node defines a texture map and parameters for that map. This map
is used to apply texture to subsequent shapes as they are rendered.

The texture can be read from the URL specified by the filename field. To turn
off texturing, set the filename field to an empty string ("").

Textures can also be specified inline by setting the image field to contain the
texture data. Specifying both a URL and data inline will result in undefined
behavior.

WRAP ENUM
     REPEAT  Repeats texture outside 0-1 texture coordinate range
     CLAMP   Clamps texture coordinates to lie within 0-1 range

FILE FORMAT/DEFAULTS
     Texture2 {
          filename    ""        # SFString
          image       0 0 0     # SFImage
          wrapS       REPEAT    # SFEnum
          wrapT       REPEAT    # SFEnum
     }

Texture2Transform

This node defines a 2D transformation applied to texture coordinates. This
affects the way textures are applied to the surfaces of subsequent shapes. The
transformation consists of (in order) a non-uniform scale about an arbitrary
center point, a rotation about that same point, and a translation. This allows
a user to change the size and position of the textures on shapes.

FILE FORMAT/DEFAULTS
     Texture2Transform {
          translation  0 0      # SFVec2f
          rotation     0        # SFFloat
          scaleFactor  1 1      # SFVec2f
          center       0 0      # SFVec2f
     }

TextureCoordinate2

This node defines a set of 2D coordinates to be used to map textures to the
vertices of subsequent PointSet, IndexedLineSet, or IndexedFaceSet objects. It
replaces the current texture coordinates in the rendering state for the shapes
to use.

Texture coordinates range from 0 to 1 across the texture. The horizontal
coordinate, called S, is specified first, followed by the vertical coordinate,
T.

FILE FORMAT/DEFAULTS
     TextureCoordinate2 {
          point  0 0    # MFVec2f
     }

Transform

This node defines a geometric 3D transformation consisting of (in order) a
(possibly) non-uniform scale about an arbitrary point, a rotation about an
arbitrary point and axis, and a translation.

FILE FORMAT/DEFAULTS
     Transform {
          translation       0 0 0       # SFVec3f
          rotation          0 0 1  0    # SFRotation
          scaleFactor       1 1 1       # SFVec3f
          scaleOrientation  0 0 1  0    # SFRotation
          center            0 0 0       # SFVec3f
     }

The transform node

Transform {
    translation T1
    rotation R1
    scaleFactor S
    scaleOrientation R2
    center T2
  }

is equivalent to the sequence

Translation { translation T1 }
Translation { translation T2 }
Rotation { rotation R1 }
Rotation { rotation R2 }
Scale { scaleFactor S }
Rotation { rotation -R2 }
Translation { translation -T2 }

TransformSeparator

This group node is similar to the separator node in that it saves state before
traversing its children and restores it afterwards. However, it saves only the
current transformation; all other state is left as is. This node can be useful
for positioning a camera, since the transformations to the camera will not
affect the rest of the scene, even through the camera will view the scene.
Similarly, this node can be used to isolate transformations to light sources or
other objects.

FILE FORMAT/DEFAULTS
     TransformSeparator {
     }

Translation

This node defines a translation by a 3D vector.

FILE FORMAT/DEFAULTS
     Translation {
          translation  0 0 0    # SFVec3f
     }

WWWAnchor

The WWWAnchor group node loads a new scene into a VRML browser when one of its
children is chosen. Exactly how a user "chooses" a child of the WWWAnchor is up
to the VRML browser; typically, clicking on one of its children with the mouse
will result in the new scene replacing the current scene. A WWWAnchor with an
empty ("") name does nothing when its children are chosen. The name is an
arbitrary URL.

WWWAnchor behaves like a Separator, pushing the traversal state before
traversing its children and popping it afterwards.

The description field in the WWWAnchor allows for a friendly prompt to be
displayed as an alternative to the URL in the name field. Ideally, browsers
will allow the user to choose the description, the URL or both to be displayed
for a candidate WWWAnchor.

The WWWAnchor's map field is an enumerated value that can be either NONE (the
default) or POINT. If it is POINT then the object-space coordinates of the
point on the object the user chose will be added to the URL in the name field,
with the syntax "?x,y,z".

MAP ENUM
     NONE  Do not add information to the URL
     POINT Add object-space coordinates to URL

FILE FORMAT/DEFAULTS
     WWWAnchor {
          name ""        # SFString
          description "" # SFString
          map NONE       # SFEnum
     }

WWWInline

The WWWInline node reads its children from anywhere in the World Wide Web.
Exactly when its children are read is not defined; reading the children may be
delayed until the WWWInline is actually displayed. A WWWInline with an empty
name does nothing. The name is an arbitrary URL.

The effect of referring to a non-VRML URL in a WWWInline node is undefined.

If the WWWInline's bboxSize field specifies a non-empty bounding box (a
bounding box is non-empty if at least one of its dimensions is greater than
zero), then the WWWInline's object-space bounding box is specified by its
bboxSize and bboxCenter fields. This allows an implementation to view-volume
cull or LOD switch the WWWInline without reading its contents.

FILE FORMAT/DEFAULTS
     WWWInline {
          name ""               # SFString
          bboxSize 0 0 0        # SFVec3f
          bboxCenter 0 0 0      # SFVec3f
     }

   Instancing

A node may be the child of more than one group. This is called "instancing"
(using the same instance of a node multiple times, called "aliasing" or
"multiple references" by other systems), and is accomplished by using the "USE"
keyword.

The DEF keyword both defines a named node, and creates a single instance of it.
The USE keyword indicates that the most recently defined instance should be
used again. If several nodes were given the same name, then the last DEF
encountered during parsing "wins". DEF/USE is limited to a single file; there
is no mechanism for USE'ing nodes that are DEF'ed in other files.

A name goes into scope as soon as the DEF is encountered, and does not go out
of scope until another DEF of the same name or end-of-file are encountered.
Nodes cannot be shared between files (you cannot USE a node that was DEF'ed
inside the file to which a WWWInline refers).

For example, rendering this scene will result in three spheres being drawn.
Both of the spheres are named 'Joe'; the second (smaller) sphere is drawn
twice:

Separator {
        DEF Joe Sphere { }
        Translation { translation 2 0 0 }
        Separator {
                DEF Joe Sphere { radius .2 }
                Translation { translation 2 0 0 }
        }
        USE Joe    # radius .2 sphere will be used here


   Extensibility

Extensions to VRML are supported by supporting self-describing nodes. Nodes
that are not part of standard VRML must write out a description of their fields
first, so that all VRML implementations are able to parse and ignore the
extensions.

This description is written just after the opening curly-brace for the node,
and consists of the keyword 'fields' followed by a list of the types and names
of fields used by that node, all enclosed in square brackets and separated by
commas. For example, if Cube was not a standard VRML node, it would be written
like this:

Cube {
  fields [ SFFloat width, SFFloat height, SFFloat depth ]
  width 10 height 4 depth 3


Specifying the fields for nodes that ARE part of standard VRML is not an error;
VRML parsers must silently ignore the field specification.

Is-a relationships

A new node type may also be a superset of an existing node that is part of the
standard. In this case, if an implementation for the new node type cannot be
found the new node type can be safely treated as as the existing node it is
based on (with some loss of functionality, of course). To support this, new
node types can define an MFString field called 'isA' containing the names of
the types of which it is a superset. For example, a new type of Material called
"ExtendedMaterial" that adds index of refraction as a material property can be
written as:

ExtendedMaterial {
  fields [ MFString isA, MFFloat indexOfRefraction,
           MFColor ambientColor, MFColor diffuseColor,
           MFColor specularColor, MFColor emissiveColor,
           MFFloat shininess, MFFloat transparency ]
  isA [ "Material" ]
  indexOfRefraction .34
  diffuseColor .8 .54 1


Multiple is-a relationships may be specified in order of preference;
implementations are expected to use the first for which there is an
implementation.

   An Example

This is a longer example of a VRML scene. It contains a simple model of a
track-light consisting of primitive shapes, plus three walls (built out of
polygons) and a reference to a shape defined elsewhere, both of which are
illuminated by a spotlight. The shape acts as a hyperlink to some HTML text.

#VRML V1.0 ascii

Separator {
    Separator {       # Simple track-light geometry:
        Translation { translation 0 4 0 }
        Separator {
            Material { emissiveColor 0.1 0.3 0.3 }
            Cube {
                width   0.1
                height  0.1
                depth   4
            }
        }
        Rotation { rotation 0 1 0  1.57079 }
        Separator {
            Material { emissiveColor 0.3 0.1 0.3 }
            Cylinder {
                radius  0.1
                height  .2
            }
        }
        Rotation { rotation -1 0 0  1.57079 }
        Separator {
            Material { emissiveColor 0.3 0.3 0.1 }
            Rotation { rotation 1 0 0  1.57079 }
            Translation { translation 0 -.2 0 }
            Cone {
                height  .4
                bottomRadius .2
            }
            Translation { translation 0 .4 0 }
            Cylinder {
                radius  0.02
                height  .4
            }
        }
    }
    SpotLight {      # Light from above
        location 0 4 0
        direction 0 -1 0
        intensity       0.9
        cutOffAngle     0.7
    }
    Separator {      # Wall geometry; just three flat polygons
        Coordinate3 {
            point [
                   -2 0 -2, -2 0 2, 2 0 2, 2 0 -2,
                   -2 4 -2, -2 4 2, 2 4 2, 2 4 -2]
        }
        IndexedFaceSet {
            coordIndex [ 0, 1, 2, 3, -1,
                        0, 4, 5, 1, -1,
                        0, 3, 7, 4, -1
                        ]
        }
    }
    WWWAnchor {   # A hyperlinked cow:
        name "http://www.foo.edu/CowProject/AboutCows.html"

        Separator {
            Translation { translation 0 1 0 }
            WWWInline {   # Reference another object
                name "http://www.foo.edu/3DObjects/cow.wrl"
            }
        }
    }


-------------------------------------------------------------------------------

   Browser Considerations

This section describes the file naming and MIME conventions to be used in
building VRML browsers and configuring WWW browsers to work with them.

   File Extensions

The file extension for VMRL files is .wrl (for world).

   MIME

The MIME type for VRML files is defined as follows:

x-world/x-vrml

The MIME major type for 3D world descriptions is x-world. The MIME minor type
for VRML documents is x-vrml. Other 3D world descriptions, such as oogl for The
Geometry Center's Object-Oriented Geometry Language, or iv, for SGI's Open
Inventor ASCII format, can be supported by using different MIME minor types.

[--] 26-MAY-95

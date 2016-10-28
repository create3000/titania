[Titania](http://titania.create3000.de/) — Titania X3D Editor
==================================================

Introduction
--------------------------------------

Titania is a free, open source, specification conform X3D Graphics authoring tool and browser for Ubuntu. The goal is a full standard compliant X3D browser for future releases of Ubuntu. For now, Titania fully supports the Moving Worlds specification for VRML 2.0 and already most parts of the X3D specification.

It can be used as a simple 3D viewer for .x3dv/.x3d files, as high quality 3D animation rendering engine, for advanced science simulations, live data visualization, or as easy to use gaming engine.

Furthermore this package includes x3dtidy, an easy to use X3D/VRML beautifier and converter.

For more information and documentation please visit our website at [http://titania.create3000.de](http://titania.create3000.de).

Installation
--------------------------------------

### Ubuntu Package

Titania is available for **Ubuntu** on Launchpad via ppa. You can install Titania by pasting the following lines into a terminal: 

```bash
sudo add-apt-repository ppa:holger-seelig/titania
sudo apt-get update
sudo apt-get install titania
```

Since it is sometimes a bit tricky to compile titania successfully on other linux systems, there is a package available for AUR, the arch linux user
repository which should simplify the process for you: [https://aur.archlinux.org/packages/titania-git/](https://aur.archlinux.org/packages/titania-git/).


### AUR package

If you want to compile Titania from source, first make sure that all dependencies listed in `debian/control` are installed. Then
type the following commands in a terminal:


### Install from Source

```bash
git clone https://github.com/create3000/titania.git
cd titania
./autogen.sh
make -j4
make run
```

**Note:** `make -j4` compiles the project with four jobs at the same time, ie. it uses four processors. Adjust this value depending
on the number of processors of your computer.  It can take 20 up to 45 minutes or longer to compile the whole project.


Acknowledgments
--------------------------------------

Titania wouldn't be possible without parts of source code or inspiration from other software packages. We would like to say thank to everyone for their great work and help.

* [H3D](http://www.h3dapi.org/)
* [FreeWRL](http://freewrl.sourceforge.net/)
* [GeoVRML](http://www.ai.sri.com/geovrml/)
* [X3Dom](http://www.x3dom.org/)
* [X3D-Edit](https://savage.nps.edu/X3D-Edit/)
* [Coin 3D](https://bitbucket.org/Coin3D/coin/wiki/Home)
* [SGI Open Inventor](http://oss.sgi.com/projects/inventor/)
* [SGI Standard Template Library](https://www.sgi.com/tech/stl/)
* [GraphicsGems](http://tog.acm.org/resources/GraphicsGems/)
* [Perl Quaternion](http://search.cpan.org/~jchin/Math-Quaternion-0.02/lib/Math/Quaternion.pm)
* [Natural Sort](https://github.com/jjk-jacky/natsort)
* [Chromium Browser Project](https://www.chromium.org/)
* [NVidia](http://www.nvidia.de/page/home.html)
* [Mozilla Foundation](https://www.mozilla.org/de/)
* [Deepak Bandyopadhyay, Lutz Kettner](http://www.cs.unc.edu/Research/compgeom/gzstream/) for gzstream
* [kishan at hackorama dot com](http://www.hackorama.com) for AnyOptions
* Etienne Lachance <etienne.lachance@polymtl.ca> for quaternion.cpp
* Max Wagner <mwagner@digipen.edu> for his paper »Generating Vertex Normals«
* Johnny Huynh for his paper »Separating Axis Theorem for Oriented Bounding Boxes«
* Erik B. Dam <erikdam@diku.dk>, Martin Koch <myth@diku.dk> and Martin Lillholm <grumse@diku.dk> for their paper »Quaternions, Interpolation and Animation«
* And very special thanks to [Silvio Borchard](http://www.union3d.net/), who built most examples worlds that comes with Titania, and [Norbert Meissner](http://www.norbertmeissner.info/) for his support in the past.

And all others not listed here.

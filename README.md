[Titania](http://titania.create3000.de/) — Titania X3D Editor
==================================================

Introduction
--------------------------------------

Titania is a free, open source, specification conform X3D Graphics authoring tool and browser for Ubuntu. The goal is a 
full standard compliant X3D browser for future releases of Ubuntu. For now, Titania fully supports the Moving Worlds 
specification for VRML 2.0 and already most parts of the X3D specification.

It can be used as a simple 3D viewer for .x3dv/.x3d files, as high quality 3D animation rendering engine, for advanced 
science simulations, live data visualization, or as easy to use gaming engine.

Furthermore Titania can also be used as an easy to use X3D/VRML beautifier and converter.

For more information and documentation please visit our website at [http://titania.create3000.de](http://titania.create3000.de).

Installation
--------------------------------------

### Install Titania via FlatPak

[FlatPak](http://flatpak.org/) is a new straightforward packaging format for Linux, which allows the same applications
to run in a sandboxed environment, across any modern distribution.

We are offering a stable release of Titania as a FlatPak bundle, which runs out of the box.

If not already included within your distribution, first, [install FlatPak for your distribution](http://flatpak.org/getting.html) – make
sure you have version 0.8.0 or higher.

Then [click here to install](http://code.create3000.de/repos/titania.flatpakref), or paste the following line into a terminal to install Titania:

```bash
flatpak install --user --from http://code.create3000.de/repos/titania.flatpakref
```

or update your existing FlatPak installation with:

```bash
flatpak update --user de.create3000.titania
```

Don't forget to reboot your system after installation has been completed. You can than run newly installed Titania from terminal using:

```bash
flatpak run de.create3000.titania
```

#### Known Issues

If Titania does not run and quits with the error message »locale::facet::_S_create_c_locale name not valid« run:

```bash
flatpak install --user gnome org.gnome.Platform.Locale//3.26
```

### Arch Linux User Repository

Since it is sometimes a bit tricky to compile titania successfully on other Linux systems than Ubuntu, there is a
package of the latest version of Titania available for AUR, the Arch Linux User Repository which should simplify the
process for you:

[https://aur.archlinux.org/packages/titania-git/](https://aur.archlinux.org/packages/titania-git/)

They are specific for the [Arch Linux](https://www.archlinux.org/) distribution. AUR Packages are not precompiled, they
are more or less simply a script that downloads the source code, installs all dependencies and executes the commands to
compile and install the software.

More information how to install an AUR package can be found at:

[https://wiki.archlinux.org/index.php/Arch_User_Repository](https://wiki.archlinux.org/index.php/Arch_User_Repository)


### Install from Source

If you want to compile Titania from source, first make sure that all dependencies listed in `debian/control` are
installed. Then type the following commands in a terminal:

```bash
git clone https://github.com/create3000/titania.git
cd titania
./autogen.sh
make -j4
make run
```

**Note:** `make -j4` compiles the project with four jobs at the same time, ie. it uses four processors. Adjust this 
value depending on the number of processors of your computer.  It can take 20 up to 45 minutes or longer to compile the
whole project.


Build Software
--------------------------------------
Titania is build with [MonoDevelop](http://www.monodevelop.com/), [Glade](https://glade.gnome.org/) and Ubuntu.


Authors
--------------------------------------
All authors contributed to this project can be found in the [list of authors](AUTHORS.md).


Acknowledgments
--------------------------------------

Titania wouldn't be possible without parts of source code or inspiration from other software packages. We would like to 
say thank to everyone for their great work and help.

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

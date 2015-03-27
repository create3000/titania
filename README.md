[Titania](http://titania.create3000.de/) — Titania X3D Editor
==================================================

Introduction
--------------------------------------

Titania is a X3D/VRML editor and browser for Ubuntu. The goal is a full standard compliant X3D browser for future releases of Ubuntu. For now, Titania fully supports the Moving Worlds specification for VRML 2.0 and already many parts of the X3D specification.

It can be used as a simple 3D viewer for .wrl files, as high quality 3D animation rendering engine, for advanced science simulations, live data visualization, or as easy to use gaming engine.

Furthermore this package includes x3dtidy, an easy to use X3D/VRML beautifier.

For more information please visit [http://titania.create3000.de](http://titania.create3000.de).

Installation
--------------------------------------

```bash
cd Titania
./autogen.sh --enable-release --prefix=`/usr/local/
./configure
make -j4
make install
```

Acknowledgments
--------------------------------------

Titania wouldn't be possible without parts of program code or inspiration from other software packages. We would like to say thank to everyone for their great work and help.

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
* [Chromium Browser](https://www.chromium.org/)
* [NVidia](http://www.nvidia.de/page/home.html)
* [Mozilla Foundation](https://www.mozilla.org/de/)
* [Deepak Bandyopadhyay, Lutz Kettner](http://www.cs.unc.edu/Research/compgeom/gzstream/) for gzstream
* [kishan at hackorama dot com](www.hackorama.com) for AnyOptions
* Etienne Lachance <etienne.lachance@polymtl.ca> for quaternion.cpp
* Max Wagner <mwagner@digipen.edu> for his paper »Generating Vertex Normals«
* Johnny Huynh for his paper »Separating Axis Theorem for Oriented Bounding Boxes«
* Erik B. Dam <erikdam@diku.dk>, Martin Koch <myth@diku.dk> and Martin Lillholm <grumse@diku.dk> for their paper »Quaternions, Interpolation and Animation«
* [Silvio Borchard](http://www.union3d.net/), who built most examples worlds that comes with Titania

And all others not listed here.

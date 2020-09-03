
SNAPCRAFT=snapcraft

all: clean version-number compile

clean:
	${SNAPCRAFT} clean
	rm -r -f parts
	rm -r -f prime
	rm -r -f snap
	rm -r -f stage
	rm -f titania_*.snap

VERSION=`perl -wln -e 'print $$1 if /version\s*=\s*"(.*?)"/' ../libtitania-x3d/Titania/X3D/Browser/X3DBrowser.cpp`

version-number:
	echo ${VERSION}
	perl -pi -e 's/version:\s*[\d\.]+/version: '${VERSION}'/sg' snapcraft.yaml

compile:
	${SNAPCRAFT}

install: remove
	sudo snap install titania_*_amd64.snap --dangerous

run:
	env SNAP_DESKTOP_DEBUG=1 /snap/bin/titania

remove:
	sudo snap remove titania

publish:
	${SNAPCRAFT} login
	${SNAPCRAFT} push --release=stable titania_*.snap

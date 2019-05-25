
SNAPCRAFT=snapcraft

all: clean compile

clean:
	${SNAPCRAFT} clean
	rm -r -f parts
	rm -r -f prime
	rm -r -f snap
	rm -r -f stage
	rm -f titania_*.snap

compile:
	${SNAPCRAFT}

install: remove
	sudo snap install titania_*_amd64.snap --devmode --dangerous

run:
	env SNAP_DESKTOP_DEBUG=1 /snap/bin/titania

remove:
	sudo snap remove titania

publish:
	snapcraft login
	snapcraft push --release=stable titania_*.snap

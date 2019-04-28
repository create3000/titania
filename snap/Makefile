
all: clean compile

configure:
	sudo snap install snapcraft --classic

clean:
	rm -r -f parts
	rm -r -f prime
	rm -r -f snap
	rm -r -f stage
	rm -f titania_*.snap

# Or use snapcraft from /snap/bin/snapcraft
compile:
	snapcraft

install: remove
	sudo snap install titania_*.snap --devmode --dangerous

run:
	env SNAP_DESKTOP_DEBUG=1 /snap/bin/titania

remove:
	sudo snap remove titania

publish:
	snapcraft login
	snapcraft push --release=edge titania_*.snap

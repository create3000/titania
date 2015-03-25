
install-exec-hook:
	install -m 0755 bin/x3d2vrml $(prefix)/bin/
	cp -r share/ $(prefix)/
	find $(prefix)/share/ -type d -exec chmod 755 {} \;
	find $(prefix)/share/ -type f -exec chmod 644 {} \;

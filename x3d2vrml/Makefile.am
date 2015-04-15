
install-exec-hook:
	install -m 0755 bin/x3d2vrml $(prefix)/bin/
	cp -r share/ $(prefix)/
	find $(prefix)/share/titania/ -type d -exec chmod 755 {} \;
	find $(prefix)/share/titania/ -type f -exec chmod 644 {} \;

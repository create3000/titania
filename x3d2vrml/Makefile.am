
install-exec-hook:
	install -m 0755 bin/titania-x3d2vrml $(DESTDIR)/bin/
	cp -r share/ $(DESTDIR)/
	find $(DESTDIR)/share/titania/ -type d -exec chmod 755 {} \;
	find $(DESTDIR)/share/titania/ -type f -exec chmod 644 {} \;

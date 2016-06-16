
install-exec-hook:
	install -m 0755 bin/titania-x3d2vrml $(DESTDIR)$(bindir)
	cp -r share/* $(DESTDIR)$(datadir)
	find $(DESTDIR)$(datadir)/titania/ -type d -exec chmod 755 {} \;
	find $(DESTDIR)$(datadir)/titania/ -type f -exec chmod 644 {} \;

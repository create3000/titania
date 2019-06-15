## Process this file with automake to produce Makefile.in

current_dir = $(shell pwd)

lib_LTLIBRARIES =  \
	libtitania-sqlite.la

libtitania_sqlite_la_LIBADD = $(TITANIA_SQLITE_LIBS)

libtitania_sqlite_la_SOURCES = $(current_dir)/Titania/SQL/SQLite3.cpp

AM_CPPFLAGS = $(TITANIA_SQLITE_CFLAGS)

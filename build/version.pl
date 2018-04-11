#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

my $CWD = `pwd`;
chomp $CWD;

say $CWD;

my $VERSION;
$VERSION = `cat libtitania-x3d/Titania/X3D/Browser/X3DBrowser.cpp`;
$VERSION =~ /version\s*=\s*"(.*?)"/;
$VERSION = $1;

my $ALPHA = $VERSION =~ /a$/;

sub commit
{
	my $version = shift;

	system "git", "commit", "-am", "Published version $VERSION";
	system "git", "push";
	system "git", "push", "origin";
	system "git", "push", "github";
}

sub publish
{
	my $version = shift;

	system "git", "tag", "--delete", "$version";
	system "git", "push", "--delete", "origin", "$version";
	system "git", "push", "--delete", "github", "$version";

	system "git", "tag", "$version";
	system "git", "push", "origin", "--tags";
	system "git", "push", "github", "--tags";
}

sub rsync
{
	my $release = shift;
	my $local   = "/home/holger/Projekte/Titania/libtitania-x3d/share/titania";
	my $ftp     = "/run/user/1000/gvfs/ftp:host=create3000.de/html/create3000.de/code/htdocs/titania/$release/";

	my @folders = (
		"/shaders/glTF/",
	);

	say "Uploading $release";

	foreach (@folders)
	{
		system "mkdir", "-p", "$ftp/$_";
		system "rsync", "-r", "-x", "-c", "-v", "--progress", "--delete", "$local/$_", "$ftp/$_";
	}
}

my $result = system "zenity", "--question", "--text=Do you really want to publish X_ITE X3D v$VERSION now?", "--ok-label=Yes", "--cancel-label=No";

if ($result == 0)
{
	say "Publishing Titania X3D v$VERSION now.";

	# GitHub

	commit;

	publish ("$VERSION");
	rsync ($VERSION);
	rsync ("alpha");

	unless ($ALPHA)
	{
		publish ("latest");
		rsync ("latest");
	}
}

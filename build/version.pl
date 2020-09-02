#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use utf8;
use open qw/:std :utf8/;

use File::Basename qw(dirname);

my $CWD = `pwd`;
chomp $CWD;

say $CWD;

my $VERSION;
$VERSION = `cat libtitania-x3d/Titania/X3D/Browser/X3DBrowser.cpp`;
$VERSION =~ /version\s*=\s*"(.*?)"/;
$VERSION = $1;

my $ALPHA = $VERSION =~ /a$/;

sub share
{
	my $version = shift;
	my $share   = "$CWD/libtitania-x3d/share/titania/";
	my $docs    = "$CWD/docs/code/$version/";

	my @folders = (
		"shaders/glTF",
	);

	foreach (@folders)
	{
		system "rm", "-r", "$docs/$_";
		system "mkdir", "-p", "$docs/$_";
		system "cp", "-r", "$share/$_", "$docs/" . dirname ($_);
	}
}

sub appdata
{
	return if $ALPHA;

	my $appdata = `cat Titania/share/appdata/de.create3000.titania.appdata.xml`;

	$appdata =~ /release.*?version="(.*?)"/s;

	my $date = `date +"%Y-%m-%d"`;
	chomp $date;

	if ($VERSION eq $1)
	{
		$appdata =~ s|<release\s+date=".*?"|<release date="$date"|s;
	}
	else
	{
		$appdata =~ s|(<releases>\n)|$1    <release date="$date" version="$VERSION"/>\n|s;
	}

	open APPDATA, ">", "Titania/share/appdata/de.create3000.titania.appdata.xml";
	print APPDATA $appdata;
	close APPDATA;
}

sub commit
{
	my $version = shift;

	system "git", "add", "-A";
	system "git", "commit", "-am", "Published version $VERSION";
	system "git", "push";
	system "git", "push", "origin";
}

sub publish
{
	my $version = shift;

	system "git", "tag", "--delete", "$version";
	system "git", "push", "--delete", "origin", "$version";

	system "git", "tag", "$version";
	system "git", "push", "origin", "--tags";
}

my $result = system "zenity", "--question", "--text=Do you really want to publish X_ITE X3D v$VERSION now?", "--ok-label=Yes", "--cancel-label=No";

if ($result == 0)
{
	say "Publishing Titania X3D v$VERSION now.";

	# GitHub

	share ("alpha");
	appdata ();
	commit ();

	unless ($ALPHA)
	{
		share ($VERSION);
		share ("latest");
		commit ();
		publish ($VERSION);
		publish ("latest");
	}
}
else
{
	exit 1;
}

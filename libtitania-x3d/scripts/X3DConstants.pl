#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

binmode STDOUT, ":utf8";

use File::Basename qw (basename);

my $components = "/home/holger/Projekte/Titania/libtitania-x3d/Titania/X3D/Components/";
my @nodeTypes;
my @abstractNodeTypes;

sub max { return $_[0] > $_[1] ? $_[0] : $_[1]; }

sub insert {
	my ($filename) = @_;

	chomp $filename;

	my $basename = basename ($filename, ".cpp");



	if ($basename =~ /^X3D/) {
		push @abstractNodeTypes, $basename;
	}
	else {
		push @nodeTypes, $basename;
	}
}

insert ($_)
	foreach sort `find '$components' -name \\*.cpp -exec echo {} \\;`;


#####

say "	// Base node type";
say "";
say "\tX3DBaseNode,";
say "";
say "	// Basic node types";
say "";
foreach (sort @nodeTypes)
{
	say "\t$_,";
}
say "";
say "	// Abstract node types";
say "";
foreach (sort @abstractNodeTypes)
{
	say "\t$_,";
}

#####


####
my $js_n = 0;
my $js_a = 0;
say "		// Base node type";
say "";
say "		JS_X3D_BASE_NODE,";
say "";
say "		// Basic node types";
say "";
foreach (sort @nodeTypes)
{
	my $n = $_;
	$n =~ s/([A-Z][a-z])/_$1/sgo;
	$n = "JS_" . uc $n;
	$n =~ s/__/_/sgo;

	say "		$n,";
	$js_n = max $js_n, length $n;
}
say "";
say "		// Abstract node types";
say "";
foreach (sort @abstractNodeTypes)
{
	my $n = $_;
	$n =~ s/([A-Z][a-z])/_$1/sgo;
	$n = "JS_" . uc $n;
	$n =~ s/__/_/sgo;

	say "		$n,";
	$js_a = max $js_a, length $n;
}

#####

say "	// Base node type";
say "";
say sprintf "	static JSBool X3DBaseNode (JSContext*, JSObject*, jsid, jsval*);", $_;
say "";
say "	// Basic node types";
say "";
my $ln = 0;
$ln = max $ln, length $_ foreach @nodeTypes;
foreach (sort @nodeTypes)
{
	say sprintf "	static JSBool %-${ln}s (JSContext*, JSObject*, jsid, jsval*);", $_;
}
say "";
say "	// Abstract node types";
say "";
my $la = 0;
$la = max $la, length $_ foreach @abstractNodeTypes;
foreach (sort @abstractNodeTypes)
{
	say sprintf "	static JSBool %-${la}s (JSContext*, JSObject*, jsid, jsval*);", $_;
}

#####

say "	// Base node type";
say "";
say "	{ \"X3DBaseNode\", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBaseNode, NULL },";
say "";
say "	// Basic node types";
say "";
foreach (sort @nodeTypes)
{
	my $n = $_;
	$n =~ s/([A-Z][a-z])/_$1/sgo;
	$n = "JS_" . uc $n;
	$n =~ s/__/_/sgo;

	say sprintf "	{ %-" . ($ln + 3) . "s %-" . ($js_n + 1) . "s JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, %-" . ($ln + 1) . "s NULL },",
		"\"$_\",",
		"$n,",
		"$_,";
}
say "";
say "	// Abstract node types";
say "";
foreach (sort @abstractNodeTypes)
{
	my $n = $_;
	$n =~ s/([A-Z][a-z])/_$1/sgo;
	$n = "JS_" . uc $n;
	$n =~ s/__/_/sgo;

	say sprintf "	{ %-" . ($la + 3) . "s %-" . ($js_a + 1) . "s JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, %-" . ($la + 1) . "s NULL },",
		"\"$_\",",
		"$n,",
		"$_,";
}

######


say "// Base node type";
say "";
	say <<EOT;
JSBool
jsX3DConstants::X3DBaseNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBaseNode, vp);
}
EOT
say "";
say "// Basic node types";
say "";
foreach (sort @nodeTypes)
{
	say <<EOT;
JSBool
jsX3DConstants::$_ (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::$_, vp);
}
EOT
}
say "";
say "// Abstract node types";
say "";
foreach (sort @abstractNodeTypes)
{
	say <<EOT;
JSBool
jsX3DConstants::$_ (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::$_, vp);
}
EOT
}



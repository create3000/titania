#!/bin/sh

cp "$1" /tmp/uncrustify-last-document.txt

cat "$1" | \
perl -e '
	$f = join "", <>;
	$f =~ s|^\n+||so;                                                       # remove newlines from beginning of file
	$f =~ s|^/\*.+?\*\*/|\n\n|so;                                           # remove copying header
	print $f;
' | \
perl -p -e '
	s/\[\s+\]/[]/go;                                                        # remove space between []
' |  \
uncrustify -q -l CPP -c "$HOME/bin/uncrustify.cfg" \
	| \
perl -p -e '
	s/^(\t+\<\<)/\t$1/o;                                                     # add a tab before stream operator <<
	s/^(\t+\>\>)/\t$1/o;                                                     # add a tab before stream operator >>
	s/\[\]/[ ]/go;                                                           # add space between [ ]
	s/\!=/not_eq/go;                                                         # change != to not_eq
	s/\|\|/or/go;                                                            # change || to or
	s/>\s+(?=>)/>/go;                                                        # change > > to >>
	s/(\s+-\>)\s*/$1 /go;                                                    # change variable->member to variable -> member
	s/([\w\d\)\]])(-\>)\s*/$1 $2 /go;                                        # change variable->member to variable -> member
	s/-\>\s*\*/->*/go;                                                       # change variable -> * member to variable ->* member
	s/([\w\d]+\:\:)(\s+)/$1/sgo;                                             # remove space after CLASS::
	s/\)\((\w)/) ($1/sgo;                                                    # add space between )(
	s/(\s[\w\d\)\]]+)\.([a-zA-Z])/$1 .$2/go
		unless /\w\.\w+?[\>"]/ or /^\s*(\/?\*|\/\/)/go;                                   # change variable.member to variable .member
' | \
perl -e '
	# indent constuctor bases and member initialization
	sub max { return $_[0] > $_[1] ? $_[0] : $_[1]; }

	@lines = <>;
	@_ = @lines;

	#while ($_ = shift @_) {
	#	last if /^\w+(?:\:\:\w+)+\s\(.*?\)\s\:\n/so;  # find class Constructor Name::Name ():
	#}

	while ($_ = shift @_)
	{
		if (/^(\w+)\:\:(\w+)\s\(/so)  # find class Constructor "Name::Name ("
		{
			if ($1 eq $2) # Constructor begin found
			{
				goto LENGTH if /\)\s\:\n/so;

				while ($_ = shift @_) # find Constructor end ") :"
				{
					goto LENGTH if /\)\s\:\n/so;
				}
			}
		}
	}

	LENGTH: # find length of indent for each member

	$lm = 0;
	$lc = 0;
	while ($_ = shift @_) {
		last unless /^\t+([\w:\s<>]+)\s*(\(.*?\}\)[,\s\n]|\(.*?\)[,\s\n])/so;
		$lm = max ($lm, length ($1));
		$lc = max ($lc, length ($2));
	}

	# print file

	@_ = @lines;

	while (@_)
	{
		while ($_ = shift @_)
		{
			if (/^(\w+)\:\:(\w+)\s\(/so)  # find class Constructor "Name::Name ("
			{
				if ($1 eq $2) # Constructor begin found
				{
					goto MEMBERS if /\)\s\:\n/so;

					print $_;

					while ($_ = shift @_) # find Constructor end ") :"
					{
						goto MEMBERS if /\)\s\:\n/so;

						print $_;
					}
				}
			}
			print $_;
		}

		MEMBERS:

		print $_;
		last unless @_;

		while ($_ = shift @_) {
			last unless /^(\t+)([\w:\s<>]+)\s*([{(].*?\)\)[,\s\n]|\(.*?[)}][,\s\n])(\s*)(.*?)$/so;
			$pad1 = $lm - length ($2);
			$pad2 = $lc - length ($3) + 1;
			print $1, " " x $pad1, $2, $3;
			print " " x $pad2, $5, "\n" unless $3 =~ /\n/o;
		}
		print $_;
	}

	print $_ while $_ = shift @_;
' | \
perl -e '
	# multiline replacements
	$f = join "", <>;

	# remove newline before doxy comment group indicator //@}
	$f =~ s|\n(\s*//@})|$1|sg;

	$f =~ s/\n((?:template|extern template)[^\n&]+&\n[^\n;]+;)/$p = $1, $p =~ s|\n| |sg, "\n$p"/sge;

	# collapse template instantiations.
	$f =~ s/\n(template[^\n]+?;\n\n(?:\/\/)?template)/$p = $1, $p =~ s|\n\n|\n|sg, "\n$p"/sge;
	$f =~ s/\n(template[^\n]+?;\n\n(?:\/\/)?template)/$p = $1, $p =~ s|\n\n|\n|sg, "\n$p"/sge;
	$f =~ s/\n(extern template[^\n]+?;\n\n(?:\/\/)?extern template)/$p = $1, $p =~ s|\n\n|\n|sg, "\n$p"/sge;
	$f =~ s/\n(extern template.[^\n]+?;\n\n(?:\/\/)?extern template)/$p = $1, $p =~ s|\n\n|\n|sg, "\n$p"/sge;

	# .operator ->
	$f =~ s/\.\n\s+operator ->/.operator ->/sgo;

	# add new line before end of class body.
	$f =~ s/([^\n])\n};/$1\n\n};/sgo;

	print $f;
' \
> /tmp/uncrustify.txt

mv /tmp/uncrustify.txt "$1"

rename-header-guards.pl /home/holger/Projekte/Titania/ "$1"

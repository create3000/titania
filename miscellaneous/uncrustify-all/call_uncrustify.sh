#!/bin/sh

cp "$1" /tmp/uncrustify-last-document.txt

#cat "$1" | uncrustify -q -l CPP -c "$HOME/bin/uncrustify.cfg" > /tmp/uncrustify.txt
#mv /tmp/uncrustify.txt "$1"
#exit;

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
	s/^(\t+\<\<)/\t$1/o;                                                    # add a tab before stream operator <<
	s/^(\t+\>\>)/\t$1/o;                                                    # add a tab before stream operator >>
	s/\[\]/[ ]/go;                                                          # add space between [ ]
	s/\!=/not_eq/go;                                                        # change != to not_eq
	s/\|\|/or/go;                                                           # change || to or
	s/>\s+(?=>)/>/go;                                                       # change > > to >>
	s/></> </go;                                                            # add space between ><
	s/(\s+-\>)\s*/$1 /go;                                                   # change variable->member to variable -> member
	s/([\w\d\)\]])(-\>)\s*/$1 $2 /go;                                       # change variable->member to variable -> member
	s/-\>\s*\*/->*/go;                                                      # change variable -> * member to variable ->* member
	s/([\w\d]+\:\:)(\s+)/$1/sgo;                                            # remove space after CLASS::
	s/\)\((\w)/) ($1/sgo;                                                   # add space between )(
	s/(\s[\w\d\)\]]+)\.([a-zA-Z])/$1 .$2/go
		unless /\w\.\w+?[\>"]/ or /^\s*(\/?\*|\/\/)/go;                      # change variable.member to variable .member
   s/\)and/) and/sgo;
   s/([)])(override|final)/$1 $2/;                                         # add space before final or override
' | \
perl -e '
	@lines = <>;
	$file = join "", @lines;
   $file =~ s/\<(\w+.*?)\n\s+\>\s\(/<$1> (/sgo;
   print $file;
' | \
perl -e '
	# indent constuctor bases and member initialization
	sub max { return $_[0] > $_[1] ? $_[0] : $_[1]; }

	$constructor_rx  = qr/(\w+)\:\:(\w+)\s\(/so;
	$member_value_rx = qr/\((?:[^()]*|(?0))*\)|\{(?:[^{}]*|(?0))*\}/so;
	$member_rx       = qr/^(\t+)([\w:\s<>]+)\s*((??{$member_value_rx}),?)(\n|\s*)(.*?)$/so;

	@lines = <>;

	while (@lines)
	{
		@_ = @lines;
		
		#print  @_;
		#return;

		while ($_ = shift @_)
		{
			if (constructor_rx)  # find class Constructor "Name::Name ("
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
			last unless $_ =~ $member_rx;
			$lm = max ($lm, length ($2));
			$lc = max ($lc, length ($3));
		}

		# print file

		while ($_ = shift @lines)
		{
			if (constructor_rx)  # find class Constructor "Name::Name ("
			{
				if ($1 eq $2) # Constructor begin found
				{
					goto MEMBERS if /\)\s\:\n/so;

					print $_;

					while ($_ = shift @lines) # find Constructor end ") :"
					{
						goto MEMBERS if /\)\s\:\n/so;

						print $_;
					}
				}
			}
			print $_;
		}

		last unless @lines;

		MEMBERS:

		print $_;

		while ($_ = shift @lines) {
			last unless $_ =~ $member_rx;
			$pad1 = $lm - length ($2);
			$pad2 = $lc - length ($3) + 1;
			$line = $1 . (" " x $pad1) . $2 . $3;
			$line .= (" " x $pad2) . $5, "\n" unless $3 =~ /\n/o;
			$line =~ s/(.*?)\s*$/$1\n/o;
			print $line;
		}

		print $_;
	}
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

#!/bin/bash

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
	sub replace {
		my $expression = shift;
		
		my @parts = split "\"", $_;

		for (0 ... (@parts - 1))
		{
			$expression -> ($parts [$_]) unless $_ % 2;
		}

		$_ = join "\"", @parts;
	}

	replace (sub { $_ [0] =~ s/^(\t+\<\<)/\t$1/o });                                                    # add a tab before stream operator <<
	replace (sub { $_ [0] =~ s/^(\t+\>\>)/\t$1/o });                                                    # add a tab before stream operator >>
	replace (sub { $_ [0] =~ s/\[\]/[ ]/go });                                                          # add space between [ ]
	replace (sub { $_ [0] =~ s/\!=/not_eq/go });                                                        # change != to not_eq
	replace (sub { $_ [0] =~ s/\|\|/or/go });                                                           # change || to or
	replace (sub { while ($_ [0] =~ s/>(\s+)>/>>$1/go) { } });                                          # change > > to >>
   replace (sub { $_ [0] =~ s/\s+;/;/so });                                                            # remove space before semicolon
	replace (sub { $_ [0] =~ s/></> </go });                                                            # add space between ><
	replace (sub { $_ [0] =~ s/(\s+-\>)\s*/$1 /go });                                                   # change variable->member to variable -> member
	replace (sub { $_ [0] =~ s/([\w\d\)\]])(-\>)\s*/$1 $2 /go });                                       # change variable->member to variable -> member
	replace (sub { $_ [0] =~ s/-\>\s*\*/->*/go });                                                      # change variable -> * member to variable ->* member
	replace (sub { $_ [0] =~ s/([\w\d]+\:\:)(\s+)/$1/sgo });                                            # remove space after CLASS::
	replace (sub { $_ [0] =~ s/\)\((\w)/) ($1/sgo });                                                   # add space between )(
	replace (sub { $_ [0] =~ s/(\s[\w\d\)\]]+)\.([a-zA-Z])/$1 .$2/go })
		unless /\w\.\w+?[\>"]/ or /^\s*(\/?\*|\/\/)/go;                                        # change variable.member to variable .member
   replace (sub { $_ [0] =~ s/\)and/) and/sgo });
   replace (sub { $_ [0] =~ s/([)])(override|final)/$1 $2/so });                                         # add space before final or override
   replace (sub { $_ [0] =~ s/<\:\:/< ::/so });                                                          # add space :: in template argument list
   replace (sub { $_ [0] =~ s/\s(not_eq|and|or)\s*\*\s*/ $1 */sgo });                                             # not_eq *
   replace (sub { $_ [0] =~ s/(\w)\s+\*\s+const/$1* const/sgo });                                        # remove space befor *
   replace (sub { $_ [0] =~ s/(\w)\s*:\*\s*(\w)/$1 : *$2/so });                                          # remove :* in for loops
   replace (sub { $_ [0] =~ s/\>&&/> &&/so });                                                           # add space before move operator
   replace (sub { $_ [0] =~ s/\>\s*&/> &/so });                                                          # add space before reference operator
   replace (sub { $_ [0] =~ s/\>&(\w)/> & $1/so });                                                      # add space before reference operator
   replace (sub { $_ [0] =~ s/(not_eq\s+[-+*&])\s+/$1/so });                                             # refine not_eq
   $_  =~ s/^;\n/\t\t;\n/so;                                                                             # indent ; at start of line
' | \
perl -e '
	# Remove \n in template argument one liner
	@lines = <>;
	$file = join "", @lines;
   $file =~ s/\<(\w+.*?)\n\s+\>\s\(/<$1> (/sgo;
   print $file;
' | \
perl -e '
	# indent constuctor bases and member initialization
	sub max { return $_[0] > $_[1] ? $_[0] : $_[1]; }

	$constructor_rx  = qr/(\w+)\:\:(\w+)\s\(/so;
	$member_rx       = qr/^(\t+)([\/\w:\s<>]+)\s*(.*)$/so;
	$member_start_rx = qr/^(\t+)([\/\w:\s<>]+)\s*\(/so;
	$member_end_rx   = qr/\),?\s*$/so;
	$block_start_rx  = qr/^\s*(?:\)\s*\{|\{)/so;

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

		$p  = 0;
		$lm = 0;
		$lc = 0;
		while ($_ = shift @_)
		{
			last if $_ =~ $block_start_rx;
			next unless $_ =~ $member_start_rx;
			last unless $_ =~ $member_rx;

			if ($p == 0)
			{
				$lm = max ($lm, length ($2));
				$lc = max ($lc, length ($3));
			}
	
			# Count parenthesises
			my $o = () = $_ =~ /\(/sgo; # opened
			my $c = () = $_ =~ /\)/sgo; # closed

			$p += $o - $c;
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
		
		$p = 0;

		while ($_ = shift @lines) {
			last if $_ =~ $block_start_rx;

			$member_start = $_ =~ $member_start_rx;

			last unless $_ =~ $member_rx;
			
			$tab   = $1;
			$name  = $2; # spaces + name 
			$value = $3;

			$pad1 = $p == 0
			        ? $lm - length ($name)
			        : $lm + 1;
			
			$name =~ s/^\s*//sgo if $p;
	
			$line = $tab . (" " x $pad1) . $name . $value;
			$line =~ s/(.*?)\s*$/$1\n/o;
			print $line;
			
			# Count parenthesises
			my $o = () = $_ =~ /\(/sgo; # opened
			my $c = () = $_ =~ /\)/sgo; # closed

			$p += $o - $c;
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

	$f =~ s/\n(\n\s*\{ \})/$1/sgo;

	print $f;
' \
> /tmp/uncrustify.txt

mv /tmp/uncrustify.txt "$1"

if [[ "$1" == *.h ]]
then
	rename-header-guards.pl /home/holger/Projekte/Titania/ "$1"
fi

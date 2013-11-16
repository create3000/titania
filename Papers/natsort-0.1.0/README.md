# Natural Sorting utility

**natsort** is a small tool to sort a list of lines, using natural sort order.
It reads lines to sort from its standard input and writes them sorted on
standard output.

## What is natural order?

Most sorting utility perform a simple ASCII comparison, with works as computers
might expect things, but not so much our human selves; Because when numbers are
involed, e.g. with "test15" and "test2" we usually want to see "test2" before
"test15" even though, taken character by character, 1 does come before 2.

There's been plenty of talk about this, the point being: natural order is what
people will mostly (if not always) expect, but not what they always get.

natsort will perform natural sort order, and in case two numbers evaluate to the
same value but aren't the same (i.e. one has (more) leading zeros, e.g. 02 vs
002) the one with less leading zeros will come first.

Unfortunately things aren't so simple as to simple treat numbers differently,
because people might expect ordering based on their locale, so that characters
with accents are placed alongside their accent-free counterparts and such.

This will not happen with natsort, because its comparison isn't locale based. It
performs simple character comparisons, but with support for natural order and a
few more options (e.g. case sensitivity).

## Want to know more?

**natsort** is a simple example of an implementation of natural sort order. For
more information :

- more about [how it works and why it came
about](http://jjacky.com/2013-03-22-natural-sort-order-in-c "Natural Sort Order in C")

- [source code & issue tracker](https://github.com/jjk-jacky/natsort "natsort @ GitHub.com")

- [PKGBUILD in AUR](https://aur.archlinux.org/packages/natsort/ "AUR: natsort")

Plus, natsort comes with a man page.

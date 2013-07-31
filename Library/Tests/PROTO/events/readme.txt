

                              INTRODUCTION

Prof Paul Fishwick has a vision of modeling and simulation called rube.
See his web page (http://www.cise.ufl.edu/~fishwick/).  Within the group
working on rube, it is my task to develop "innards" to support this
effort with event scheduling primitives to implement a conservative
distributed simulation strategy rooted in VRML, including support for
what Prof Fishwick has termed "virtual time" (which differs from
Jefferson's use of the phrase in an optimistic simulation strategy).
Prof Fishwick has imposed a requirement that the software work under two
VRML plug-ins: blaxxun contact and CosmoPlayer.

To reach this objective, i sailed a hostile sea.  My compass and sextant
have been Carey & Bell's book, but there are uncharted reefs and shoals
in VRML, and i have lots of wreckage to show for it.  I began this
process as a VRML neophyte with paradigm blindness for the event
execution model.  But although some of the pain which accompanies a
paradigm shift is inescapable, much of the pain could have been avoided
had I not suffered from certain deceptive appearances, about which, more
below.

Recently an email conversation took place at www-vrml@web3d.org.  Prof
Fishwick wrote (my ref #s 14526, 14534, 14557, 14570, 14571):

  From: Paul Fishwick [mailto:fishwick@cise.ufl.edu]
  Sent: Wednesday, February 09, 2000 4:24 PM
  To: Braden N. McDaniel
  Cc: www-vrml@web3d.org
  Subject: RE: [www-vrml] Scripts

  Yes, I will ask one of my research staff to provide this list with a
  specific example.  In short, the problems arise when one has
  statements within a script that generate events, either as a result of
  setting an eventout or as a result of setting an exposed field of an
  attribute defined outside of the scope of the script.  The result in
  Cosmo is that one cannot guarantee the order in which the events are
  executed since they have the same time stamps.  This is a really bad
  thing.  The order in which events occur should be the same in which
  the statements appear syntactically within the script.  Blaxxun
  supports this.  Cosmo does not.  The VRML97 is ambiguous to the extent
  that it actually allows Cosmo's behavior for this event ordering.
  Anyway, I'll let Robert give you more specific example(s).

Here is that example.

The original of this email is being sent with a .zip file attached.  The
.zip file triples the size of the email, so, as a matter of email
etiquette, the attachment is being sent only to the "to" addressee; the
"cc" addressee, a mailing list, is getting the smaller version.
However, copies of the .zip file are available to members of the
mailing list, upon request.


                               BACKGROUND

This is readme.txt.  It is being packaged with a VRML world that is a
reduced version of the rube movers and paths functional block model
world (mpfbm).  If you'd like to see some movers and paths worlds, they
are available thru my webpage (http://www.cise.ufl.edu/~rmc/rube/).

Amyway, most of mpfbm's functionality is omitted here, so you won't see
much interesting behavior in your web browser.  The real action is in
the console log.

This world was tested in two environments:

  1. web browser: Internet Explorer 5
     VRML plug-in: blaxxun 4.2

  2. web browser: Netscape 4.6x
     VRML plug-in: CosmoPlayer 2.1.1

For each of the two environments, console log output was copied and
pasted into a file (blaxxun.log and cosmo.log) which is included in this
package.  To see the behavior reported, you may either view the log
files, or you may run the world yourself.

To activate the world:  (1) Use winzip to extract the files from the
.zip file into the directory of your choice, (2) open the web browser,
(3) use windows explorer to navigate to the directory containing the
world, (5) drag world.wrl to the open web browser window and drop it
there.


                              THE PROBLEM

Consider a PROTO whose definition is a Transform, one of whose children
is Script node S.  The url field of S begins with '"javascript: ...',
and consists of statements written in ECMAscript.

VRML events may be generated during an event cascade by execution of
"assignment" statements like these within the code of S:

    print('a');
    someNode.oneOfItsEventIns = aValue;  // event 1, prints 'b'
    print('c');
    oneOfMyEventOuts = aValue;           // event 2, prints 'd'
    print('e');

Suppose "someNode" has "oneOfItsEventIns" associated by IS with code in
a Script node which executes "print('b');"; similarly, suppose
"oneOfMyEventOuts" is ROUTEd to an eventIn of S whose code executes
"print('d');".

Events 1 and 2 above will have the same VRML time stamp as each other,
and that time stamp will be the same as the time stamp of the
currently-executing event cascade.  Thus events 1 and 2 will "occur"
during the present event cascade.  An interesting question is what can
we say about the sequence in which the computer will execute these
events.

There be those among us who think it reasonable to expect the log to
contain

    a...b...c...d...e.

But that is not required by the VRML spx, and indeed that is not what
happens with Cosmo.  The sequence

    a...c...e...b...d

accords with a rationale that events occur in the order we execute the
scheduling of them (the "assignment" statements above).  For example, we
think it reasonable for a simulation future event list to be sorted on
primary key simulation time, and secondarily FIFO.  So we are happy with
"a...b...c...d...e" and we accept "a...c...e...b...d" as reasonable.
But how do we characterize the sequence

    a...c...e...d...b?

How about with confusion and broken worlds with strange and usually
pathological behavior which differs markedly from one VRML plug-in to
another?  Yet the last sequence above is what Cosmo often produces, so
you cannot count on anything taking effect in any particular sequence in
the present Script node nor in any Script node ROUTEd to from this
Script node, unless you force the VRML time stamp to advance (even a
millisecond is fine), and put the "later" code somewhere else.  For
worlds with more than trivial behaviors in Script nodes, this is a major
issue.

As a bandaid, i created a SYNC PROTO.  ROUTEing an event via a SYNC
instance guarantees advance of VRML time by say 1 msec, which, in turn,
guarantees that everything "already" encountered by executing
"assignment" statements (and that has NOT gone thru a SYNC node) has
"already happened" by the time the code on the far side of the SYNC node
executes.  Another way to say this is that by advancing the VRML clock
from t to t+delta_t, we force occurrence of every event already
scheduled whose time of occurrence is less than t+delta_t.  In
particular, this forces occurrence of all events scheduled to occur at
VRML time t.


But this bandaid does not scale well, because we cannot say WHICH of the
events scheduled to occur at VRML time t will happen first; that is,
because we cannot predict "b...d" or "d...b".  As the number of such
events increases, as is the case in worlds whose Script node behavior is
non-trivial, the overhead of generating multiple SYNC nodes increase,
and readability of the "program" goes down, because we must put the
logic on an "assembly line" and use a SYNC node followed by a different
Script eventIn for EVERY "assignment" to a node.eventIn (or exposedField)
that we perform to guarantee evaluation in a required sequence.


                       SPECIFICALLY IN THIS WORLD

In this world, a MOVER has an SFNode field into which we put an EVT.
The EVT is an SFNode which, in turn, contains some other things,
ultimately an SFBool.  The EVT is hooked up with a MOVER at a certain
juncture "after" the MOVER was created (fromString).  We assign the EVT
to the MOVER's field by using a MOVER eventIn:

    mvr.setEvt = nuEvt; // (in efactory.wrl)

and "then" ROUTE the MOVER on its way.  In blaxxun, the "assignment"
takes place as one might expect if it were an assignment.  But in Cosmo,
when the MOVER arrives at another node, mysteriously, the EVT field of
the MOVER remains null, and, not surprisingly, the true/false value
within EVT is inaccessible:

  blaxxun:

    SYNC arrive, mover=101, ts: 950279884.0091509
    mover=101 inserted at cursor: 0

  cosmo:

    SYNC arrive, mover=101, ts: 950279958.010896
    **** evt null ****
    mover=101 inserted at cursor: 0


                      AN ASIDE ABOUT EXPOSEDFIELDS

If you wonder, why not just set the field, why use an eventIn, it is to
apply the Object paradigm's principles of encapsulation and information
hiding.  We can still enjoy benefits equivalent to using exposedFields,
but instead of accessing them as "node.field=x;" and "y=node.field;", we
access them from outside as "node.anEventIn=x;" and "y=node.anEventOut;".
This overcomes the prohibition against exposedFields in Script nodes.
By splitting them out into (eventIn, field, eventOut) triples, we have
private access to fields within the Script node inside the PROTO where
the fields are declared, and we access them from outside only with the
equivalent of accessor and mutator methods.  Caveat:  when "requesting"
a mutator method with the ECMAscript thing that looks like an assignment
statement, it is important to have a clear understanding of what to
expect in terms of what possible ordering its occurrence may have in the
event sequence.


                               CONCLUSION

i find 'url "javascript:' a misnomer.  ECMAscript is no more JavaScript
than i am the Easter Bunny.  An ECMAscript "assignment" statement is no
assignment statement, due to VRML notions of "before" and "after".
Worse, it appears deceptively like an assignment statement and so lures
a novice like me into wholly erroneous perspectives that are dispelled
only after great frustration and pain.


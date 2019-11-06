#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

my $component = "Annotation";
my $file      = `cat $component.txt`;
my $nodes     = [ ];

sub parse_component
{
	parse_nodes ();
}

sub parse_nodes
{
	1 while parse_node ();
}

sub parse_node
{
	if ($file =~ /\G\s*([\w\d]+)/gc)
	{
		my $node = { bases => [ ], fields => [ ] };

		say $1;
		$node -> {typeName} = $1;

		if ($file =~ /\G\s*:\s*/gc)
		{
			if ($file =~ /\G\s*([\w\d]+)/gc)
			{
				push @{$node -> {bases}}, $1;

				while ($file =~ /\G\s*,\s*([\w\d]+)/gc)
				{
					push @{$node -> {bases}}, $1;
				}

				if ($file =~ /\G\s*\{/gc)
				{
					parse_fields ($node);

					if ($file =~ /\G\s*\}/gc)
					{
						push @$nodes, $node;

						return 1;
					}
				}
			}
		}
	}

	return 0;
}

sub parse_fields
{
	my $node = shift;

	1 while parse_field ($node);
}

sub parse_field
{
	my $node = shift;

	if ($file =~ /\G\s*([\w\d]+)/gc)
	{
		my $field = { };

		$field -> {type} = $1;

		if ($file =~ /\G\s*\[(in)?,?(out)?\]/gc)
		{
			$field -> {in}  = $1 ? 1 : 0;
			$field -> {out} = $2 ? 1 : 0;

			if ($file =~ /\G\s*([\w\d]+)/gc)
			{
				$field -> {name} = $1;

				if ($file =~ /\G\s*?(?:\n|\s*(.*?)\s*?(?:\(.*?\n|\[.*?\n|\n))/gc)
				{
					$field -> {value} = $1 // "";

					push @{$node -> {fields}}, $field;
					return 1;
				}
			}
		}
	}

	return 0;
}

sub create_files
{
	system "mkdir -p 'cpp/$component'";
	system "mkdir -p 'js/$component'";

	create_h ($_) foreach (@$nodes);
	create_cpp ($_) foreach (@$nodes);
	create_js ($_) foreach (@$nodes);
}

sub create_h
{
	my $node = shift;
	my $guard = "${component}_$node->{typeName}";

	$guard =~ s/([a-z])([A-Z])/$1_$2/g;
	$guard = uc ($guard);

	open FILE, ">", "cpp/$component/$node->{typeName}.h";
	print FILE <<EOT;
/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig\@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig\@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_COMPONENTS_${guard}_H__
#define __TITANIA_X3D_COMPONENTS_${guard}_H__

EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "#include \"../Core/$base.h\"\n";
	}

	print FILE <<EOT;

namespace titania {
namespace X3D {

class $node->{typeName} :
EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "	public $base,\n";
	}

	print FILE <<EOT;
{
public:
EOT

	unless ($node -> {typeName} =~ /^X3D/)
	{
		print FILE <<EOT;

	///  \@name Construction

	$node->{typeName} (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  \@name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }
EOT
	}

	print FILE <<EOT;

	///  \@name Fields
EOT

	foreach my $field (@{$node -> {fields}})
	{
		next if is_in_base ($node, $field);

		print FILE "\n";
		print FILE "	virtual\n";
		print FILE "	$field->{type} &\n";
		print FILE "	$field->{name} ()\n";
		print FILE "	{ return *fields .$field->{name}; }\n";
		print FILE "\n";
		print FILE "	virtual\n";
		print FILE "	const $field->{type} &\n";
		print FILE "	$field->{name} () const\n";
		print FILE "	{ return *fields .$field->{name}; }\n";
	}

	print FILE <<EOT;

	///  \@name Destruction

	virtual
	~$node->{typeName} () override;


protected:

	///  \@name Construction
EOT
	if ($node -> {typeName} =~ /^X3D/)
	{
		print FILE "\n	$node->{typeName} (X3DExecutionContext* const executionContext);\n";
	}

	print FILE <<EOT;

	virtual
	void
	initialize () override;


private:
EOT
	unless ($node -> {typeName} =~ /^X3D/)
	{
		print FILE <<EOT;

	///  \@name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;
EOT
	}

	print FILE <<EOT;

	///  \@name Fields

	struct Fields
	{
		Fields ();

EOT
	foreach my $field (@{$node -> {fields}})
	{
		next if is_in_base ($node, $field);

		print FILE "		$field->{type}* const $field->{name};\n";
	}

	print FILE <<EOT;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
EOT

	close FILE;
}

sub create_cpp
{
	my $node = shift;

	open FILE, ">", "cpp/$component/$node->{typeName}.cpp";
	print FILE <<EOT;
/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig\@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig\@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "$node->{typeName}.h"

EOT

	print FILE "#include \"../../Execution/X3DExecutionContext.h\"\n\n" unless $node -> {typeName} =~ /^X3D/;

	print FILE <<EOT;
namespace titania {
namespace X3D {
EOT

	unless ($node -> {typeName} =~ /^X3D/)
	{
		print FILE <<EOT;

const Component   $node->{typeName}::component      = Component ("$component", 0);
const std::string $node->{typeName}::typeName       = "$node->{typeName}";
const std::string $node->{typeName}::containerField = "xxxxx";
EOT
	}

	print FILE <<EOT;

$node->{typeName}::Fields::Fields () :
EOT
	foreach my $field (@{$node -> {fields}})
	{
		next if is_in_base ($node, $field);

		$field -> {value} =~ s/NULL//;
		$field -> {value} =~ s/""//;
		$field -> {value} =~ s/TRUE/true/i;
		$field -> {value} =~ s/FALSE/false/i;
		$field -> {value} =~ s/\s+/, /g;

		print FILE "	$field->{name} (new $field->{type} ($field->{value})),\n";
	}

	print FILE <<EOT;
{ }

$node->{typeName}::$node->{typeName} (X3DExecutionContext* const executionContext) :
EOT

	print FILE "	X3DBaseNode (executionContext -> getBrowser (), executionContext),\n" unless $node -> {typeName} =~ /^X3D/;

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "	${base} (),\n";
	}

	print FILE <<EOT;
	fields ()
{
	addType (X3DConstants::$node->{typeName});
EOT

	unless ($node -> {typeName} =~ /^X3D/)
	{
		print FILE "\n";

		foreach my $field (@{$node -> {fields}})
		{
			my $accessType = "";
			$accessType = "initializeOnly" if ! $field -> {in} && ! $field -> {out};
			$accessType = "inputOnly"      if   $field -> {in} && ! $field -> {out};
			$accessType = "outputOnly"     if ! $field -> {in} &&   $field -> {out};
			$accessType = "inputOutput"    if   $field -> {in} &&   $field -> {out};

			print FILE "	addField ($accessType, \"$field->{name}\", $field->{name} ());\n";
		}
	}

	print FILE <<EOT;
}

X3DBaseNode*
$node->{typeName}::create (X3DExecutionContext* const executionContext) const
{
	return new $node->{typeName} (executionContext);
}

void
$node->{typeName}::initialize ()
{
EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "	${base}::initialize ();\n";
	}

	print FILE <<EOT;
}

$node->{typeName}::~$node->{typeName} ()
{ }

} // X3D
} // titania
EOT

	close FILE;
}

sub create_js
{
	my $node = shift;

	open FILE, ">", "js/$component/$node->{typeName}.js";
	print FILE <<EOT;
/* -*- Mode: JavaScript; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig\@yahoo.de>.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 2015, 2016 Holger Seelig <holger.seelig\@yahoo.de>.
 *
 * This file is part of the X_ITE Project.
 *
 * X_ITE is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * X_ITE is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with X_ITE.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/


define ([
	"x_ite/Fields",
	"x_ite/Basic/X3DFieldDefinition",
	"x_ite/Basic/FieldDefinitionArray",
EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "	\"x_ite/Components/Core/$base\",\n",
	}

	print FILE <<EOT;
	"x_ite/Bits/X3DConstants",
],
function (Fields,
          X3DFieldDefinition,
          FieldDefinitionArray,
EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "          $base,\n";
	}

	print FILE <<EOT;
          X3DConstants)
{
"use strict";

	function $node->{typeName} (executionContext)
	{
EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "		$base .call (this, executionContext);\n";
	}

	print FILE <<EOT;

		this .addType (X3DConstants .$node->{typeName});
	}

	$node->{typeName} .prototype = Object .assign (Object .create ($node->{bases}[0] .prototype),
EOT

	for (1 ... scalar (@{$node->{bases}}) - 1)
	{
		print FILE "		$node->{bases}->[$_] .prototype,\n";
	}

	print FILE <<EOT;
	{
		constructor: $node->{typeName},
EOT

	unless ($node -> {typeName} =~ /^X3D/)
	{
		print FILE <<EOT;
		fieldDefinitions: new FieldDefinitionArray ([
EOT

		foreach my $field (@{$node -> {fields}})
		{
			my $accessType = "";
			$accessType = "initializeOnly" if ! $field -> {in} && ! $field -> {out};
			$accessType = "inputOnly"      if   $field -> {in} && ! $field -> {out};
			$accessType = "outputOnly"     if ! $field -> {in} &&   $field -> {out};
			$accessType = "inputOutput"    if   $field -> {in} &&   $field -> {out};

			$field -> {value} =~ s/NULL//;
			$field -> {value} =~ s/""//;
			$field -> {value} =~ s/TRUE/true/i;
			$field -> {value} =~ s/FALSE/false/i;

			print FILE "			new X3DFieldDefinition (X3DConstants .$accessType, \"$field->{name}\", new Fields .$field->{type} ($field->{value})),\n";
		}

		print FILE <<EOT;
		]),
		getTypeName: function ()
		{
			return "$node->{typeName}";
		},
		getComponentName: function ()
		{
			return "$component";
		},
		getContainerField: function ()
		{
			return "xxxxxxxx";
		},
EOT
	}

	print FILE <<EOT;
		initialize: function ()
		{
EOT

	foreach my $base (@{$node -> {bases}})
	{
		print FILE "			$base .prototype .initialize .call (this);\n";
	}

	print FILE <<EOT;
		},
	});

	return $node->{typeName};
});


EOT

	close FILE;

}

sub is_in_base
{
	my $node  = shift;
	my $field = shift;

	foreach my $base (@{$node -> {bases}})
	{
		my $node = get_node ($base);

		next unless $node;

		foreach my $f (@{$node -> {fields}})
		{
			return 1 if $f -> {name} eq $field -> {name};
		}

		return 1 if is_in_base ($node, $field);
	}

	return 0;
}

sub get_node
{
	my $typeName = shift;

	foreach my $node (@$nodes)
	{
		return $node if $typeName eq $node -> {typeName};
	}

	return undef;
}

parse_component ();
create_files ();

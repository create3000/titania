/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "X3DBrowserSelection.h"

#include "../Browser/BrowserWindow.h"
#include "../OutlineEditor/OutlineTreeViewEditor.h"

namespace titania {
namespace puck {

static constexpr double SELECTION_TIME = 0.01; // Use ExamineViewer SPIN_RELEASE_TIME here.

X3DBrowserSelection::X3DBrowserSelection (BrowserWindow* const browserWindow) :
	      X3DBaseInterface (),
	X3D::X3DPointingDevice (browserWindow -> getBrowser ()),
	               enabled (false),
	             pressTime (0),
	              hasMoved (false)
{ }

void
X3DBrowserSelection::setEnabled (const bool value)
{
	enabled = value;

	if (enabled)
	{
		connect ();

		getBrowser () -> setPicking (false);
	}
	else
	{
		disconnect ();

		getBrowser () -> setPicking (true);
	}
}

bool
X3DBrowserSelection::trackSensors ()
{
	return not (getBrowserWindow () -> getKeys () .shift () and not getBrowserWindow () -> getKeys () .control ());
}

void
X3DBrowserSelection::motionNotifyEvent (const bool)
{
	hasMoved = true;
}

bool
X3DBrowserSelection::buttonPressEvent (const bool, const int button)
{
	pressTime = chrono::now ();
	hasMoved  = false;

	switch (button)
	{
		case 2:
		{
			if (getBrowserWindow () -> getKeys () .shift ())
			{
				__LOG__ << std::endl;
				// Activate SnapTarget.
			}

			break;
		}
		default:
			break;
	}

	return true;
}

void
X3DBrowserSelection::buttonReleaseEvent (const bool picked, const int button)
{
	switch (button)
	{
		case 1:
		{
			// Selected highest or lowest Node, or clear selection.z

			if (hasMoved and chrono::now () - pressTime > SELECTION_TIME)
				return;

			if (picked)
			{
				const auto hit       = getBrowser () -> getHits () .front ();
				const auto hierarchy = X3D::find (getBrowser () -> getExecutionContext () -> getRootNodes (),
				                                  hit -> shape,
				                                  X3D::TRAVERSE_ROOT_NODES |
				                                  X3D::TRAVERSE_PROTO_INSTANCES |
				                                  X3D::TRAVERSE_INLINE_NODES |
				                                  X3D::TRAVERSE_TOOL_OBJECTS);

				if (not hierarchy .empty ())
				{
					X3D::SFNode node;

					if (getBrowserWindow () -> getConfig () .getBoolean ("selectLowest"))
					{
						for (const auto & object : basic::reverse_adapter (hierarchy))
						{
							const X3D::SFNode lowest (object);

							if (not lowest)
								continue;

							if (lowest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
								continue;
								
							if (not node)
								node = lowest;

							if (dynamic_cast <X3D::Transform*> (lowest .getValue ()))
							{
								node = lowest;
								break;
							}
						}
					}
					else
					{
						// Find highest Transform
					
						for (const auto & object : hierarchy)
						{
							const X3D::SFNode highest (object);

							if (not highest)
								continue;

							if (highest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
								continue;

							if (not node)
								node = highest;

							if (dynamic_cast <X3D::Transform*> (highest .getValue ()))
							{
								node = highest;
								break;
							}
						}

						// If highest is a LayerSet, no Transform is found and we search for the highest X3DChildNode.

						if (X3D::x3d_cast <X3D::LayerSet*> (node .getValue ()))
						{
							for (const auto & object : hierarchy)
							{
								const X3D::SFNode highest (object);

								if (not highest)
									continue;

								if (highest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
									continue;

								if (X3D::x3d_cast <X3D::X3DChildNode*> (highest .getValue ()))
								{
									node = highest;
									break;
								}
							}
						}
					}

					if (node)
					{
						if (getBrowser () -> getSelection () -> isSelected (node))
							getBrowser () -> getSelection () -> removeChildren ({ node });

						else
						{
							if (getBrowserWindow () -> getKeys () .shift ())
								getBrowser () -> getSelection () -> addChildren ({ node });

							else
								getBrowser () -> getSelection () -> setChildren ({ node });
						}

						getBrowser () -> update ();

						if (getBrowserWindow () -> getConfig () .getBoolean ("followPrimarySelection"))
							getBrowserWindow () -> getOutlineTreeView () -> expand_to (node);
					}
				}
			}
			else
			{
				getBrowser () -> getSelection () -> clear ();
				getBrowser () -> update ();
			}
			
			break;
		}
		default:
			break;
	}
}

X3DBrowserSelection::~X3DBrowserSelection ()
{ }

} // puck
} // titania

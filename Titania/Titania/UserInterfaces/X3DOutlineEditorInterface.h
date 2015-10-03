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
#ifndef __TMP_GLAD2CPP_OUTLINE_EDITOR_H__
#define __TMP_GLAD2CPP_OUTLINE_EDITOR_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DOutlineEditorInterface :
	public X3DUserInterface
{
public:

	X3DOutlineEditorInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DOutlineEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const
	{ return m_IconFactory; }

	Gtk::Menu &
	getPopupMenu () const
	{ return *m_PopupMenu; }

	Gtk::MenuItem &
	getSetAsCurrentSceneMenuItem () const
	{ return *m_SetAsCurrentSceneMenuItem; }

	Gtk::SeparatorMenuItem &
	getSetAsCurrentSceneSeparatorMenuItem () const
	{ return *m_SetAsCurrentSceneSeparatorMenuItem; }

	Gtk::MenuItem &
	getCreateInstanceMenuItem () const
	{ return *m_CreateInstanceMenuItem; }

	Gtk::MenuItem &
	getCreateReferenceMenuItem () const
	{ return *m_CreateReferenceMenuItem; }

	Gtk::Menu &
	getCreateReferenceMenu () const
	{ return *m_CreateReferenceMenu; }

	Gtk::MenuItem &
	getRemoveReferenceMenuItem () const
	{ return *m_RemoveReferenceMenuItem; }

	Gtk::Menu &
	getRemoveReferenceMenu () const
	{ return *m_RemoveReferenceMenu; }

	Gtk::MenuItem &
	getUnlinkCloneMenuItem () const
	{ return *m_UnlinkCloneMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem () const
	{ return *m_CreateParentGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentTransformMenuItem () const
	{ return *m_CreateParentTransformMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem1 () const
	{ return *m_CreateParentGroupMenuItem1; }

	Gtk::MenuItem &
	getCreateParentStaticGroupMenuItem () const
	{ return *m_CreateParentStaticGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentSwitchMenuItem () const
	{ return *m_CreateParentSwitchMenuItem; }

	Gtk::MenuItem &
	getCreateParentBillboardMenuItem () const
	{ return *m_CreateParentBillboardMenuItem; }

	Gtk::MenuItem &
	getCreateParentCollisionMenuItem () const
	{ return *m_CreateParentCollisionMenuItem; }

	Gtk::MenuItem &
	getCreateParentLODMenuItem () const
	{ return *m_CreateParentLODMenuItem; }

	Gtk::MenuItem &
	getCreateParentAnchorMenuItem () const
	{ return *m_CreateParentAnchorMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayoutLayerMenuItem () const
	{ return *m_CreateParentLayoutLayerMenuItem; }

	Gtk::MenuItem &
	getCreateParentScreenGroupMenuItem () const
	{ return *m_CreateParentScreenGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayoutGroupMenuItem () const
	{ return *m_CreateParentLayoutGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentGeoTransformMenuItem () const
	{ return *m_CreateParentGeoTransformMenuItem; }

	Gtk::MenuItem &
	getCreateParentGeoLocationMenuItem () const
	{ return *m_CreateParentGeoLocationMenuItem; }

	Gtk::MenuItem &
	getCreateParentCADPartMenuItem () const
	{ return *m_CreateParentCADPartMenuItem; }

	Gtk::MenuItem &
	getCreateParentCADAssemblyMenuItem () const
	{ return *m_CreateParentCADAssemblyMenuItem; }

	Gtk::MenuItem &
	getCreateParentCADLayerMenuItem () const
	{ return *m_CreateParentCADLayerMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayerSetMenuItem () const
	{ return *m_CreateParentLayerSetMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayerMenuItem () const
	{ return *m_CreateParentLayerMenuItem; }

	Gtk::MenuItem &
	getCreateParentViewportMenuItem () const
	{ return *m_CreateParentViewportMenuItem; }

	Gtk::MenuItem &
	getRemoveParentMenuItem () const
	{ return *m_RemoveParentMenuItem; }

	Gtk::ImageMenuItem &
	getRemoveMenuItem () const
	{ return *m_RemoveMenuItem; }

	Gtk::MenuItem &
	getViewMenuItem () const
	{ return *m_ViewMenuItem; }

	Gtk::CheckMenuItem &
	getShowExternProtosMenuItem () const
	{ return *m_ShowExternProtosMenuItem; }

	Gtk::CheckMenuItem &
	getShowPrototypesMenuItem () const
	{ return *m_ShowPrototypesMenuItem; }

	Gtk::CheckMenuItem &
	getShowImportedNodesMenuItem () const
	{ return *m_ShowImportedNodesMenuItem; }

	Gtk::CheckMenuItem &
	getShowExportedNodesMenuItem () const
	{ return *m_ShowExportedNodesMenuItem; }

	Gtk::CheckMenuItem &
	getExpandExternProtosMenuItem () const
	{ return *m_ExpandExternProtosMenuItem; }

	Gtk::CheckMenuItem &
	getExpandPrototypeInstancesMenuItem () const
	{ return *m_ExpandPrototypeInstancesMenuItem; }

	Gtk::CheckMenuItem &
	getExpandInlineNodesMenuItem () const
	{ return *m_ExpandInlineNodesMenuItem; }

	Gtk::CheckMenuItem &
	getUseLocaleMenuItem () const
	{ return *m_UseLocaleMenuItem; }

	Gtk::Menu &
	getSceneMenu () const
	{ return *m_SceneMenu; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getSceneMenuBox () const
	{ return *m_SceneMenuBox; }

	Gtk::MenuButton &
	getSceneMenuButton () const
	{ return *m_SceneMenuButton; }

	Gtk::Image &
	getSceneImage () const
	{ return *m_SceneImage; }

	Gtk::Label &
	getSceneLabel () const
	{ return *m_SceneLabel; }

	Gtk::Button &
	getPreviousSceneButton () const
	{ return *m_PreviousSceneButton; }

	Gtk::Button &
	getNextSceneButton () const
	{ return *m_NextSceneButton; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	virtual
	void
	on_set_as_current_scene_activate () = 0;

	virtual
	void
	on_create_instance_activate () = 0;

	virtual
	void
	on_unlink_clone_activate () = 0;

	virtual
	void
	on_create_parent_transform_activate () = 0;

	virtual
	void
	on_create_parent_group_activate () = 0;

	virtual
	void
	on_create_parent_static_group_activate () = 0;

	virtual
	void
	on_create_parent_switch_activate () = 0;

	virtual
	void
	on_create_parent_billboard_activate () = 0;

	virtual
	void
	on_create_parent_collision_activate () = 0;

	virtual
	void
	on_create_parent_lod_activate () = 0;

	virtual
	void
	on_create_parent_anchor_activate () = 0;

	virtual
	void
	on_create_parent_layout_layer_activate () = 0;

	virtual
	void
	on_create_parent_screen_group_activate () = 0;

	virtual
	void
	on_create_parent_layout_group_activate () = 0;

	virtual
	void
	on_create_parent_geo_transform_activate () = 0;

	virtual
	void
	on_create_parent_geo_location_activate () = 0;

	virtual
	void
	on_create_parent_cad_part_activate () = 0;

	virtual
	void
	on_create_parent_cad_assembly_activate () = 0;

	virtual
	void
	on_create_parent_cad_layer_activate () = 0;

	virtual
	void
	on_create_parent_layer_set_activate () = 0;

	virtual
	void
	on_create_parent_layer_activate () = 0;

	virtual
	void
	on_create_parent_viewport_activate () = 0;

	virtual
	void
	on_remove_parent_activate () = 0;

	virtual
	void
	on_remove_activate () = 0;

	virtual
	void
	on_show_extern_protos_toggled () = 0;

	virtual
	void
	on_show_prototypes_toggled () = 0;

	virtual
	void
	on_show_imported_nodes_toggled () = 0;

	virtual
	void
	on_show_exported_nodes_toggled () = 0;

	virtual
	void
	on_expand_extern_protos_toggled () = 0;

	virtual
	void
	on_expand_prototype_instances_toggled () = 0;

	virtual
	void
	on_expand_inline_nodes_toggled () = 0;

	virtual
	void
	on_use_locale_menu_item_toggled () = 0;

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_previous_scene_clicked () = 0;

	virtual
	void
	on_next_scene_clicked () = 0;

	virtual
	bool
	on_button_press_event (GdkEventButton* event) = 0;

	virtual
	~X3DOutlineEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                     filename;
	Glib::RefPtr <Gtk::Builder>     m_builder;
	Glib::RefPtr <Gtk::IconFactory> m_IconFactory;
	Gtk::Menu*                      m_PopupMenu;
	Gtk::MenuItem*                  m_SetAsCurrentSceneMenuItem;
	Gtk::SeparatorMenuItem*         m_SetAsCurrentSceneSeparatorMenuItem;
	Gtk::MenuItem*                  m_CreateInstanceMenuItem;
	Gtk::MenuItem*                  m_CreateReferenceMenuItem;
	Gtk::Menu*                      m_CreateReferenceMenu;
	Gtk::MenuItem*                  m_RemoveReferenceMenuItem;
	Gtk::Menu*                      m_RemoveReferenceMenu;
	Gtk::MenuItem*                  m_UnlinkCloneMenuItem;
	Gtk::MenuItem*                  m_CreateParentGroupMenuItem;
	Gtk::MenuItem*                  m_CreateParentTransformMenuItem;
	Gtk::MenuItem*                  m_CreateParentGroupMenuItem1;
	Gtk::MenuItem*                  m_CreateParentStaticGroupMenuItem;
	Gtk::MenuItem*                  m_CreateParentSwitchMenuItem;
	Gtk::MenuItem*                  m_CreateParentBillboardMenuItem;
	Gtk::MenuItem*                  m_CreateParentCollisionMenuItem;
	Gtk::MenuItem*                  m_CreateParentLODMenuItem;
	Gtk::MenuItem*                  m_CreateParentAnchorMenuItem;
	Gtk::MenuItem*                  m_CreateParentLayoutLayerMenuItem;
	Gtk::MenuItem*                  m_CreateParentScreenGroupMenuItem;
	Gtk::MenuItem*                  m_CreateParentLayoutGroupMenuItem;
	Gtk::MenuItem*                  m_CreateParentGeoTransformMenuItem;
	Gtk::MenuItem*                  m_CreateParentGeoLocationMenuItem;
	Gtk::MenuItem*                  m_CreateParentCADPartMenuItem;
	Gtk::MenuItem*                  m_CreateParentCADAssemblyMenuItem;
	Gtk::MenuItem*                  m_CreateParentCADLayerMenuItem;
	Gtk::MenuItem*                  m_CreateParentLayerSetMenuItem;
	Gtk::MenuItem*                  m_CreateParentLayerMenuItem;
	Gtk::MenuItem*                  m_CreateParentViewportMenuItem;
	Gtk::MenuItem*                  m_RemoveParentMenuItem;
	Gtk::ImageMenuItem*             m_RemoveMenuItem;
	Gtk::MenuItem*                  m_ViewMenuItem;
	Gtk::CheckMenuItem*             m_ShowExternProtosMenuItem;
	Gtk::CheckMenuItem*             m_ShowPrototypesMenuItem;
	Gtk::CheckMenuItem*             m_ShowImportedNodesMenuItem;
	Gtk::CheckMenuItem*             m_ShowExportedNodesMenuItem;
	Gtk::CheckMenuItem*             m_ExpandExternProtosMenuItem;
	Gtk::CheckMenuItem*             m_ExpandPrototypeInstancesMenuItem;
	Gtk::CheckMenuItem*             m_ExpandInlineNodesMenuItem;
	Gtk::CheckMenuItem*             m_UseLocaleMenuItem;
	Gtk::Menu*                      m_SceneMenu;
	Gtk::Window*                    m_Window;
	Gtk::Box*                       m_Widget;
	Gtk::Box*                       m_SceneMenuBox;
	Gtk::MenuButton*                m_SceneMenuButton;
	Gtk::Image*                     m_SceneImage;
	Gtk::Label*                     m_SceneLabel;
	Gtk::Button*                    m_PreviousSceneButton;
	Gtk::Button*                    m_NextSceneButton;
	Gtk::ScrolledWindow*            m_ScrolledWindow;

};

} // puck
} // titania

#endif

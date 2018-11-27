/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_EDITORS_PROJECTS_EDITOR_X3D_FILE_BROWSER_H__
#define __TITANIA_EDITORS_PROJECTS_EDITOR_X3D_FILE_BROWSER_H__

#include "../../Base/ScrollFreezer.h"
#include "../../Bits/File.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <regex>

namespace titania {
namespace puck {

template <class Type>
class X3DFileBrowser :
	virtual public Type
{
public:

	using Type::getFileStore;
	using Type::getFileView;
	using Type::getFileSelection;
	using Type::getConfig;
	using Type::createDialog;

	///  @name Destruction

	virtual
	~X3DFileBrowser () override;


protected:

	///  @name Member types

	enum class TransferAction
	{
		COPY,
		MOVE,
		LINK
	};

	///  @name Construction

	X3DFileBrowser ();

	virtual
	void
	initialize () override;

	virtual
	void
	configure () override;

	///  @name File operations

	void
	transferFiles (const TransferAction action,
	               const std::vector <Glib::RefPtr <Gio::File>> & files,
	               const Glib::RefPtr <Gio::File> & folder);

	void
	transferFile (const TransferAction action,
	              const Glib::RefPtr <Gio::File> & source,
	              const Glib::RefPtr <Gio::File> & destination);

	///  @name Clipboard handling

	void
	clearClipboard ();
	
	void
	cutItems (const std::vector <Glib::RefPtr <Gio::File>> & file);
	
	void
	copyItems (const std::vector <Glib::RefPtr <Gio::File>> & file);

	void
	pasteIntoFolder (const Glib::RefPtr <Gio::File> & folder);

	const std::vector <Glib::RefPtr <Gio::File>> &
	getClipboard () const
	{ return clipboard; }
	
	///  @name Selection handling

	void
	unselectAll ();

	bool
	isSelected (const Glib::RefPtr <Gio::File> & file) const;

	bool
	selectFile (const Glib::RefPtr <Gio::File> & file, const bool scroll = false);

	std::vector <Glib::RefPtr <Gio::File>>
	getSelectedFiles () const;

	bool
	expandTo (const Glib::RefPtr <Gio::File> & file);

	///  @name Folder handling

	void
	setRootFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	addRootFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	removeRootFolder (const Glib::RefPtr <Gio::File> & folder);

	const std::set <std::string> &
	getRootFolders () const
	{ return rootFolders; }

	Gtk::TreeIter
	getIter (const Glib::RefPtr <Gio::File> & file) const;

	Glib::RefPtr <Gio::File>
	getFile (const Gtk::TreeIter & iter) const;

	///  @name Event handler

	virtual
	bool
	on_test_expand_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path) override;

	virtual
	void
	on_file_changed (const Glib::RefPtr <Gio::File> & file,
	                 const Glib::RefPtr <Gio::File> & other_file,
	                 Gio::FileMonitorEvent event);

	///  @name Expanded handling

	void
	restoreExpanded (std::string id = "");

	void
	saveExpanded (std::string id = "");

	///  @name Destruction

	virtual
	void
	store () override;


private:

	///  Member types

	class FolderElement;

	using FolderElementPtr = std::shared_ptr <FolderElement>;

	class Columns
	{
	public:
	
		static constexpr int ICON      = 0;
		static constexpr int NAME      = 1;
		static constexpr int PATH      = 2;
		static constexpr int SENSITIVE = 3;
	
	};

	void
	on_file_changed_update_tree_view (const Glib::RefPtr <Gio::File> & file);

	///  @name Clipboard handling

	Glib::RefPtr <Gio::File>
	getTransferDestination (const TransferAction action, const Glib::RefPtr <Gio::File> & source, const Glib::RefPtr <Gio::File> & folder) const;

	std::string
	getTransferCopyString (const int32_t count) const;

	///  @name Folder handling

	void
	addFolder (const Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & folder);

	void
	addFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	addChildren (const Gtk::TreeModel::iterator & parent, const Glib::RefPtr <Gio::File> & folder);

	void
	addChild (const Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & file);

	void
	removeChildren (const Gtk::TreeIter & iter);

	void
	removeChild (const Gtk::TreeModel::iterator & iter);
	
	bool
	getIter (const Gtk::TreeIter & iter, const std::string & path, Gtk::TreeIter & result) const;

	///  @name Expanded handling

	void
	getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders);

	///  @name Members

	std::set <std::string>                   rootFolders;
	std::map <std::string, FolderElementPtr> folders;
	std::vector <Glib::RefPtr <Gio::File>>   clipboard;
	TransferAction                           clipboardAction;
	std::unique_ptr <ScrollFreezer>          scrollFreezer;

};

template <class Type>
class X3DFileBrowser <Type>::FolderElement
{
public:

	Glib::RefPtr <Gio::FileMonitor> fileMonitor;
};

template <class Type>
X3DFileBrowser <Type>::X3DFileBrowser () :
	           Type (),
	    rootFolders (),
	        folders (),
	      clipboard (),
	clipboardAction (TransferAction::COPY),
	  scrollFreezer (new ScrollFreezer (getFileView ()))
{ }

template <class Type>
void
X3DFileBrowser <Type>::initialize ()
{ }

template <class Type>
void
X3DFileBrowser <Type>::configure ()
{ }

template <class Type>
bool
X3DFileBrowser <Type>::on_test_expand_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	// Refresh children.

	removeChildren (iter);
	addChildren (iter, getFile (iter));

	// Return false to allow expansion, true to reject.
	return false;
}

template <class Type>
void
X3DFileBrowser <Type>::on_file_changed (const Glib::RefPtr <Gio::File> & file,
                                        const Glib::RefPtr <Gio::File> & other_file,
                                        Gio::FileMonitorEvent event)
{
	try
	{
		switch (event)
		{
			case Gio::FILE_MONITOR_EVENT_DELETED:
			case Gio::FILE_MONITOR_EVENT_CREATED:
			case Gio::FILE_MONITOR_EVENT_MOVED:
			case Gio::FILE_MONITOR_EVENT_RENAMED:
			case Gio::FILE_MONITOR_EVENT_MOVED_IN:
			case Gio::FILE_MONITOR_EVENT_MOVED_OUT:
			{
				// Update tree view
				break;
			}
			case Gio::FILE_MONITOR_EVENT_CHANGED:
			case Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT:
			case Gio::FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED:
			case Gio::FILE_MONITOR_EVENT_PRE_UNMOUNT:
			case Gio::FILE_MONITOR_EVENT_UNMOUNTED:
			{
				// Do nothing.
				return;
			}
		}

		saveExpanded ();

		if (file)
			on_file_changed_update_tree_view (file);

		if (other_file)
			on_file_changed_update_tree_view (other_file);

		restoreExpanded ();
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DFileBrowser <Type>::on_file_changed_update_tree_view (const Glib::RefPtr <Gio::File> & file)
{
	if (not file -> has_parent ())
		return;

	const auto folder = file -> get_parent ();
	const auto iter   = getIter (folder);

	if (not getFileStore () -> iter_is_valid (iter))
		return;

	// Work with path as iter don't remain valid after append and erase.
	const auto path = getFileStore () -> get_path (iter);

	getFileStore () -> insert_after (iter);
	removeChild (iter);
	addFolder (getFileStore () -> get_iter (path), folder);
}

template <class Type>
void
X3DFileBrowser <Type>::transferFiles (const TransferAction action,
                                      const std::vector <Glib::RefPtr <Gio::File>> & files,
                                      const Glib::RefPtr <Gio::File> & folder)
{
	const auto folderInfo = folder -> query_info ();

	if (folderInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
		return;

	auto selection = std::vector <Glib::RefPtr <Gio::File> > ();

	for (const auto & source : files)
	{
		try
		{
			const auto destination = getTransferDestination (action, source, folder);

			transferFile (action, source, destination);

			selection .emplace_back (destination);
		}
		catch (const Glib::Error & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	// Select destinations.

	unselectAll ();

	for (const auto & file : selection)
		selectFile (file);
}

template <class Type>
void
X3DFileBrowser <Type>::transferFile (const TransferAction action,
                                     const Glib::RefPtr <Gio::File> & source,
                                     const Glib::RefPtr <Gio::File> & destination)
{
	try
	{
		const auto sourceInfo      = source -> query_info ();
		const auto sourceDirectory = sourceInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
		const auto sourceUri       = basic::uri (source -> get_uri ());
		const auto destinationUri  = basic::uri (destination -> get_uri ());

		if (action == TransferAction::MOVE)
		{
			if (source -> get_path () == destination -> get_path ())
				return;
		}

		if (not destination -> has_parent ())
			return;

		if (destination -> query_exists ())
		{
			const auto dialog = this -> template createDialog <MessageDialog> ("MessageDialog");
			const auto folder = destination -> get_parent ();

			dialog -> setType (Gtk::MESSAGE_QUESTION);

			if (sourceDirectory)
			{
				dialog -> setMessage (basic::sprintf (_ ("Replace directory «%s¢?"), destination -> get_basename () .c_str ()));
				dialog -> setText (basic::sprintf (_ ("A directory with the same name already exists in ?%s?. Replacing it will overwrite its content."), folder -> get_basename () .c_str ()));
			}
			else
			{
				dialog -> setMessage (basic::sprintf (_ ("Replace file »%s«?"), destination -> get_basename () .c_str ()));
				dialog -> setText (basic::sprintf (_ ("Another file with the same name already exists in »%s«. Replacing it will overwrite its content."), folder -> get_basename () .c_str ()));
			}

			if (dialog -> run () not_eq Gtk::RESPONSE_OK)
				return;

			File::removeFile (destination);
		}

		if (sourceDirectory)
		{
			if (File::isSubfolder (destination, source))
			{
				const auto dialog = this -> template createDialog <MessageDialog> ("MessageDialog");

				dialog -> setType (Gtk::MESSAGE_ERROR);
				dialog -> setMessage (_ ("You cannot copy or move a folder into itself!"));
				dialog -> setText (_ ("The destination folder is inside the source folder."));
				dialog -> run ();

				return;
			}
		}

		switch (action)
		{
			case TransferAction::COPY:
			{
				auto flags = Gio::FILE_COPY_OVERWRITE;

				if (sourceUri .is_local () and destinationUri .is_local ())
					flags |= Gio::FILE_COPY_NOFOLLOW_SYMLINKS;

				File::copyFile (source, destination, flags);

				on_file_changed (destination, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);
				break;
			}
			case TransferAction::MOVE:
			{
				try
				{
					source -> move (destination, Gio::FILE_COPY_OVERWRITE | Gio::FILE_COPY_NOFOLLOW_SYMLINKS);
				}
				catch (const Gio::Error & error)
				{
					auto flags = Gio::FILE_COPY_OVERWRITE;

					if (sourceUri .is_local () and destinationUri .is_local ())
						flags |= Gio::FILE_COPY_NOFOLLOW_SYMLINKS;

					File::copyFile (source, destination, flags);
					//File::removeFile (source);
				}

				on_file_changed (source, destination, Gio::FILE_MONITOR_EVENT_MOVED);
				break;
			}
			case TransferAction::LINK:
			{
				destination -> make_symbolic_link (source -> get_path ());
				break;
			}
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
Glib::RefPtr <Gio::File>
X3DFileBrowser <Type>::getTransferDestination (const TransferAction action, const Glib::RefPtr <Gio::File> & source, const Glib::RefPtr <Gio::File> & folder) const
{
	static const std::regex pattern (_ (R"(\s*\((?:copy|another copy|\d+\.\s+copy)\)\s*$)"));

	if (action == TransferAction::COPY and source -> get_parent () -> get_uri () == folder -> get_uri ())
	{
		auto    basename = basic::uri (source -> get_basename ());
		auto    name     = std::regex_replace (basename .stem (), pattern, "");
		auto    extension   = basename .extension ();
		int32_t copy     = 0;
		auto    child    = Glib::RefPtr <Gio::File> ();
	
		do
		{
			child = folder -> get_child (name + getTransferCopyString (copy) + extension);
			copy += 1;
		}
		while (child -> query_exists ());
	
		return child;
	}
	else
	{
		return folder -> get_child (source -> get_basename ());
	}
}

template <class Type>
std::string
X3DFileBrowser <Type>::getTransferCopyString (const int32_t count) const
{
	switch (count)
	{
		case 0:
			return "";
		case 1:
			return _ (" (copy)");
		case 2:
			return _ (" (another copy)");
		default:
			return basic::sprintf (_(" (%d. copy)"), count);
	}
}

template <class Type>
void
X3DFileBrowser <Type>::clearClipboard ()
{
	for (const auto & file : clipboard)
	{
		const auto & iter = getIter (file);

		if (not getFileStore () -> iter_is_valid (iter))
			continue;

		iter -> set_value (Columns::SENSITIVE, true);
	}

	clipboard .clear ();
}

template <class Type>
void
X3DFileBrowser <Type>::cutItems (const std::vector <Glib::RefPtr <Gio::File>> & files)
{
	clearClipboard ();

	for (const auto & file : files)
	{
		getIter (file) -> set_value (Columns::SENSITIVE, false);

		clipboard .emplace_back (file);
	}

	clipboardAction = TransferAction::MOVE;
}

template <class Type>
void
X3DFileBrowser <Type>::copyItems (const std::vector <Glib::RefPtr <Gio::File>> & files)
{
	clearClipboard ();

	for (const auto & file : files)
		clipboard .emplace_back (file);

	clipboardAction = TransferAction::COPY;
}

template <class Type>
void
X3DFileBrowser <Type>::pasteIntoFolder (const Glib::RefPtr <Gio::File> & folder)
{
	transferFiles (clipboardAction, clipboard, folder);

	if (clipboardAction == TransferAction::MOVE)
		clearClipboard ();
}

template <class Type>
void
X3DFileBrowser <Type>::unselectAll ()
{
	getFileSelection () -> unselect_all ();
}

template <class Type>
bool
X3DFileBrowser <Type>::isSelected (const Glib::RefPtr <Gio::File> & file) const
{
	for (const auto & selectedFile : getSelectedFiles ())
	{
		if (selectedFile -> get_path () == file -> get_path ())
			return true;
	}

	return false;
}

template <class Type>
bool
X3DFileBrowser <Type>::selectFile (const Glib::RefPtr <Gio::File> & file, const bool scroll)
{
	using ScrollToRow = void (Gtk::TreeView::*) (const Gtk::TreePath &, float);

	if (not expandTo (file))
		return false;

	const auto iter = getIter (file);
	const auto path = getFileStore () -> get_path (iter);

	getFileSelection () -> select (iter);

	if (scroll)
		Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (getFileView (), (ScrollToRow) &Gtk::TreeView::scroll_to_row), path, 2 - math::phi <double>));

	return true;
}

template <class Type>
std::vector <Glib::RefPtr <Gio::File>>
X3DFileBrowser <Type>::getSelectedFiles () const
{
	std::vector <Glib::RefPtr <Gio::File>> selectedFiles;

	for (const auto & path : getFileSelection () -> get_selected_rows ())
		selectedFiles .emplace_back (getFile (getFileStore () -> get_iter (path)));

	return selectedFiles;
}

template <class Type>
bool
X3DFileBrowser <Type>::expandTo (const Glib::RefPtr <Gio::File> & file)
{
	if (rootFolders .count (file -> get_path ()))
		return true;

	if (not file -> has_parent ())
		return false;

	const auto folder = file -> get_parent ();

	if (expandTo (folder))
	{
		getFileView () .expand_row (getFileStore () -> get_path (getIter (folder)), false);
		return true;
	}

	return false;
}

template <class Type>
void
X3DFileBrowser <Type>::setRootFolder (const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		getFileStore () -> clear ();
		rootFolders .clear ();
		folders .clear ();

		rootFolders .emplace (folder -> get_path ());

		for (const auto & fileInfo : File::getChildren (folder))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getFileStore () -> append ();
	
					addFolder (iter, child);
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getFileStore () -> append ();
	
					addChild (iter, child);
					continue;
				}
				default:
					continue;
			}
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DFileBrowser <Type>::addRootFolder (const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		if (not rootFolders .emplace (folder -> get_path ()) .second)
			return;

		const auto iter = getFileStore () -> append ();

		if (folder -> query_exists () and folder -> query_info () -> get_file_type () == Gio::FILE_TYPE_DIRECTORY)
		{
			addFolder (iter, folder);
		}
		else
		{
			const auto uri = basic::uri (folder -> get_uri ());

			if (uri .is_local ())
			{
				iter -> set_value (Columns::ICON, std::string ("folder-symbolic"));
				iter -> set_value (Columns::NAME, folder -> get_basename ());
			}
			else
			{
				iter -> set_value (Columns::ICON, std::string ("folder-remote-symbolic"));
				iter -> set_value (Columns::NAME, folder -> get_basename () + " (" + uri .authority () + ")");
			}

			iter -> set_value (Columns::PATH, folder -> get_path ());
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DFileBrowser <Type>::addFolder (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & folder)
{
	addFolder (folder);
	addChild (iter, folder);

	if (File::hasChildren (folder))
		getFileStore () -> append (iter -> children ());
}

template <class Type>
void
X3DFileBrowser <Type>::addFolder (const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		const auto fileElement = std::make_shared <FolderElement> ();
	
		folders .emplace (folder -> get_path (), fileElement);
	
		fileElement -> fileMonitor = folder -> monitor_directory (Gio::FILE_MONITOR_WATCH_MOVES);
	
		fileElement -> fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &X3DFileBrowser::on_file_changed));
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DFileBrowser <Type>::addChildren (const Gtk::TreeIter & parentIter, const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		for (const auto & fileInfo : File::getChildren (folder))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getFileStore () -> append (parentIter -> children ());

					addFolder (iter, child);
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getFileStore () -> append (parentIter -> children ());

					addChild (iter, child);
					continue;
				}
				default:
					continue;
			}
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DFileBrowser <Type>::addChild (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & file)
{
	const auto fileInfo = file -> query_info ();
	const auto uri      = basic::uri (file -> get_uri ());

	if (fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY and not uri .is_local ())
		iter -> set_value (Columns::ICON, std::string ("folder-remote"));
	else
		iter -> set_value (Columns::ICON, File::getIconName (fileInfo));

	if (uri .is_local () or not rootFolders .count (file -> get_path ()))
		iter -> set_value (Columns::NAME, file -> get_basename ());
	else
		iter -> set_value (Columns::NAME, file -> get_basename () + " (" + uri .authority () + ")");

	iter -> set_value (Columns::PATH,      file -> get_path ());
	iter -> set_value (Columns::SENSITIVE, true);
}

template <class Type>
void
X3DFileBrowser <Type>::removeRootFolder (const Glib::RefPtr <Gio::File> & folder)
{
	if (rootFolders .erase (folder -> get_path ()))
		removeChild (getIter (folder));
}

template <class Type>
void
X3DFileBrowser <Type>::removeChildren (const Gtk::TreeIter & iter)
{
	std::vector <Gtk::TreePath> children;

	for (const auto & child : iter -> children ())
		children .emplace_back (getFileStore () -> get_path (child));

	for (const auto & child : basic::make_reverse_range (children))
		removeChild (getFileStore () -> get_iter (child));
}

template <class Type>
void
X3DFileBrowser <Type>::removeChild (const Gtk::TreeIter & iter)
{
	const auto child = getFile (iter);

	getFileStore () -> erase (iter);

	if (child -> get_path () .empty ())
		return;

	// Remove project and subfolders if path is a folder.

	const auto fileInfo = child -> query_info ();

	if (fileInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
		return;

	// Remove subfolders.

	std::vector <std::string> subfolders;

	for (const auto & pair : folders)
	{
		if (File::isSubfolder (Gio::File::create_for_path (pair .first), child))
			subfolders .emplace_back (pair .first);
	}

	for (const auto & subfolder : subfolders)
		folders .erase (subfolder);
}

template <class Type>
Gtk::TreeIter
X3DFileBrowser <Type>::getIter (const Glib::RefPtr <Gio::File> & file) const
{
	Gtk::TreeIter result;

	for (const auto & childIter : getFileStore () -> children ())
	{
		if (getIter (childIter, file -> get_path (), result))
			break;
	}

	return result;
}

template <class Type>
bool
X3DFileBrowser <Type>::getIter (const Gtk::TreeIter & iter, const std::string & path, Gtk::TreeIter & result) const
{
	const auto parent = getFile (iter) -> get_path ();

	if (parent == path)
	{
		result = iter;
		return true;
	}

	if (path .find (parent) not_eq 0)
		return false;

	for (const auto & childIter : iter -> children ())
	{
		if (getIter (childIter, path, result))
			return true;
	}

	return false;
}

template <class Type>
Glib::RefPtr <Gio::File>
X3DFileBrowser <Type>::getFile (const Gtk::TreeIter & iter) const
{
	std::string path;

	iter -> get_value (Columns::PATH, path);

	return Gio::File::create_for_path (path);
}

template <class Type>
void
X3DFileBrowser <Type>::restoreExpanded (std::string id)
{
	if (not id .empty ())
		id = '.' + id;

	const auto folders = getConfig () -> template getItem <X3D::MFString> ("expanded" + id);

	for (const auto & folder : folders)
	{
		const auto iter = getIter (Gio::File::create_for_path (folder));

		if (not getFileStore () -> iter_is_valid (iter))
			continue;

		getFileView () .expand_row (getFileStore () -> get_path (iter), false);
	}

	scrollFreezer -> restore (getConfig () -> template getItem <double> ("hadjustment"),
	                          getConfig () -> template getItem <double> ("vadjustment"));
}

template <class Type>
void
X3DFileBrowser <Type>::saveExpanded (std::string id)
{
	if (not id .empty ())
		id = '.' + id;

	X3D::MFString folders;

	getExpanded (getFileStore () -> children (), folders);

	getConfig () -> template setItem <X3D::MFString> ("expanded" + id, folders);
	getConfig () -> template setItem <double> ("hadjustment", getFileView () .get_hadjustment () -> get_value ());
	getConfig () -> template setItem <double> ("vadjustment", getFileView () .get_vadjustment () -> get_value ());
}

template <class Type>
void
X3DFileBrowser <Type>::getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders)
{
	for (const auto & child : children)
	{
		if (getFileView () .row_expanded (getFileStore () -> get_path (child)))
			folders .emplace_back (getFile (child) -> get_path ());

		getExpanded (child -> children (), folders);
	}
}

template <class Type>
void
X3DFileBrowser <Type>::store ()
{ }

template <class Type>
X3DFileBrowser <Type>::~X3DFileBrowser ()
{ }

} // puck
} // titania

#endif

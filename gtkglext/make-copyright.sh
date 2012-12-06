#!/bin/sh
#
# Basic idea is shamelessly ripped from gtk+/makecopyright.
#
# Naofumi Yasufuku <naofumi@users.sourceforge.net>
#

exclude_files="config.h glext.h glxext.h wglext.h docs examples"

header_files=`find . -name '*.h' -print`
source_files=`find . -name '*.c' -print`

target_files="$header_files $source_files"


copyright_gdk ()
{
  cat << EOF
/* GdkGLExt - OpenGL Extension to GDK
 * Copyright (C) 2002-2004  Naofumi Yasufuku
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA.
 */

EOF
}

copyright_gdk_glext ()
{
  copyright_gdk
}

copyright_gdk_x11 ()
{
  copyright_gdk
}

copyright_gdk_win32 ()
{
  copyright_gdk
}

copyright_gtk ()
{
  cat << EOF
/* GtkGLExt - OpenGL Extension to GTK+
 * Copyright (C) 2002-2004  Naofumi Yasufuku
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA.
 */

EOF
}


for file in $target_files; do

  exclude=""
  for file_exclude in bak $exclude_files; do
    exclude=`echo $file | grep $file_exclude`
    if test -n "$exclude"; then
      continue 2
    fi
  done

  dir=`dirname $file`
  subdir=`echo ${dir#*/} | sed -e 's/\//_/g'`

  if test -z "$subdir"; then
    continue
  fi

  src_header=`copyright_$subdir | head -1`
  dst_header=`cat $file | grep "$src_header" | head -1`

  src_copyright=`copyright_$subdir | grep Copyright | head -1 | \
                 sed -e 's/^.*Copyright/Copyright/'`
  dst_copyright=`cat $file | grep Copyright | head -1 | \
                 sed -e 's/^.*Copyright/Copyright/'`

  if test "x$dst_header" != "x$src_header" -o \
          "x$dst_copyright" != "x$src_copyright"; then
    backup_dir="$dir/bak"
    if test ! -d $backup_dir; then
      echo -n "Making directory ... "
      mkdir $backup_dir
      echo "$backup_dir"
    fi

    filename=`basename $file`
    echo "Copying $file to $backup_dir/$filename ..."
    cp $file $backup_dir/$filename

    if test "x$dst_header" != "x$src_header"; then
      copyright_$subdir > $file
      cat $backup_dir/$filename >> $file
    else
      cat $backup_dir/$filename | \
	  sed -e "s/$dst_copyright/$src_copyright/" > $file
    fi

    echo "$file is updated"

  fi

done

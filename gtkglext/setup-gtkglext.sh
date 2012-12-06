#!/bin/sh
#
# Setup GtkGLExt win32 binary package.
#

PKGCONFIG_FILES="lib/pkgconfig/gdkglext-3.0.pc lib/pkgconfig/gdkglext-win32-3.0.pc lib/pkgconfig/gtkglext-3.0.pc lib/pkgconfig/gtkglext-win32-3.0.pc"

PREFIX=""

case $# in
1)
    PREFIX="$1"
    ;;
*)
    echo ""
    echo "Usage: ${0#*/} <unpacked prefix dir>"
    echo ""
    echo "Example: ./${0#*/} c:/gtkglext"
    echo ""
    exit
    ;;
esac

#
# Update .pc files
#
for file in ${PKGCONFIG_FILES}; do

    src="${PREFIX}/${file}.orig-$$"
    dst="${PREFIX}/${file}"

    echo -n "${dst}: "

    if test ! -f ${dst}; then
        echo "no such file"
        exit
    fi

    cp ${dst} ${src}
    cat ${src} | sed -e "s|^prefix=.*|prefix=${PREFIX}|" > ${dst}

    echo "updated"

done

echo "done."

echo ""
echo "PATH:            ${PREFIX}/bin"
echo "PKG_CONFIG_PATH: ${PREFIX}/lib/pkgconfig"
echo ""

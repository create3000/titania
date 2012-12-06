#!/bin/sh
#
# gen-def-file.sh:
# Shell script to generate .def file.
#

DLL_FILE=""
DLLTOOL=dlltool

case $# in
1)
    DLL_FILE="$1"
    ;;
*)
    echo "Usage: ${0#*/} <DLL file>"
    exit
    ;;
esac

TMP="./tmp-$$.def"

$DLLTOOL --output-def $TMP --export-all-symbols $DLL_FILE

echo EXPORTS

cat $TMP | \
    grep -v "$TMP" | \
    grep gtk | \
    grep gl | \
    grep -v _gtk_gl | \
    sed -e 's| \@.*||g'

rm -f $TMP

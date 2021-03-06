#!/bin/sh

rm kdepackages.h
curl "http://bugs.kde.org/xml.cgi?data=versiontable" > tmp_kdepackages_download
echo "// DO NOT EDIT - EDIT bugs/Maintainers instead" >  kdepackages.h
echo "const char * const packages[] = {" >> kdepackages.h
cat tmp_kdepackages_download | egrep "<product|<component" | grep -v "</product>" | \
    sed -e "s,<product name=\"\([^\"]*\)\">,product=\"\1\"; component=\"general\",; \
    s#<component>\([^<]*\)</component>#component=\"\1\"#"  > tmp_kdepackages_download2
(while read i; do eval $i; echo "\"$product/$component\","; done) < tmp_kdepackages_download2 | \
    sed -e "s,/general\",/00general\"," | LC_ALL=C sort -u | \
    grep -v 'valgrind/' | sed -e "s,/00general\",\"," >> kdepackages.h
echo "0 };" >>  kdepackages.h
rm tmp_kdepackages_download2 tmp_kdepackages_download

#!/bin/sh
[ -f /etc/preremove/gcc-mingw-core-manifest.lst ] || exit 0
echo "*** Removing gcc-mingw-core files.  Please wait. ***"
cd /usr
(while read LN
do
  if [ -e $LN -o -h $LN ]
  then
    if [ -f $LN -o -h $LN ]
    then
      echo rm -f ${LN}
      rm -f $LN 2>/dev/null
      if [ $? -ne 0 ]
      then
        echo "File or symbolic link ${LN} can't be removed."
      fi
    elif [ -d $LN ]
    then
      echo rmdir $LN
      rmdir $LN 2>/dev/null
      if [ $? -ne 0 ]
      then
        echo "Directory ${LN} is not empty; skipping."
      fi
    fi
  else
    echo "${LN} already removed."
  fi
done) < /etc/preremove/gcc-mingw-core-manifest.lst
echo 'rm -f /etc/preremove/gcc-mingw-core-manifest.lst'
rm -f /etc/preremove/gcc-mingw-core-manifest.lst

echo "*** Removing symbolic links for runtime files ***"
cd /usr/i686-pc-mingw32/lib
for f in CRT_fp10.o CRT_fp8.o CRT_noglob.o binmode.o \
	crt1.o crt2.o crtmt.o crtst.o dllcrt1.o dllcrt2.o \
	gcrt1.o gcrt2.o txtmode.o
do   
  if [ -h $f ]
  then
    echo rm -f "${f}"  
    rm -f $f
  fi
done


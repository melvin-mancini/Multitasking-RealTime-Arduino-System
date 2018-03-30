#!/bin/bash

PATH=/bin
export PATH

prefix=/usr
sbindir=${prefix}/sbin
bindir=${prefix}/bin

manifest=/etc/preremove/rxvt-manifest.lst

[ -f ${manifest} ] || (echo "Unable to find manifest file" && exit 0)

while read f; do
    fSrc=/etc/defaults/${f}
    fDest=/${f}
    # only delete "real" configuration file if same as default
    if [ -e ${fDest} -o -L ${fDest} ]; then
        if [ -e ${fSrc} -o -L ${fSrc} ] ; then
            if /bin/cmp ${fSrc} ${fDest} >/dev/null ; then
                /bin/rm -f ${fDest}
            else
                echo "${fDest} has been customized; will not delete."
            fi
        fi
    fi
    # always delete default configuration file
    if [ -e ${fSrc} -o -L ${fSrc} ]; then
        /bin/rm ${fSrc}
    fi
done < ${manifest}

rm -f "$(cygpath -AP)/Cygwin/rxvt-native.lnk"
rm -f "$(cygpath -AP)/Cygwin/rxvt-x.lnk"
rmdir --ignore-fail-on-non-empty "$(cygpath -AP)/Cygwin"


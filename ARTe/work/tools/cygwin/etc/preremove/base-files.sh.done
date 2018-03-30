#!/usr/bin/sh
# base-files version 4.0-6

MANIFEST="/etc/preremove/base-files-manifest.lst"

[ -f ${MANIFEST} ] || (echo "Unable to find manifest file." && exit 1)

echo "*** Removing unmodified base files."
echo "*** These will be updated by the postinstall script."
echo "*** Please wait."

while read f; do
  /usr/bin/cmp -s "/${f}" "/etc/defaults/${f}" && \
    (echo "/${f} hasn't been modified, it will be updated." ; \
     /usr/bin/rm -f "/${f}")
done < ${MANIFEST}

exit 0

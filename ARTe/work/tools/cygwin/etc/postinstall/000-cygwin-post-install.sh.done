#!/bin/bash
#
# Copyright 2008, 2009, 2010, 2012, 2013 Red Hat, Inc.
#
# This file is part of Cygwin.
#
# This software is a copyrighted work licensed under the terms of the
# Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
# details.
#
export PATH="/bin:$PATH"

# Uncomment for testing
# SYSCONFDIR="${PWD}"
SYSCONFDIR="${SYSCONFDIR:=/etc}"

FSTAB="${SYSCONFDIR}/fstab"
MTAB="${SYSCONFDIR}/mtab"
FSTABDIR="${SYSCONFDIR}/fstab.d"

DEVDIR=/dev

print_flags ()
{
  (( $1 & 0x0002 )) && echo -n "binary" || echo -n "text"
  (( $1 & 0x0010 )) && echo -n ",exec"
  (( $1 & 0x0040 )) && echo -n ",cygexec"
  (( $1 & 0x0100 )) && echo -n ",notexec"
}

# Create fstab file if it doesn't exist.
if [ -e "${FSTAB}" -a ! -f "${FSTAB}" ]
then
  # Try to move
  mv -f "${FSTAB}" "${FSTAB}.orig"
  if [ -e "${FSTAB}" -a ! -f "${FSTAB}" ]
  then
    echo
    echo "${FSTAB} is existant but not a file."
    echo "Since this file is specifying the mount points, this might"
    echo "result in unexpected trouble.  Please fix that manually."
    echo
  fi
fi

if [ ! -e "${FSTAB}" ]
then

  # Create fstab default header
  cat > ${FSTAB} << EOF
# For a description of the file format, see the Users Guide
# http://cygwin.com/cygwin-ug-net/using.html#mount-table

# This is default anyway:
none /cygdrive cygdrive binary,posix=0,user 0 0
EOF
fi

# Check for ${FSTABDIR} directory

if [ -e "${FSTABDIR}" -a ! -d "${FSTABDIR}" ]
then 
  # No mercy.  Try to remove.
  rm -f "${FSTABDIR}"
  if [ -e "${FSTABDIR}" -a ! -d "${FSTABDIR}" ]
  then 
    echo
    echo "${FSTABDIR} is existant but not a directory."
    echo "Please fix that manually."
    echo
    exit 1
  fi
fi

# Create it if necessary

if [ ! -e "${FSTABDIR}" ]
then
  mkdir -m 1777 "${FSTABDIR}"
  if [ ! -e "${FSTABDIR}" ]
  then
    echo
    echo "Creating ${FSTABDIR} directory failed."
    echo "Please fix that manually."
    echo
    exit 1
  fi
fi

# Check for ${DEVDIR} directory

if [ -e "${DEVDIR}" -a ! -d "${DEVDIR}" ]
then 
  # No mercy.  Try to remove.
  rm -f "${DEVDIR}"
  if [ -e "${DEVDIR}" -a ! -d "${DEVDIR}" ]
  then 
    echo
    echo "${DEVDIR} is existant but not a directory."
    echo "Please fix that manually, otherwise you WILL get problems."
    echo
    exit 1
  fi
fi

# Create it if necessary

mkdir -m 755 "${DEVDIR}" 2> /dev/null
if [ ! -e "${DEVDIR}" ]
then
  echo
  echo "Creating ${DEVDIR} directory failed."
  echo "Please fix that manually, otherwise you WILL get problems."
  echo
  exit 1
fi

# Check for ${DEVDIR}/shm directory (for POSIX semaphores and POSIX shared mem)

if [ -e "${DEVDIR}/shm" -a ! -d "${DEVDIR}/shm" ]
then
  # No mercy.  Try to remove.
  rm -f "${DEVDIR}/shm"
  if [ -e "${DEVDIR}/shm" -a ! -d "${DEVDIR}/shm" ]
  then 
    echo
    echo "${DEVDIR}/shm is existant but not a directory."
    echo "POSIX semaphores and POSIX shared memory will not work"
    echo
  fi
fi

# Create it if necessary

if [ ! -e "${DEVDIR}/shm" ]
then
  mkdir -m 1777 "${DEVDIR}/shm"
  if [ ! -e "${DEVDIR}/shm" ]
  then
    echo
    echo "Creating ${DEVDIR}/shm directory failed."
    echo "POSIX semaphores and POSIX shared memory will not work"
    echo
  fi
else
  chmod 1777 "${DEVDIR}/shm"
fi

# Check for ${DEVDIR}/mqueue directory (for POSIX message queues)

if [ -e "${DEVDIR}/mqueue" -a ! -d "${DEVDIR}/mqueue" ]
then
  # No mercy.  Try to remove.
  rm -f "${DEVDIR}/mqueue"
  if [ -e "${DEVDIR}/mqueue" -a ! -d "${DEVDIR}/mqueue" ]
  then 
    echo
    echo "${DEVDIR}/mqueue is existant but not a directory."
    echo "POSIX message queues will not work"
    echo
  fi
fi

# Create it if necessary

if [ ! -e "${DEVDIR}/mqueue" ]
then
  mkdir -m 1777 "${DEVDIR}/mqueue"
  if [ ! -e "${DEVDIR}/mqueue" ]
  then
    echo
    echo "Creating ${DEVDIR}/mqueue directory failed."
    echo "POSIX message queues will not work"
    echo
  fi
else
  chmod 1777 "${DEVDIR}/mqueue"
fi

# Create /etc/mtab as symlink to /proc/mounts
[ ! -L "${MTAB}" ] && ln -sf /proc/mounts ${MTAB}

# Create default /etc/passwd and /etc/group files
created_passwd=no
created_group=no

if [ ! -e /etc/passwd -a ! -L /etc/passwd ] ; then
  mkpasswd -l -c > /etc/passwd
  chmod 644 /etc/passwd
  created_passwd=yes
fi

if [ ! -e /etc/group -a ! -L /etc/group ] ; then
  mkgroup -l -c > /etc/group
  chmod 644 /etc/group
  created_group=yes
fi

cp -fp /etc/group /tmp/group.mkgroup && \
( [ -w /etc/group ] || chmod --silent a+w /etc/group ; ) && \
echo "root:S-1-5-32-544:0:" > /etc/group && \
sed -e '/root:S-1-5-32-544:0:/d' /tmp/group.mkgroup >> /etc/group && \
chmod --silent --reference=/etc/passwd /etc/group
rm -f /tmp/group.mkgroup

# Deferred to be sure root group entry exists
[ "$created_passwd" = "yes" ] && chgrp --silent root /etc/passwd
[ "$created_group" = "yes"  ] && chgrp --silent root /etc/group

exit 0

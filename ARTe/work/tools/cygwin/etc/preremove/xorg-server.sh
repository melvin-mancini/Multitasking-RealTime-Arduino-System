if [ -f /etc/X11/system.XWinrc ] && cmp -s /etc/defaults/etc/X11/system.XWinrc /etc/X11/system.XWinrc
then
    rm /etc/X11/system.XWinrc
fi


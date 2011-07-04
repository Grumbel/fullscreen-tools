#! /usr/bin/env python
##
##  decorations.py - Remove or show window borders
##  Copyright (C) 2011 Ingo Ruhnke <grumbel@gmail.com>
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.

from gtk.gdk import *
import gtk.gdk
import time
import sys

if len(sys.argv) != 3:
    print "Usage:", sys.argv[0], "show|hide", "WINDOW"
    print "  WINDOW is either a window id as returned by 'xwininfo' or 'wmctrl -l'"
    print "  or the string ACTIVE for the currently focused window."
    exit(0)
else:
    if sys.argv[2] == "ACTIVE":
        w = window_foreign_new((get_default_root_window().property_get("_NET_ACTIVE_WINDOW")[2][0]))
    else:
        w = window_foreign_new(int(sys.argv[2], 16))

if sys.argv[1] == 'hide':
    w.set_keep_above(True) # to raise it over the panel
    w.set_decorations(0)
    w.set_override_redirect(True)
    gtk.gdk.flush()    
    time.sleep(0.5)
    w.move(1280, 0)
    gtk.gdk.flush()    
elif sys.argv[1] == 'show':
    w.set_keep_above(False) # to raise it over the panel
    w.set_decorations(gtk.gdk.DECOR_ALL)
    w.set_override_redirect(False)
    gtk.gdk.flush()
    time.sleep(0.5)
else:
    print "error: invalid action, must be show|hid"
    exit(0)

# EOF #

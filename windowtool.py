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
import argparse

# make it more swiss army knife:
# --move X,Y
# --resize WxH
# --override-redirect False
# --decorations False
# --window WINDOWID
# --keep-above BOOL
# --keep-below BOOL
#
# --windowed to set defaults for window mode
# --fullscreen to set defaults for fullscreen mode
#
# substr=foobar
# title=""
# <integer>

# print "Usage:", sys.argv[0], "show|hide", "WINDOW"
# print "  WINDOW is either a window id as returned by 'xwininfo' or 'wmctrl -l'"
# print "  or the string ACTIVE for the currently focused window."

def str2bool(str):
    return bool(int(str))

class WindowAction(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        if getattr(namespace, 'override_redirect') == None:
            setattr(namespace, 'override_redirect', False)

        if getattr(namespace, 'decorations') == None:
            setattr(namespace, 'decorations', True)

        if getattr(namespace, 'keep_above') == None:
            setattr(namespace, 'keep_above', False)

        if getattr(namespace, 'keep_below') == None:
            setattr(namespace, 'keep_below', False)

class FullscreenAction(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        if getattr(namespace, 'override_redirect') == None:
            setattr(namespace, 'override_redirect', True)

        if getattr(namespace, 'decorations') == None:
            setattr(namespace, 'decorations', False)

        if getattr(namespace, 'keep_above') == None:
            setattr(namespace, 'keep_above', True)

        if getattr(namespace, 'keep_below') == None:
            setattr(namespace, 'keep_below', False)

argp = argparse.ArgumentParser(description="Move, resize and set properties on X11 window")

detail_group = argp.add_argument_group("Detail Options")
detail_group.add_argument('-o', '--override-redirect', help="Set override redirect flag",
                          type=str2bool, dest="override_redirect")
detail_group.add_argument('-d', '--decorations', help="Hide or show window decorations",
                          type=str2bool, dest="decorations")
detail_group.add_argument('-a', '--keep-above', help="Keep window above others",
                          type=str2bool, dest="keep_above")
detail_group.add_argument('-b', '--keep-below', help="Keep window below others",
                          type=str2bool, dest="keep_below")
detail_group.add_argument('-m', '--move', dest='position', metavar="X,Y",
                          default=None, type=lambda x: [int(x) for x in x.split(",")],
                          help="Move window to position X,Y (default: None)")
detail_group.add_argument('-r', '--resize', dest='size', metavar="WxH",
                          default=None, type=lambda x: [int(x) for x in x.split("x")],
                          help="Resize window to WxH (default: None)")
detail_group.add_argument('windows', metavar='WINDOWID', type=str, nargs='+',
                          help='Apply actions to window with WINDOWID')

quick_group = argp.add_argument_group("Quick Options")
quick_group.add_argument('-w', '--window', action=WindowAction, nargs=0,
                         help="Short cut for window mode")
quick_group.add_argument('-f', '--fullscreen', action=FullscreenAction, nargs=0,
                         help="Short cut for fullscreen mode")

args = argp.parse_args()

for window in args.windows:
    if window == "ACTIVE":
        w = window_foreign_new((get_default_root_window().property_get("_NET_ACTIVE_WINDOW")[2][0]))
    else:
        w = window_foreign_new(int(window, 16))

    if args.keep_above != None:
        print "Setting keep_above:", args.keep_above
        w.set_keep_above(args.keep_above) # to raise it over the panel

    if args.decorations != None:
        print "Setting decorations:", args.decorations
        if args.decorations:
            time.sleep(1)
            w.set_decorations(gtk.gdk.DECOR_ALL)
        else:
            time.sleep(1)
            w.set_decorations(0)

    if args.override_redirect != None:
        w.set_override_redirect(args.override_redirect)
        gtk.gdk.flush()
        time.sleep(0.5)

    if args.position:
        time.sleep(1)
        w.move(args.position[0], args.position[1])

    elif args.keep_above != None:
        w.set_keep_above(args.keep_above) # to raise it over the panel

    gtk.gdk.flush()

# EOF #

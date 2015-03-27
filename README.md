Fullscreen Tools
================

Fullscreen Tools contains a collection of simple tools to force
applications into fullscreen mode by different means. Those are right
now mostly just hacks and might not work without some customization in
the source code. Also no actual resolution switching is provided right
now, use xrandr for that.


Compilation
-----------

For compilation type:

    mkdir build
    cd build
    cmake ..
    make


capturewindow
-------------
Create a window with a simple black background and captures another
applications window in it. The parent window can be resized and put
into fullscreen mode. Useful in cases where an application requires a
resolution or aspect ratio not supported by your monitor or graphics
card and you want to fill the surrounding space with a letter box.


windowtool.py
--------------
Removes window decorations, sets override redirect and moves window to
the top/left of the screen. Useful for applications that don't support
fullscreen properly in multi monitor configurations.


windowhierachy
--------------
Displays the hierachy of windows in X11, mainly useful for debugging
or getting the window id of subwindows.


sdl-hack
--------

sdl-hack is a simple LD_PRLOAD based hack that lets you get between
SDL and a game to override specific behaviour. For example if a game
requests fullscreen mode and provides no option to switch to window
mode, you can force it to be in window mode.

Usage:

    sdl-hack APPNAME


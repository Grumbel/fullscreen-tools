//  capturewindow - capture a window and add a border around it
//  Copyright (C) 2011 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <Xm/MwmUtil.h>
#include <string.h>

unsigned int hex2int(const char* str)
{
  unsigned int value;
  if (sscanf(str, "0x%x", &value) == 1)
  {
    return value;
  }
  else
  {
    throw std::runtime_error("couldn't convert string to integer");
  }
}

void set_decorations(Display *dpy, Window w, unsigned long decorations)
{
  MotifWmHints hints;
  memset(&hints, 0, sizeof(hints));
  hints.flags = MWM_HINTS_DECORATIONS;
  hints.decorations = decorations;

  Atom hints_atom = XInternAtom(dpy, _XA_MOTIF_WM_HINTS, True);

  XChangeProperty(dpy, w,                         
                  hints_atom, hints_atom, 32,                             
                  PropModeReplace,
                  reinterpret_cast<unsigned char*>(&hints),
                  PROP_MOTIF_WM_HINTS_ELEMENTS);
}

void run_app(Window forgein_window)
{
  Display* dpy = XOpenDisplay(NULL);
  
  Window window = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                                      0, 0, 
                                      1024, 768, 
                                      0, // borderwidth
                                      0, // border
                                      0x000000); // background
  
  XSelectInput(dpy, window, 
               KeyPressMask      |
               KeyReleaseMask    |
               ButtonPressMask   |
               ButtonReleaseMask |
               StructureNotifyMask);

  // FIXME: No idea why I have to do all this stuff, but it makes the
  // XReparentWindow() work the first time, otherwise the program has
  // to be started twice to make a successful capture
  XUnmapWindow(dpy, forgein_window);
  set_decorations(dpy, forgein_window, 0);
  usleep(100000);
  XReparentWindow(dpy, forgein_window, window, 0, 0);

  XMapRaised(dpy, window);
  XMapWindow(dpy, forgein_window);

  bool quit = false;
  while(!quit)
  {
    XEvent event;
    XNextEvent (dpy, &event);
     
    switch (event.type)
    {
      case ButtonPress:
        std::cout << "ButtonPress" << std::endl;
        break;

      case ButtonRelease:
        std::cout << "ButtonPress" << std::endl;
        break;

      case KeyPress:
        std::cout << "KeyPress" << std::endl;
        {
          KeySym sym = XLookupKeysym(&event.xkey,0);
          switch (sym)
          {
            case XK_Escape:
              quit = true;
              break;

            default:
              XSendEvent(dpy, forgein_window, True, KeyPressMask, &event);
              break;
          }
        }
        break;

      case KeyRelease:
        std::cout << "KeyRelease" << std::endl;
        // doesn't work with Wine/Gargoyle?
        XSendEvent(dpy, forgein_window, True, KeyReleaseMask, &event);
        break;

      case ConfigureNotify:
        std::cout << "ConfigureNotify: "
                  << event.xconfigure.width << "x" << event.xconfigure.height 
                  << "+" << event.xconfigure.x << "+" << event.xconfigure.y << std::endl;
        {
          XWindowAttributes attr;
          XGetWindowAttributes(dpy, forgein_window, &attr);
          std::cout << " --  " << attr.x << "+" << attr.y << " " << attr.width << "x" << attr.height << std::endl;
          XResizeWindow(dpy, forgein_window, attr.width, event.xconfigure.height);
          XGetWindowAttributes(dpy, forgein_window, &attr);
          XMoveWindow(dpy, forgein_window, 
                      event.xconfigure.width/2  - attr.width/2,
                      event.xconfigure.height/2 - attr.height/2);
          std::cout << " --  " << attr.x << "+" << attr.y << " " << attr.width << "x" << attr.height << std::endl;
        }
        break;

      default:
        std::cout << "unhandled message type" << std::endl;
        break;
    }
  }

  // cleanup
  XDestroyWindow(dpy, window);
  XCloseDisplay(dpy);
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " WINDOWID" << std::endl;
  }
  else
  {
    Window foreign_window = hex2int(argv[1]);
    run_app(foreign_window);
    return 0;
  }
}

/* EOF */

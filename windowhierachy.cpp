//  windowhierachy - Display the X11 window hierachy
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

#include <fmt/format.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void traverse_hierachy(Display* dpy, Window win, int depth = 0)
{
  { // print the window
    XWindowAttributes attr;
    XGetWindowAttributes(dpy, win, &attr);

    std::string title;
    XTextProperty text;
    if (XGetWMName(dpy, win, &text))
    {
      title = (char*)text.value;
    }
    XFree((char*)(text.value));

    fmt::print("{:<35s} {:12s} {:12s}  \"{}\"\n",
               fmt::format("{}Window: 0x{:08x}", std::string(depth*2, ' '), win),
               fmt::format("{},{}", attr.x, attr.y),
               fmt::format("{}x{}", attr.width, attr.height),
               title);
  }

  Window root_return;
  Window parent_return;
  Window* children_return;
  unsigned int nchildren_return;

  XQueryTree(dpy, win, &root_return, &parent_return, &children_return, &nchildren_return);

  for(unsigned int i = 0; i < nchildren_return; ++i)
  {
    traverse_hierachy(dpy, children_return[i], depth+1);
  }

  XFree(children_return);
}

int main(int argc, char** argv)
{
  Display* dpy = XOpenDisplay(NULL);

  traverse_hierachy(dpy, DefaultRootWindow(dpy));

  XCloseDisplay(dpy);

  return 0;
}

/* EOF */

/*
**  sdl-hack - force window mode, resolution other things in SDL games
**  Copyright (C) 2012 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdio.h>
#include <SDL.h>

SDL_Surface* (*orig_SDL_SetVideoMode)(int width, int height, int bpp, Uint32 flags);

void wrap_init(void) __attribute__((constructor));

void wrap_init(void)
{
  orig_SDL_SetVideoMode = dlsym(RTLD_NEXT, "SDL_SetVideoMode");
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags)
{
  printf("SDL_SetVideoMode: %d %d %d %u\n", width, height, bpp, flags);

  if (flags & SDL_FULLSCREEN)
  {
    puts("[sdl-hack] ignoring fullscreen flag");
    flags &= ~SDL_FULLSCREEN;
  }

  return (*orig_SDL_SetVideoMode)(width, height, bpp, flags);
}

/* EOF */

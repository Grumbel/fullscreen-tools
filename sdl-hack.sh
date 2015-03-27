#!/bin/sh

LD_PRELOAD=libsdl-hack.so
export LD_PRELOAD
exec "$@"

# EOF #

## Compiling lib on Windows
A linker error occurs because the default visual studio config uses `/MD` instead of `/MT`, which
is what Godot expects.

https://stackoverflow.com/questions/3007312/resolving-lnk4098-defaultlib-msvcrt-conflicts-with

## Calling into Ruby
https://dev.to/roryo/managing-windows-windows-within-mruby-part-2-creating-a-window-from-mruby-2jbb

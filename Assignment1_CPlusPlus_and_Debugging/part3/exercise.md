Debugging

Edit the following lines to list two things you learned about debugging (a command, a technique, a concept. etc.). This is a "for-credit" exercise, I will not evaluate what I consider "difficult enough", I just want to know you learned or practiced two things about debugging.

1. I run the definition of gdb and what advantages that I can have with gdb.

2. How to use gdb

Useful commands

(1) gcc -g main.c -o main

(2) Ctrl + x + 1
: TUI

(3) Ctrl + x + o
: Switch between TUI (Text User Interface) and terminal

(4) (gdb) set new-console on
: gdb During startup program exited with code 0xc0000142.

(5) n or next
: Move the breakpoint forward

(6) s or step
: into another function like foo

(7) bt or backtrace

(8) br foo or breakpoint foo or br 18

(10) delete 8
: delete 8th breakpoint

(11) info breakpoints

(12) continue
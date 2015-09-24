# %W% %G% %U%
#
# 1992 makefile
#
# Copyright (c) 1992, Landon Curt Noll & Larry Bassel.
# All Rights Reserved.  Permission for personal, educational or non-profit
# use is granted provided this this copyright and notice are included in its
# entirety and remains unaltered.  All other uses must receive prior permission
# from Landon Curt Noll and Larry Bassel.


RM=rm
LN=ln

# flags for ansi compiles
#
# NOTE: Some ANSI compilers make use of -Xa to turn on ANSI mode,
#	others such as gcc may want -ansi, others may want nothing
#	at all.  Adjust the CFLAGS line as needed.
#
# NOTE: Some compilers cannot optimize, or optimize some entries
#	entries incorrectly.  You might want need to turn on -O 
#	to see if your compiler can handle them.
#
#CFLAGS=-O -Xa
CFLAGS=-O3 -march=native -ffast-math -funroll-loops
CC=cc


all: first

buzzard.2: buzzard.2.c
	$(CC) $(CFLAGS) buzzard.2.c -o buzzard.2

first: buzzard.2
	$(RM) -f first
	$(LN) buzzard.2 first

test: first
	cat demo1.1st | ./first
	cat third help.th | ./first
	cat third demo1.th | ./first
	cat third demo2.th | ./first
	cat third demo3.th | ./first
	cat third demo4.th | ./first
	cat third demo5.th | ./first
	cat third demo6.th | ./first

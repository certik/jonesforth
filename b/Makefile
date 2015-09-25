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

#CFLAGS=-Wall -Wextra -fPIC -O3 -march=native -ffast-math -funroll-loops
CFLAGS=-Wall -Wextra -fPIC -g -ggdb

CC=gcc

all: first

first: first.c
	$(CC) $(CFLAGS) first.c -o first

test: first
	cat demo1.1st | ./first
	cat third help.th | ./first
	cat third demo1.th | ./first
	cat third demo2.th | ./first
	cat third demo3.th | ./first
	cat third demo4.th | ./first
	cat third demo5.th | ./first
	cat third demo6.th | ./first
	cat third ../test_comparison.f testcommand.f | ./first | tail -n +3 > x
	diff x ../test_comparison.f.out
	cat third ../test_stack.f testcommand.f | ./first | tail -n +3 > x
	diff x ../test_stack.f.out

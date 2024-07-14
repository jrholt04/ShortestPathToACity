# FILE: makefile [program 6]
# Jackson Holt, Transy U
# CS 2444, Fall 2023
#
#	build program 6 files 
#
# 0. Traget: prog6b [main program]
# depends on: progb.cpp

prog6 : prog6b.cpp
	g++ prog6b.cpp -o prog6

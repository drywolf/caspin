#!/usr/bin/env python

import os;
import sys;

if os.name == "posix":
	HEADER = '\033[95m'
	RED = '\033[91m'
	BLUE = '\033[94m'
	GREEN = '\033[92m'
	YELLOW = '\033[93m'
	WHITE = '\033[0m'
	
	def set_color(color):
		if color == "red":
			print RED,
		
		elif color == "blue":
			print BLUE,

		elif color == "green":
			print GREEN,
		
		elif color == "yellow":
			print YELLOW,
		
		elif color == "white":
			print WHITE,
		
		elif color == "gray":
			print WHITE,

elif os.name == "nt":
	STD_INPUT_HANDLE = -10
	STD_OUTPUT_HANDLE= -11
	STD_ERROR_HANDLE = -12

	FOREGROUND_BLUE = 0x01
	FOREGROUND_GREEN= 0x02
	FOREGROUND_RED  = 0x04
	FOREGROUND_INTENSITY = 0x08
	BACKGROUND_BLUE = 0x10
	BACKGROUND_GREEN= 0x20
	BACKGROUND_RED  = 0x40
	BACKGROUND_INTENSITY = 0x80
	
	import ctypes

	std_out_handle = ctypes.windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)

	def set_color(color):
		if color == "red":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_INTENSITY)
		
		elif color == "green":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY)
		
		elif color == "blue":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY)
		
		elif color == "yellow":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
		
		elif color == "white":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
		
		elif color == "gray":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

else:
	def set_color(color):
		return
	
def end_color():
	return set_color("gray")

def printc(color, text):
	set_color(color)
	print text,
	end_color()
	print ""
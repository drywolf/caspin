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
	
	def get_color_code(color):
		if color == "r":
			return RED
		
		elif color == "g":
			return GREEN
		
		elif color == "b":
			return BLUE
		
		elif color == "y":
			return YELLOW
		
		elif color == "w":
			return WHITE
		
		else:
			return WHITE
	
	def printf(color, text):
		sys.stdout.write(get_color_code(color) + text + get_color_code("d"))
	
	def println(color, text):
		sys.stdout.write(get_color_code(color) + text + get_color_code("d") + '\r\n')

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
		if color == "r":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_INTENSITY)
		
		elif color == "g":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY)
		
		elif color == "b":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY)
		
		elif color == "y":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
		
		elif color == "w":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
		
		elif color == "d":
			ctypes.windll.kernel32.SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
	
	def printf(color, text):
		set_color(color)
		print text,
		set_color("d")
	
	def println(color, text):
		set_color(color)
		print text
		set_color("d")
	
else:
	def printf(color, text):
		print text,

	def println(color, text):
		print text

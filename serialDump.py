import os

while True:
	os.system('cls' if os.name == 'nt' else 'clear')
	raw_input('Please connect a badge and press Enter to scan.')
	execfile('serialDumpModule.py')
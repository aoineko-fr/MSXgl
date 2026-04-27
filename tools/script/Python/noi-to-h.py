import glob
import sys

symbolList = []

# Extract symbols from a NOI file
def get_symbols(fname):
  a_file = open(fname, "r")
  for line in a_file:
    stripped_line = line.strip()
    line_list = stripped_line.split()
    if line_list[0] == "DEF" and line_list[1][0] == "_":
      str = "#define " + line_list[1][1:] + "\t" + line_list[2] + "\n"
      symbolList.append(str)
  a_file.close()

# Parse mapper files
file_list = glob.glob("*.noi")
for filename in file_list:
  get_symbols(filename)

# Convert
outFileName = "s_vgm_segments.h"
f = open(outFileName, "w")
f.write("// File autogeneraed by noi-to-h.py for MSXgl\n")
for sym in symbolList:
  f.write(sym)
f.close()

#print(symbolList)

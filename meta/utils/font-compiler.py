#!/usr/bin/env python3
# font compiler for pranaOS

# imports
import json 
import sys 
import struct 

in_filename = sys.argv[1]
out_filename = sys.argv[2]

infp = open(in_filename, 'r')
data = json.load(infp)
infp.close()

outfp = open(out_filename, 'wb')
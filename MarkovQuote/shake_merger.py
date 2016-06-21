#!/usr/bin/python
'''
shake_merger.py
  Combines all of Shakespear's plays into a single text document
  Pre-processing done to the initial text:
    Remove introduction (play information/meta-data)
    Remove stage directions and character names
    Standardize line breaks
    Surround punctuation by white space 
    Remove garbage characters 

Writen by: 
Brock Moir
bmoir@ualberta.net
Oct 6 2014
'''

import os
import textwrap
from string import punctuation

'''
process_scenes handles the pre-processing
  writes processed scenes to a single file
'''
def process_scenes(scenes, outfile):
  garbage = ['(', ')', '{', '}', '#', '--']  # list of useless punctuation
  for scene in scenes:                       # loop through the scenes
    blocks = scene.split('\n\n')             # blocks of character speach
    for block in blocks:                     # loop through the blocks
      block = block[block.find('\t')+1:]     # remove character names
      block = block.replace('\t', ' ')       # remove extra white space
      block = block.replace('\n', '')        # remove extra line breaks
      block = block.lower()                  # standardize capitalization
      if len(block) < 100: continue          # skip short blocks
      if '[' in block or 'act' in block or 'scene' in block: continue # garbage blocks
      for junk in garbage:                   # loop over garbage punctuation
        if junk in block: block = block.replace(junk, '') 
      for mark in punctuation:               # loop over the punctuation types
        if mark == "'" or mark == '-': continue # apostrophes and hyphens stay where they are
        if mark in block: block = block.replace(mark, ' ' + mark) #everythong else gets a space
      outfile.write(block + '\n')             #write the block

'''
scan_dirs crawls the directories
  hands the scenes to the scene processor
'''
def scan_dirs(indir, outfile):
  for root, dirs, filenames in os.walk(indir): # loop over filenames and directories
    if root == indir: continue # if the current directory is where this is run from
    for f in filenames: # loop over files
      with open (os.path.join(root, f), "r") as myfile: # open file
        text=myfile.read() # read text
        begin = text.find("]") # start text after intro
        scenes = text[begin+1:].split('\n\n\n') # split into scenes
        process_scenes(scenes, outfile) # hand to scene processor

if __name__ == "__main__":
  out_file = open("Combined.txt", "w")
  scan_dirs('/Shakespeare', out_file)
  out_file.close()


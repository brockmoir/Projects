#!/usr/bin/python
'''
quote_generator.py
  Generates Shakespearian quotes using Markov Chains

Writen by: 
Brock Moir
bmoir@ulberta.net
Oct 6 2014
'''

from random import randrange
from string import punctuation

class markov_quote():

  def __init__(self):
    self.dictionary = {}

  ''' the dictionary keys represent the current state
      the entries represent the possible future states
  '''
  def add_to_dictionary(self, key, value):
    if not self.dictionary.has_key(key):
      self.dictionary[key] = []
    self.dictionary[key].append(value)

  ''' learning requires reading each block word for word
      for each state, the next state is entered into the dictionary
  '''
  def learn(self, infile):
    blocks = infile.read().split('\n') # text is seperated into blocks
    for block in blocks:
      block_words = block.split(' ')   # each block is a character's quote
      current_state = ''               # initialize the states
      new_state = '\n'
      while block_words:               # while there are words in the block
        current_state = new_state      # update current state
        new_state = block_words.pop(0) # get a new word
        self.add_to_dictionary(current_state, new_state) # update the dictionary
      self.add_to_dictionary(new_state, '\n') # no more words

  ''' generating the next word is done by randomly sampling an entry from 
      the current word's dictionary entries
  '''
  def generate_next_word(self, current_state):
    return self.dictionary[current_state][randrange(len(self.dictionary[current_state]))]

  ''' putting it all together
  '''
  def generate(self):
    current_state = self.generate_next_word('\n')  # start with a new line 
    quote = current_state                          # initialize the quote 
    while True:                                    # loop until quote is generated
      next_word = self.generate_next_word(current_state) # get the next word
      if not next_word: continue # sometimes empty strings are put into the dictionary
      if next_word == '\n': return quote # an endline indicates the end of a quote
      if (not next_word in punctuation): quote += ' ' + next_word # spacing if the next word is a word
      else: quote += next_word # no space if it is punctuation
      current_state = next_word #update current state

if __name__=='__main__':
  infile = open('Combined.txt', "r")
  quote_gen = markov_quote()
  quote_gen.learn(infile)
  infile.close()
  print quote_gen.generate()



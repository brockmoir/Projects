# Markov-Quote
A fun pythonian project that loads and cleans Shakespearian literature, then simulates quotes using a niave Markov model.

#shakespear_merger.py
  
  Combines all of Shakespear's plays into a single text document using string manipulations and file i/o operations
  
  Pre-processing done to the initial text:
  
  &nbsp;&nbsp;Remove introduction (play information/meta-data)

  &nbsp;&nbsp;Remove stage directions and character names
    
  &nbsp;&nbsp;Standardize line breaks
    
  &nbsp;&nbsp;Surround punctuation by white space 
    
  &nbsp;&nbsp;Remove garbage characters 


#quote_generator.py
  &nbsp;&nbsp;Generates Shakespearian quotes using pythonin objects and and Monte Carlo sampling


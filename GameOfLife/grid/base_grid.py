import pygame

class grid:
  def __init__(self, block, screen, colour):
    # Define some colors
    self.colours = {} 
    self.colours["black"] = ( 0, 0, 0)
    self.colours["white"] = ( 255, 255, 255)
    self.colours["red"]   = ( 255, 0, 0)
    self.colours["green"] = ( 0, 255, 0)
    self.colours["blue"] = ( 0, 0, 255)
    self.colour = colour

    self.width, self.height, self.margin = block
    self.screen_width, self.screen_height  = screen
    
    self.xmax, self.ymax = 0, 0
    self.cells = {}
    self.init_cells()

  # Initialize the cell keys, should only be called internally  
  def init_cells(self):
    y, keep_going = 0, True
    while keep_going:
      y = y + self.margin
      x, dont_stop = 0, True
      while dont_stop:
        x = x + self.margin
        self.cells [(x,y)] = [self.colour]
        x = x + self.width
        if x > self.screen_width - self.width:
          dont_stop = False
          self.xmax = x - self.width
      y = y + self.width
      if y > self.screen_height - self.height:    
        keep_going = False
        self.ymax = y - self.height

  def draw_grid(self, screen):
    for coordinate, states in self.cells.iteritems():
      x, y = coordinate
      pygame.draw.rect(screen, self.colours[states[0]], (x, y, self.width, self.height)) 


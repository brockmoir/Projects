import base_grid

# Inherit a Grid and give it an update instruction
class life_grid(base_grid.grid):
  def __init__(self, block, screen, colour):
    base_grid.grid.__init__(self, block, screen, colour)
    self.mouse_clicks = []

  # Propogate mouse_clicks to the cells
  def add_mouse_clicks(self, mouse_click):
    x, y = mouse_click
    for coordinate in self.cells:
      xo, yo = coordinate
      if (xo < x and x <= xo + self.width) and (yo < y and y <= yo + self.height):
        self.cells[coordinate].pop()
        self.cells[coordinate].append("white")

  # Propogate the life
  def propogate_life(self):
    if len(self.cells[self.margin, self.margin]) == 1: 
      self.add_neighbors()
      return
    if len(self.cells[self.margin, self.margin]) == 9: 
      for coordinate in self.cells:      
        live_neighbors = 0
        old_state = self.cells[coordinate].pop(0)
        new_state = old_state

        while self.cells[coordinate]:
          if not self.cells[coordinate].pop() == "black":
            live_neighbors = live_neighbors + 1

        if old_state == "black":
          if live_neighbors == 3:
            new_state = "white"
        elif live_neighbors < 2 or live_neighbors > 3:
          new_state = "black"
        elif live_neighbors == 3:
          if old_state == "green":
            new_state = "red"
          if old_state == "blue":
            new_state = "green"
          if old_state == "red":
            new_state = "blue"
          if old_state == "white":
            new_state = "blue"

        self.cells[coordinate].append(new_state)
        self.add_neighbors()

  #Get the neighbor cell states
  def add_neighbors(self):
    if len(self.cells[self.margin, self.margin]) == 1:
      for coordinate in self.cells:
        xo, yo = coordinate
        for i in range(3):
          x = xo - (i - 1) * (self.width + self.margin)
          for j in range(3):
            y = yo - (j - 1) * (self.height + self.margin)
            if not (x == xo and y == yo):  
              if x < self.margin:
                x = self.xmax
              if x > self.screen_width - self.width:
                x = self.margin
              if y < self.margin: 
                y = self.ymax
              if y > self.screen_width - self.height:
                y = self.margin
              self.cells[(xo, yo)].append(self.cells[(x, y)][0])

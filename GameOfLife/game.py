import pygame
import grid.life_grid

pygame.init()
clock = pygame.time.Clock()

# Set the width and height of the screen[width,height]
screen_width , screen_height = 1000, 1000
size = [screen_width,screen_height]
screen = pygame.display.set_mode(size)
pygame.display.set_caption("My Game")

# Set the width and height of the grid
grid_width, grid_height = 5, 5
grid_margin = 1

# Loop until the user clicks the close button.
done = False

# If initialized is true then start the game
initialized = False

# i is the current frame
i=0

my_Grid = grid.life_grid.life_grid((grid_width, grid_height, grid_margin), (screen_width, screen_height), "black")

import random
initialized = True
for coordinate in my_Grid.cells:
  if bool(random.getrandbits(1)):
    my_Grid.cells[coordinate].pop()
    my_Grid.cells[coordinate].append("white")
my_Grid.add_neighbors()

while done == False:
  # ALL EVENT PROCESSING SHOULD GO BELOW THIS COMMENT

  for event in pygame.event.get(): # User did something
    if event.type == pygame.QUIT: # If user clicked close
      done = True # Flag that we are done so we exit this loop
    if not initialized:
      if event.type == pygame.KEYDOWN:
        if event.key == pygame.K_RETURN:
          initialized = True
    if not initialized:
      if event.type == pygame.MOUSEBUTTONDOWN:
        my_Grid.add_mouse_clicks(event.pos)
        
  # ALL EVENT PROCESSING SHOULD GO ABOVE THIS COMMENT

  # ALL GAME LOGIC SHOULD GO BELOW THIS COMMENT

  if i%1==0 and initialized:
    my_Grid.propogate_life()
  i = i + 1

  # ALL GAME LOGIC SHOULD GO ABOVE THIS COMMENT

  # ALL CODE TO DRAW SHOULD GO BELOW THIS COMMENT
  screen.fill(my_Grid.colours["white"])
  my_Grid.draw_grid(screen)

  pygame.display.flip()
  # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT


  # Limit to 20 frames per second
  clock.tick(20)

# Close the window and quit.
pygame.quit()

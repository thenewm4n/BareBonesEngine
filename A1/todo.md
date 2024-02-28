
# To Implement

1. Font case in switch statement.

- Read parameters from config.txt.
	- Will always be Window and Font line
	- May not be any shape lines
	- Window W H; Font F S R G B; Circle N X Y SX SY R G B R; Rectangle N X Y SX SY R G B W H

- New Shape class - what isn't in sf::Shape? -> velocities, name, drawn/not...
	- sf::Shape, draw/not, X & Y velocities, text, array for the imgui input box, width & height (if rectangle), radius & no. segments (if circle)
	- Upon instantiation: set position of shape according to file, set position of sf::Text (using text.getLocalBounds())

- Vector of shapes

- Reversing of x velocity if hit left or right, reversing of y velocity if hit top or bottom.
	- shape.getLocalBounds().top/.left/.width/.height
	- shape.getGlobalBounds() (?)


In ImGui:
- Drop box to select a shape
- Toggle if shape drawn
- Change scale (from 0-4)
- X & Y velocity (from -8 to 8)
- Change color
- Change text
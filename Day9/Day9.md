<h2>Part 1</h2>
A very simple geometry problem that can be brute forced.

<h2>Part 2</h2>
After a suggestion from a friend, I tried the raycast method. Firstly, I need to collect all the possible pairs of coordinates that form a border. Then for each possible rectangle, I check its border and whether it contains only red/green tiles for each tile in the border (except the corners). If the tile itself is already on one of the existing borders, then it passes the test. Otherwise, I check it against every existing border and see if it is below/to the right of it. If it is below/to the right of an odd number of borders, then it is a green tile.

The code right now is a converted python code from the same friend, I haven't gotten it to work with my own code yet. Still thinking about it.
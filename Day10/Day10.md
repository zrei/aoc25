<h2>Part 1</h2>
For part 1, I represented the final state of lights required as an unsigned int where each light is represented by a single bit. Each button is then represented by a bitmask that toggles certain bits, applied by the xor operator. I did a simple recursion method where I either press or don't press the current button, before moving on to the next button.

<h2>Part 2</h2>
Part 2 required me installing z3, which was its own adventure because g++ still could not find it after installing it via homebrew and initially sudo make install via the z3-master repo was failing due to some permission problems. Even after installing it via sudo make install g++ still couldn't find it, so I did some temporary environmental variables so it could be compiled.

Each button is represented by a variable that indicates how many times it should be pressed. We can then create equations for each number in the joltage. This was done using `expr_vector` to compile the individual expressions. Since we needed to optimise the sum of these variables, I used an optimiser. An additioanl condition that all the variables had to be >= 0 was also added.

Part 1 can probably be done with z3 as well.
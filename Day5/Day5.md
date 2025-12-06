<h2>Part 1</h2>
When parsing the ranges, I made sure to iterate through the available ranges and merge them if possible. There's an additional optimisation I could have done by sorting the ranges first so I don't have to arbitrarily repeat the merging process until no more merges are detected.

For each ingredient ID, we can then check through all the ranges to find if it's a part of any.

<h2>Part 2</h2>
Since I'd already merged the ranges in part 1, all I had to do was sum up their intervals.

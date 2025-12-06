<h2>Part 1</h2>
The operators were parsed first by iterating through the final input line.

I tried to find the required indices to separate the columns by parsing the first line, but this wasn't possible with just the first line since the columns may be either left or right aligned. In the end I opted to iterate through each line individidually to parse out the numbers and add/multiply them to the current result for each column group.

<h2>Part 2</h2>
I iterated through all the lines at the same time with one pointer, ignoring stretches where all characters in a column were spaces. For each column with non-space characters, I went from the top to calculate the number and add/multiply them to the current result for the entire column group.
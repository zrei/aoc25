<h2>Part 1</h2>
Store the pointers for the current two highest digits (will start at 0 and 1 respectively). Iterate through the string. If the current digit is higher than the first digit of our currently stored highest number and there's space to put the second digit after this current digit, update the first digit and put the second digit as the next digit. Otherwise, check if we should update our second digit. 

Initially missed out the overlap case, e.g. if our current highest digits is 79 and there are more digits after, you'd want to transfer the first digit to 9. This check is done after every update of our highest digits by constantly checking if we need to transfer the digits (and if there's space) until we no longer need to. The pointer used to iterate through the string is then updated.

Since digits can't be rearranged, we can only ever go forward in the string and this solution is O(n).

<h2>Part 2</h2>
Part 2 is just an extension of part 1 so I stored all the highest digit pointers in a `std::array` of size 12 and wrote helper functions to perform the same functionality as part 1 but with an array of pointers instead.
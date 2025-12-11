<h2>Part 1</h2>
First build an adjacency graph, then just BFS through it.

<h2>Part 2</h2>
From investigation (running part 1's code with `dac` as input and `fft` as output and vice versa), I found that there was no way between `dac` and `fft`.

You could calculate the number of ways from `svr` to `fft`, then from `fft` to `dac`, and finally from `dac` to `out`, and multiply all of them together to get the total number of ways (permutations).

The problem space in this part is much bigger than part 1, so memoisation was required. At first I was memoising wrongly...
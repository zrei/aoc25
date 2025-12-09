<h2>Part 1</h2>
Misunderstood part 1 as needing to actually connect 1000 nodes, so I ended up writing part 2's code first, but later corrected it to be the first 1000 pairs of nodes even if they don't end up directly connected.

First, I calculated the squared distances between every possible pair, before sorting the pairs in ascending order by their squared distances. I maintained an adjacency list that is bi-directional and stores the direct links for each node. I then iterated through the pairs, checking if they're already connected via traversal of the adjacency list. If they're not already connected, add a direct connection.

I then iterated through the adjacency list, calculating the length of each unique circuit via traversal.

<h2>Part 2</h2>
Part 2 simply involves continuing to update the adjacency list until we've made 999 direct links, which will ensure all nodes are linked in one circuit.
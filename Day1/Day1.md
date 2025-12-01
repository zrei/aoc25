<h2>Part 1</h2>
A fairly simple problem. Modulo the amount to move since we don't need to take into account cycles. Apply the movement amount to the current safe value, then convert it back into the acceptable range. Increment the counter every time we stop on 0.

<h2>Part 2</h2>
Not very complicated either, but I committed quite a few off-by-one errors. Before we modulo the amount to move, we calculate the number of full cycles moved around the safe, which will add to the number of times we pass 0. Then if the final value before modulo is greater than the maximum number or smaller or equal to 0 and the original value is not 0, it has passed 0 one more time.
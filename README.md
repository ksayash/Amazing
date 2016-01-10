# amazing
Implementing An Optimal Bidding Strategy for EC2

##How Does This Work?
This is currently able to work for 2 cases:
###Simulating data and outputting value
For testing Amazing on simulated data with given Transition matrix and spot prices being integers from 1 to n (the most basic case), use the small.c file located in the lpsolve1 directory. You can change the number of spot prices (nspot), the number of tests you want to run (num).
Note that this program has an implicit random number generator, meaning you don't have to input any spot price values.
###Inputting data from spot price file
For testing Amazing on real world data , gathered from the Amazon EC2 website, use the results.c file. Note that currenly the C code has been written for a standard output file, containing 951 different spot instance prices. However, this can be easily modified to take in different values.

##That is alright, but HOW DOES THIS WORK?
To get results, Amazing uses the lpsolve library for C ( A huge shout out to its creators).
Open terminal in the lpsolve1 folder.
In the terminal, write

<pre><code>
gcc small.c liblpsolve55.a -ldl -lm
</code></pre>

Currently, the implementation doesn't work on windows, due to library linking issues.


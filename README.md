# Weighted Activity Scheduler for Algorithms HW
## To run
g++ activitySelection.cpp
./a.out [input] [output]

Default input: input.txt
Default output: output.txt
Both are optional parameters

## Test cases
Below are some test inputs I made to make sure some parts of my program were working as intended.

### test1.txt:  
- Tests for the case that the greatest value schedule is out of the interval, and the next best is one single activity.
### test2.txt: 
- Tests for the case that the greatest value schedule is multiple activities right after each other.
### test3.txt:
- Tests for the case that the ascending order of ID’s is not the same as the descending order of end times (make sure the output orders the ID’s right).
### test4.txt: 
- Tests to ensure the output is 0 for an interval of length 0
### test5.txt: 
- Tests to ensure the program works with just one activity in the interval

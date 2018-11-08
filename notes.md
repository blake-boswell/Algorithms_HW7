# Greedy Algorithms
- Make the choice that looks best at the moment, hoping this leads to optimal solution
## Activity-selection problem
- Want to find the maximum subset for renting a room
## The Greedy choice
- Activities are sorted in increasing order based on finish time
- Find the activity that opens up the most space for more activities
    - Find the activity with the earliest finish time
- Choose a_1 first
- Then choose the next earliest finish time after a_1 finishes
- Top down design: make a choice then solve a subproblem

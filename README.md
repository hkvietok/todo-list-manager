# To-Do List Manager
Final Project for CSCI 2270: Data Structures 

To-Do List for Managing Tasks Sorted by Urgency

**Data Structures Used:**

For my final project, I created a to-list manager where tasks are sorted by urgency. The main data structure that I used was a Binary Search Tree. In the tree, the to-do nodes are sorted by their urgency. So, the rightmost node is the most urgent task at any given time. I used this data structure because it has the organization structure that I need for sorting elements and it also has search time complexity of O(log(n)). I could have used a priority queue for my main data structure, but searching through a priority queue has a time complexity of O(n). The search complexity mattered to me because I wanted the user to be able to quickly find tasks that were not necessarily the most urgent.

I also used a number of other data structures in a smaller capacity throughout my project. I used stacks for the traversal of my tree in a number of functions. For instance, my random function uses a stack to put nodes into an access a random node given a random count which decrements each time we pop a node from the stack. Also, I use a stack for printing all of the elements of the tree to the text file when the user exits the program.

**References:**

I thought it would be a neat feature to add the time at which each to-do task was created by the user. We had not learned this is in class so I researched online and utilize a piece of code from: https://stackoverflow.com/a/16358224. This allowed me to include when exactly the to-do was created, so a user can notice if they made their to-do a long time ago.

**Methodology and Design:**
![alt text](https://github.com/hkvietok/todo-list-manager/blob/master/Design.png?raw=true)

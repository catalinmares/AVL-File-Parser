# AVL-File-Parser

The program I wrote is composed, beside the functions given in the header file,

of other auxiliar functions that I wrote to help me implement the problem given.

In the following file I will explain the implementantion of every function and

the way I thought while writing this program.

\\--------------------------------- TASK 1 ----------------------------------//


Beside the specific functions of an AVL tree (minimum, maximum, successor,

predecessor, search) which are explained in the comments of the source file, I

will explain the most important details of implementation of the functions

insert and delete.


The function insert treats 3 cases:

1. The tree is empty -> In this case we insert the new element into the root.

2. The tree already contains the inserted element -> In this case we insert the

node in the list after the last duplicate. More details of implementation can

be read in the comments of the source file.
3. The tree isn't empty and the 
element isn't contained in the tree -> In this
case we parse the tree comparing 
the new element with each node of the tree to
find the position for the new 
element. After insertion, we link the new
element to its successor and its 
predecessor in the list and start rebalancing
the tree from the new node inserted.


The function delete treats 4 cases:

1. The element has duplicates -> In this case we delete the last duplicate from

the list of duplicates. After deletion we update the end pointer for each

duplicate.

2. The element has 2 children -> In this case we put in its place its successor

and the new node to be deleted will be the successor's old position.

3. The element has no children -> In this case we remove it from both the tree

and the list by setting their neighbours' links and we start rebalancing the

tree fro the deepest node.
4. The element has one child -> In this case we simply 
link the one child of the
node to its parent and viceversa and we delete the 
element and update the links
in the list and start rebalancing from the parent.



\\--------------------------------- TASK 2 ----------------------------------//


In the implementation of this task, I use the function fgets to read the text

from the file and I extract the words using strtok. After each extraction, I

update the index for the next word and I insert the current word with its

current index into the tree. The tree will contain now strings of 3 characters

with their index of appearence into the text file. This way, at an inorder

traversal of the tree, the words inserted will be printed alphabetically.



\\--------------------------------- TASK 3 ----------------------------------//


In the implementation of this task, I make use of the inorder list of the tree

to extract the indexes of the words starting with the characters given by the

key. For this, I copy as many characters as they key has from the element of

the list into a new string which I compare with the key. If they are equal, I

save the index into the range array of indexes. If the index array gets full,

I realloc it with double capacity.


\\--------------------------------- TASK 4 ----------------------------------//


Very similar to the previous task, except the fact that here I need to copy as

many characters as each key has (starting key, ending key) in 2 separated

strings which I compare with the keys. As long as strcmp returns a positive

value comparing the first string with the first key and a negative value

comparing the second string with the second key I stock the index of the element

into the range array of indexes. If the index array gets full, I realloc it

with double capacity.

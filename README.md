# iterative-dichotomiser-3
A vector based implementation of the Iterative Dichotomiser 3 (ID3) algorithm by John Ross Quinlan

The data used consists of the price, weight, page count and the height and the width of the book. The labels are 0 for history/social 1 for computer science and 2 for fiction. Altough not a big dataset, it is good to validate the functionality of the tree because it leads to results understandable for humans.

## Concept
![](images/tree.png "decision tree")
The basic idea is that the 2 dimensional tree structure (left) can be reduced to 1 dimension almost like a chain (right). In order to work, the nodes must contain information about class decision (feature and threshold) and connections to the  child nodes

A tree in the 2 dimensional structure needs node objects to store this information with references to the child nodes as connections. 

Since all nodes in the 1 dimensional representation can be easily indexed *[0..n]*. Since satisfying the condition for a given decision node with index m simply leads to the node with the index m+1, only one integer number is needed for decision nodes to store the "jumps".

Class, feature and threshold are also stored as single numbers. However, as classification uses class labels from 0 to number of classes - 1, the amount of variables needed to be stored can be further reduced by combining the class and feature. For doing so the formula *-(label + 1)* is used meaning that for the lowest class label (0) a negative and therefore invalid feature index is calculated. When looking at the feature/class value, the program is then able to determine whether it has reached a leaf node.

Storing the information of each node in three vectors (features/class, thresholds and jumps) allows for placing all the information of a node in the same index. Since the root node always has the index 0, the path to the next node can be easily calculated by either incrementing the index to 1 or by jumping to the index stored in jumps[0] depending on whether the condition based on *feature/class[0]* and *thresholds[0]* was satisfied. The process is then repeated until a invalid (i.e. negative) feature is met, which means that the program has arrived at a leaf node. When returning the prediction, the modified class label is then transformed back to the corresponding value between *0* and *number of classes-1*
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))
#define HEIGHT(x) ((x)?((x)->height):(0))

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just assume
  some type Item was previously defined.
 */
// -----------------------------------------------------------------------------

typedef struct node{
	void* elem;
	void* info;
	struct node *pt;
	struct node *lt;
	struct node *rt;
	struct node* next;
	struct node* prev;
	struct node* end;
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root;
	void* (*createElement)(void*);
	void (*destroyElement)(void*);
	void* (*createInfo)(void*);
	void (*destroyInfo)(void*);
	int (*compare)(void*, void*);
	long size;
}TTree;


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*)){
	// TODO: Cerinta 1
	/* 1. This tree does NOT have any sentinels!!!
	 * "root" is just a pointer to the actual root node.
	 * 
	 * 2. You must set all function pointers in the structure!!!
	 */
	 // Allocate memory.
	 TTree* tree = (TTree*) calloc(1, sizeof(TTree));
	 // Initialize the structure fields.
	 tree->createElement = createElement;
	 tree->destroyElement = destroyElement;
	 tree->createInfo = createInfo;
	 tree->destroyInfo = destroyInfo;
	 tree->compare = compare;
	 // Set size to 0 - empty tree.
	 tree->size = 0;

	return tree;
}

TreeNode* createTreeNode(TTree *tree, void* value, void* info){	
	// TODO: Cerinta 1
	/* Implementing and using this function
	 * will greatly improve your chances of 
	 * getting this to work.
	 */


	// Allocate memory.
	TreeNode* node = (TreeNode*) calloc(1, sizeof(TreeNode));
	// Initialize fields.
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);
	// Increase size and set height to 1.
	node->height = 1;
	tree->size++;

	return node;
}

void destroyTreeNode(TTree *tree, TreeNode* node){
	// TODO: Cerinta 1
	/* Implementing and using this function
	 * will greatly improve your chances of 
	 * getting this to work.
	 */

	// Destroy elem and info fields.
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);
	// Decrease size.
	tree->size--;
	// Free node.
	free(node);
	node = NULL;
}


int isEmpty(TTree* tree){
	// TODO: Cerinta 1
	return tree->root == NULL;
}

TreeNode* search(TTree* tree, TreeNode* x, void* elem){

	// TODO: Cerinta 1
	while (x != NULL)
	{
		int cmp = tree->compare(x->elem, elem);

		if (cmp == 0)
			// Node was found in the tree.
			return x;

		else
		{
			if (cmp == 1)
			{
				// Node has higher value than the searched node.
				// We move to its left child.
				x = x->lt;
			}

			else
			{
				// Node has lower value thant the searched node.
				// We move to its right child.
				x = x->rt;
			}
		}
	}

	// Return NULL for no such node found in the tree.
	return NULL;
}

TreeNode* minimum(TTree* tree, TreeNode* x){
	// TODO: Cerinta 1
	while (x->lt != NULL)
		// The minimum is the leftest node.
		x = x->lt;
	return x;
}

TreeNode* maximum(TTree* tree, TreeNode* x){
	// TODO: Cerinta 1
	while (x->rt != NULL)
		// The maximum is the rightest node.
		x = x->rt;
	return x;
}

TreeNode* successor(TTree* tree, TreeNode* x){
	// TODO: Cerinta 1
	if (x == maximum(tree, tree->root))
	{
		// Maximum node has no successor.
		return NULL;
	}

	if (x == NULL)
	{
		// Invalid input.
		return NULL;
	}

	if (x->rt != NULL)
	{
		// If the node has a right sub-tree, the successor is the
		// minimum element of the right sub-tree.
		return minimum(tree, x->rt);
	}

	TreeNode* parent = x->pt;

	while (parent != NULL && x == parent->rt)
	{
		// The node is a right child.
		// Its successor will be the first parent with a higher value.
		x = parent;
		parent = parent->pt;
	}

	// The node is a left child.
	// Its successor will be its parent.
	return parent;
}

TreeNode* predecessor(TTree* tree, TreeNode* x){
	// TODO: Cerinta 1

	if (x == minimum(tree, tree->root))
	{
		// Minimum node has no predecessor.
		return NULL;
	}

	if (x == NULL)
	{
		// Invalid imput.
		return NULL;
	}

	if (x->lt != NULL)
	{
		// If the node has a left sub-tree, the predessor is the
		// maximum element of the left sub-tree.
		return maximum(tree, x->lt);
	}

	TreeNode* parent = x->pt;

	while (parent != NULL && x == parent->lt)
	{
		// The node is a left child.
		// Its predecessor will be the first parent with a lower value.
		x = parent;
		parent = parent->pt;
	}

	// The node is a right child.
	// Its predecessor will be its parent.
	return parent;
}

void avlRotateLeft(TTree* tree, TreeNode* x){
	// TODO: Cerinta 1
	/* You may want to use the macros at the top of this file.
	 */

	// Save the right child of the node.
	TreeNode* y = x->rt;

	// The left child of the rotate node's right child
	// becomes the right child.
    x->rt = y->lt;

    if (y->lt != NULL)
    {
    	// If the left child isn't NULL, link it to the new parent.
    	y->lt->pt = x;
    }

    // Right child's parent becomes the rotated node's parent.
    y->pt = x->pt;

    if (x->pt == NULL)
    {
    	// Case for root rotation.
    	tree->root = y;
    }
    
    else
    {
    	// Test if the node is a left or right child and
    	// links its child to its parent.
    	if (x->pt->rt == x)
    	{
    		x->pt->rt = y;
    	}

    	else
    	{
    		x->pt->lt = y;
    	}

    }

    // Move the node to its child's left and update the parent.
    y->lt = x;
    x->pt = y;


    // Recalculate the heights for the 2 nodes.
    y->height = MAX(HEIGHT(y->lt), HEIGHT(y->rt)) + 1;
    x->height = MAX(HEIGHT(x->lt), HEIGHT(x->rt)) + 1;
}

void avlRotateRight(TTree* tree, TreeNode* y){
	// TODO: Cerinta 1
	/* You may want to use the macros at the top of this file.
	 */

	// Save the left child of the node.
	TreeNode* x = y->lt; 

	// The right child of the rotated node's left child
	// becomes the left child.
    y->lt = x->rt;

    if (x->rt != NULL)
    {
    	// If the right child isn't NULL, link it to the new parent.
    	x->rt->pt = y;
    }

    // Left child's parent becomes the rotated node's parent.
    x->pt = y->pt;

    if (y->pt == NULL)
    {
    	// Case for root rotation.
    	tree->root = x;
    }

    else
    {
    	// Test if the node is a left or right child and
    	// link its child to its parent.
    	if (y->pt->lt == y)
    	{
    		y->pt->lt = x;
   		}

    	else
    	{
    		y->pt->rt = x;
    	}
    }

    // Move the node to his child's right and update the parent.
    x->rt = y;
    y->pt = x;

    // Recalculate the heights for the 2 nodes.
    y->height = MAX(HEIGHT(y->lt), HEIGHT(y->rt)) + 1;
    x->height = MAX(HEIGHT(x->lt), HEIGHT(x->rt)) + 1;
}


int avlGetBalance(TTree* tree, TreeNode *x){
	//TODO: // TODO: Cerinta 1

	/* Get AVL balance factor for node x.
	 * You may want to use the macros at the top of this file.
	 */ 

	 if (x == NULL)
	 	// The height of NULL is 0 by convention.
		return 0;
	return HEIGHT(x->lt) - HEIGHT(x->rt);
}


void avlFixUp(TTree* tree, TreeNode* y){
	//TODO: Cerinta 1
	/* Fix any unblance from this node to the top of tree
	 */

	TreeNode* tmp2 = y;
	// Parse the tree from bottom to top
	while (tmp2 != NULL)
	{
		// Update the current node's height and calculate its balance factor.
		tmp2->height = MAX(HEIGHT(tmp2->lt), HEIGHT(tmp2->rt)) + 1;

		int balance = avlGetBalance(tree, tmp2);

		// Left Left Case
		if (balance > 1 && tree->compare(y->elem, tmp2->lt->elem) == -1)
		{
			avlRotateRight(tree, tmp2);
			continue;
		}

		// Right Right Case
		if (balance < -1 && tree->compare(y->elem, tmp2->rt->elem) == 1)
    	{
    		avlRotateLeft(tree, tmp2);
    		continue;
	    }
 
    	// Left Right Case
    	if (balance > 1 && tree->compare(y->elem, tmp2->lt->elem) == 1)
    	{
        	avlRotateLeft(tree, tmp2->lt);
        	avlRotateRight(tree, tmp2);
        	continue;
	    }
 
    	// Right Left Case
    	if (balance < -1 && tree->compare(y->elem, tmp2->rt->elem) == -1)
    	{
        	avlRotateRight(tree, tmp2->rt);
        	avlRotateLeft(tree, tmp2);
        	continue;
	    }

	    tmp2 = tmp2->pt;
	}
}

void insert(TTree* tree, void* elem, void* info) {
	//TODO: Cerinta 1
	/*
	 * 1. Begin by implementing the normal BST insersion (no duplicates).
	 * 2. Fix any unbalance caused by this insertion as the last operation.
	 * 3. Now if this element is a duplicate all you have to do is to
	 *    inserted in the approapiate list. Inseting the duplicate at the
	 *    end of a list is easier and consistent!
	 */

	TreeNode* duplicate = search(tree, tree->root, elem);
	TreeNode* newElem = createTreeNode(tree, elem, info);

	if (isEmpty(tree))
	{
		// The tree has no elements; insert into the root.
		tree->root = newElem;
		tree->root->end = tree->root;	
		return;
	}

	TreeNode* tmp = tree->root;
	TreeNode* parent = NULL;

	while(tmp != NULL)
	{
		// The tree has multiple elements;
		// parse the tree and insert the new node at its place.
		if (tree->compare(tmp->elem, newElem->elem) == 1)
		{
			parent = tmp;
			tmp = tmp->lt;
		}

		else
		{
			if (tree->compare(tmp->elem, newElem->elem) == -1)
			{
				parent = tmp;
				tmp = tmp->rt;
			}

			else
			{
				// I have found a duplicate.
				break;
			}
		}
	}

	if (tmp != NULL)
	{
		// Case for duplicate insertion.

		// Move to the last duplicate.
		TreeNode* end = tmp->end;

		// Insert the new duplicate after the old last.
		newElem->next = end->next;
		newElem->prev = end;

		// Update the neighbours' links.
		if (end->next != NULL)
		{
			end->next->prev = newElem;
		}

		end->next = newElem;

		TreeNode* temp = tmp;

		// Parse the duplicates and update the end pointer.
		while (temp != newElem->next)
		{
			temp->end = newElem;
			temp = temp->next;
		}

		return;
	}
	
	// Link the parent to the new node.
	if (tree->compare(parent->elem, newElem->elem) == 1)
	{
		parent->lt = newElem;
	}

	else
	{
		parent->rt = newElem;
	}

	// Link the new node to the parent and insert it into the list.
	newElem->pt = parent;
	newElem->prev = predecessor(tree, newElem);
	newElem->next = successor(tree, newElem);
	newElem->end = newElem;

	// Update the neighbours' links in the list.
	if (newElem->prev != NULL) newElem->prev->end->next = newElem;
	if (newElem->next != NULL) newElem->next->end->prev = newElem;

	// Start rebalancing from the new element.
	avlFixUp(tree, newElem);
}

void delete(TTree* tree, void* elem){

	//TODO: Cerinta 1
	/*
	 * 1. Begin by implementing the normal BST deletion (no duplicates).
	 * 2. Fix any unbalance caused by this insertion as the last operation.
	 * 3. Now what happends if the elem you are trining to delete has duplicates?
	 *    Is it easier to delete the duplicate? Which one?
	 *    What happends to the tree-list when you delete an elem with no duplicates?
	 *    
	 */	

	TreeNode* toDelete = search(tree, tree->root, elem);

	if (toDelete == NULL)
	{
		// No node with given elem was found.
	 	return;
	}

	if (toDelete->end != toDelete)
	{
	 	// The node to be deleted has duplicates.
	 	TreeNode* end = toDelete->end;
	 	
	 	end->prev->next = end->next;

	 	if (end->next != NULL)
	 	{
	 		end->next->prev = end->prev;
	 	}

	 	TreeNode* newEndNode = end->prev;

	 	// Destroy the last duplicated node.
	 	destroyTreeNode(tree, end);
	 	
	 	TreeNode* temp = toDelete;

	 	// Update end pointer for all duplicates.
	 	while(temp != newEndNode->next)
	 	{
	 		temp->end = newEndNode;
	 		temp = temp->next;
	 	}

	 	return;
	}

	// Disconnect the node from the list
	if (toDelete->prev != NULL) toDelete->prev->next = successor(tree, toDelete);
	if (toDelete->next != NULL) toDelete->next->prev = predecessor(tree, toDelete);

	if (toDelete->lt != NULL && toDelete->rt != NULL)
	{
		// Case with 2 children.
		// Replace the node with his successor's fields and
		// set the successor node for deletion.
	 	TreeNode* succ = successor(tree, toDelete);
	 	// Destroy the current element.
	 	tree->destroyElement(toDelete->elem);
	 	tree->destroyInfo(toDelete->info);
	 	// Replace with the new element.
	 	toDelete->elem = tree->createElement(succ->elem);
	 	toDelete->info = tree->createInfo(succ->info);

	 	toDelete->prev = succ->prev;
	 	toDelete->next = succ->next;
	 	succ->prev->next = tree->root;
	 	toDelete = succ;
	}

	// Save the parent of the node to be deleted.
	TreeNode* parent = toDelete->pt;

	if (toDelete->lt == NULL && toDelete->rt == NULL)
	{
		// Case with no children.
		if (toDelete == tree->root)
		{
			// The node is root and has no children.
			destroyTreeNode(tree, tree->root);
			tree->root = NULL;
			return;
		}

		// Break the link with the parent node.
	 	if (parent->lt == toDelete)
	 	{
	 		parent->lt = NULL;
	 	}

	 	else
	 	{
	 		parent->rt = NULL;
	 	}

	 	destroyTreeNode(tree, toDelete);

	 	if (parent->lt == NULL && parent->rt == NULL)
	 	{
	 		// The parent has no more children. 
	 		// No balancing is needed.
	 		return;
	 	}

	 	TreeNode* fixNodeStart;

	 	// Find the deepest node in the left/right subtree.
	 	fixNodeStart = ((parent->lt == NULL) ? 
	 	(minimum(tree, maximum(tree, parent->rt))) : 
	 	(maximum(tree, minimum(tree, parent->lt))));

	 	// Start rebalancing from there.
	 	avlFixUp(tree, fixNodeStart);
	 	return;
	}

	if (toDelete->rt != NULL || toDelete->lt != NULL)
	{
		// Case with only one children.

		// Link the parent with the children of the node.
	 	if (parent->lt == toDelete)
	 	{
	 		parent->lt = ((toDelete->rt != NULL) ? 
	 		(toDelete->rt) : (toDelete->lt));
	 	}

	 	else
	 	{
	 		parent->rt = ((toDelete->rt != NULL) ? 
	 		(toDelete->rt) : (toDelete->lt));
	 	}

	 	// Link the children with the node's parent.
	 	(toDelete->rt != NULL) ? 
	 	(toDelete->rt->pt = parent) : (toDelete->lt->pt = parent);

	 	destroyTreeNode(tree, toDelete);

	 	avlFixUp(tree, parent);
	}
}

void destroyTree(TTree* tree){
	//TODO: Cerinta 1
	/* What is the easiest way to get rid of all elements for this SPECIAL tree?
	 */

	if (tree == NULL) return;
	
	// Start from the minimum element.
	TreeNode* temp = minimum(tree, tree->root);
	TreeNode* next;

	// Parse the tree's inorder list and delete each element.
	while (temp != NULL)
	{
		next = temp->next;
		destroyTreeNode(tree, temp);
		temp = next;
	}

	free(tree);
}


#endif /* AVLTREE_H_ */

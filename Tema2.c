#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 1024
#define ELEMENT_TREE_LENGTH 3

#include "AVLTree.h"

typedef struct Range{
	int *index;
	int size;
	int capacity;
}Range;

void printFile(char* fileName){
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;
	char *buf = (char*) malloc(BUFLEN+1);
	while(fgets(buf,BUFLEN,file) != NULL){
		printf("%s",buf);
	}
	printf("\n");
	free(buf);
	fclose(file);
}

void printWordsInRangeFromFile(Range* range, char* fileName){
	if(fileName == NULL || range == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;
	char *buf = (char*) malloc(BUFLEN+1);
	for(int i = 0; i < range->size;i++){
		fseek(file,range->index[i],SEEK_SET);
		if(fgets(buf,BUFLEN,file) != NULL){
			char* token = strtok(buf," .,\n");
			printf("%d. %s:%d\n",i+1, token, range->index[i]);
		}
	}
	printf("\n");
	free(buf);
	fclose(file);
}

void printTreeInOrderHelper(TTree* tree, TreeNode* node){
	if(node != NULL){
		printTreeInOrderHelper(tree, node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%s  ",*((int*)begin->info),((char*)begin->elem));
			begin = begin->next;
		}
		printTreeInOrderHelper(tree, node->rt);
	}
}

void printTreeInOrder(TTree* tree){
	if(tree == NULL) return;
	printTreeInOrderHelper(tree, tree->root);
}


void* createStrElement(void* str){
	// TODO: Cerinta 2
	/* Allocate a string element to store in the tree.
	 */
	char* s = malloc(4*sizeof(char));
	// Save the first 3 characters in the element.
	strncpy(s, (char*) str, 3);
	s[3] = '\0';	
	return s;
}

void destroyStrElement(void* elem){
	// TODO: Cerinta 2
	/* Free a string element.
	 */

	free((char*)elem);
	elem = NULL;
}


void* createIndexInfo(void* index){
	// TODO: Cerinta 2
	/* Allocate space to store a new index
	 */
	int *l = malloc(sizeof(int));
	// Save the index.
	*l = *((int*) (index));
	return (void*) l;
}

void destroyIndexInfo(void* index){
	// TODO: Cerinta 2
	/* Free storage space for the index.
	 */

	free((int*)index);
	index = NULL;
}

int compareStrElem(void* str1, void* str2){
	// TODO: Cerinta 2
	/*  0  means strings are equal 
	 * -1  means str1 < str2
	 *  1  means str1 > str2
	 */

	 int cmp = strcmp((char*) str1, (char*) str2);

	if (cmp < 0) return -1; // str1 < str2
	if (cmp > 0) return 1;	// str1 > str2
	return 0;				// str1 == str2
}


TTree* buildTreeFromFile(char* fileName){
	// TODO: Cerinta 2
	/* Parse the text file keeping track of indices!
	 * Words separators are the following: " :.,\r\n"
	 * At each step insert the <token, index> pair in the tree.
	 */
	
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return NULL;

	TTree* tree = createTree(createStrElement, destroyStrElement, 
	createIndexInfo, destroyIndexInfo, compareStrElem);

	int index = 0;
	char *buf = (char*) malloc(BUFLEN+1);
	if (buf == NULL) return NULL;

	// Read the whole text from file.
	fgets(buf, 250, file);

	// Extract the first word.
	char* str2 = strtok(buf, " .,\n");

	while (str2 != NULL)
	{
		// Insert the word in the tree.
		insert(tree, (void*) str2, (void*) &index);
		// Update the new index.
		index = index + strlen(str2) + 1;
		// Extract the next word.
		str2 = strtok(NULL, " .,\n");
	}

	free(buf);
	buf = NULL;
	fclose(file);

	return tree;
}

Range* createRange(int capacity)
{
	Range* range = (Range*) calloc(1, sizeof(Range));
	if (range == NULL) return NULL;

	range->index = (int*) calloc(capacity, sizeof(int));
	if (range->index == NULL) return NULL;

	range->size = 0;
	range->capacity = capacity;

	return range;
}

Range* singleKeyRangeQuery(TTree* tree, char* q){
	// TODO: Cerinta 3
	/* Get the words indices begining with the patern specified by q
	 * How can do you traverse the tree?
	 * There might be duplicates, can you make use of the tree duplicate lists?
	 */

	Range* range = createRange(1);

	// Start parsing the list from the minimum element in the tree.
	TreeNode* list = minimum(tree, tree->root);

	int keyLen = strlen(q);
	char* str = (char*) calloc(ELEMENT_TREE_LENGTH, sizeof(char));
	if (str == NULL) return NULL;

	while (list != NULL)
	{
		// Copy as many characters as the key has from the element in the tree.
	 	strncpy(str, (char*)(list->elem), keyLen);
	 	if (strcmp(str, q) == 0)
	 	{
	 		// if those characters are equal to the given key,
	 		// the element index must be saved.
	 		if (range->size == range->capacity)
			{
				// Realloc at double capacity if the index array is full.
				range->capacity = 2 * range->capacity;
				range->index = (int*) realloc(range->index, range->capacity * sizeof(range->index));
			}

	 		range->index[range->size++] = *(int*)(list->info);
	 	}

	 	list = list->next;
	}

	free(str);
	str = NULL;

	return range;
}

Range* multiKeyRangeQuery(TTree* tree, char* q, char* p){
	// TODO: Cerinta 4
	/* Get the words indices begining with the any patern 
	 * between q and p (inclusive).
	 */

	Range* range = createRange(1);

	// start parsing the list from the minimum element in the tree.
	TreeNode* list = minimum(tree, tree->root);

	int keyStartLen = strlen(q);
	int keyStopLen = strlen(p);

	char* str1 = (char*) calloc(ELEMENT_TREE_LENGTH, sizeof(char));
	char* str2 = (char*) calloc(ELEMENT_TREE_LENGTH, sizeof(char));

	while (list != NULL)
	{
		// Copy as many characters as the keys have from the element in the tree.
		strncpy(str1, (char*)(list->elem), keyStartLen);

		strncpy(str2, (char*)(list->elem), keyStopLen);
		
		if (strcmp(str1, q) >= 0 && strcmp(str2, p) <= 0)
		{
			// If the element is in the range determined by the keys,
			// the leement index must be saved.
			if (range->size == range->capacity)
			{
				// Realloc at double capacity if the index array is full.
				range->capacity = 2 * range->capacity;
				range->index = (int*) realloc(range->index, range->capacity * sizeof(range->index));
			}

			range->index[range->size++] = *(int*)(list->info);
		}

		list = list->next;
	}

	free(str1);
	str1 = NULL;
	free(str2);
	str2 = NULL;

	return range;
}


int main(void) {

	printf("The text file:\n");
	printFile("text.txt");

	TTree* tree = buildTreeFromFile("text.txt");
	printf("Tree In Order:\n");
	printTreeInOrder(tree);
	printf("\n\n");

	printf("Single search:\n");
	Range *range = singleKeyRangeQuery(tree,"v");
	printWordsInRangeFromFile(range,"text.txt");

	printf("Multi search:\n");
	Range *range2 = multiKeyRangeQuery(tree,"j","pr");
	printWordsInRangeFromFile(range2,"text.txt");

	if(range != NULL) free(range->index);
	free(range);

	if(range2 != NULL) free(range2->index);
	free(range2);

	destroyTree(tree);
	return 0;
}
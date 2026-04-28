#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct node{
	node* parent = nullptr;
	node* left = nullptr;
	node* right = nullptr;
	char color = 'R';
	int value;
};
void printTree(node*, int);
void addNode(node* &, node* &, node* &);
void update(node*&, node*);
node* getUncle(node*);
node* getGrandfather(node*);
node* getRoot(node*);
node* rotateLeft(node*);
node* rotateRight(node*);

#endif

#include <iostream>
#include <string>
#include <fstream>
#include "functions.h"
using namespace std;

void printTree(node* current, int depth){
	//Displays the tree using recursion
	node* left = current->left;
	node* right = current->right;
	if (right != nullptr){
		printTree(right,depth+1);
	}
	for (int i = 0;i< depth;i++){
		cout << "\t";
	}
	if (current != nullptr){
		if (current->parent){
			cout << current->value << " " << current->color << " " << current->parent->value << endl;
		}
		else{
			cout << current->value << " " << current->color << " N/A" << endl;
		}
	}
	if (left != nullptr){
		printTree(left, depth+1);
	}
}

node* getGrandfather(node* curr){
	//Standard: Get's grandfather
	return curr->parent->parent;
}
node* getUncle(node* curr){
	//Standard, gets uncle or sibling of parent
	node* grandfather = getGrandfather(curr);
	if (grandfather){
		if (curr->parent == grandfather->left){
			return grandfather->right;
		}
		else{
			return grandfather->left;
		}
	}
	else{
		return nullptr;
	}
}

node* getRoot(node* current){
	//Go up the tree until you find root
	if (current->parent != nullptr){
		return getRoot(current->parent);
	}
	else{
		return current;
	}
}

node* rotateLeft(node* current){

	//Define right and rightLeft nodes for this case
	node* right = current->right;
	node* rightLeft = right->left;

	//Relink for left rotation
	right->left = current;
	current->right = rightLeft;
	right->parent = current->parent;

	//If not root, link to parent appropriately
	if (current->parent){
		if (current->value <= current->parent->value){
			current->parent->left = right;
		}
		else{
			current->parent->right = right;
		}
	}
	current->parent = right;

	//Links final rightLeft node
	if (rightLeft){
		rightLeft->parent = current;
	}

	//Return root (root may be changed so passed by ref)
	return getRoot(right);
}

node* rotateRight(node* current){

	//Define left and leftRight nodes for this case
	node* left = current->left;
	node* leftRight = left->right;

	//Relink for right rotation
	left->right = current;
	current->left = leftRight;
	left->parent = current->parent;

	//If not root, link to parent appropriately
	if (current->parent){
		if (current->value <= current->parent->value){
			current->parent->left = left;
		}
		else{
			current->parent->right = left;
		}
	}
	current->parent = left;

	//Links final leftRight node
	if (leftRight){
		leftRight->parent = current;
	}

	//Return root (root may be changed so passed by ref)
	return getRoot(left);
}

void update(node*& root, node* addedNode){ //test

	//Base cases
	if (addedNode == root) {root->color = 'B'; return;}
	if (addedNode->parent->parent == nullptr){return;}

	//Recursive case
	if (addedNode->parent->color == 'R'){
		node* uncle = getUncle(addedNode);

		//Uncle is red case
		if (uncle && uncle->color == 'R'){
			uncle->color = 'B';
			addedNode->parent->color = 'B';
			addedNode->parent->parent->color = 'R';
			update(root, addedNode->parent->parent);
		}

		//Uncle is black case
		else if ((uncle && uncle->color == 'B') or uncle == nullptr){
			
			//Define parent and grandparent
			node* grandparent = getGrandfather(addedNode);
			node* parent = addedNode->parent;

			//Define bools to figure out case
			bool leftParent = addedNode->parent->value <= grandparent->value; 
			bool leftChild = addedNode->value <= parent->value;

			//Rotate in accordance with each case
			if (leftParent && leftChild){ // LL case
				root = rotateRight(grandparent);
			}
			else if (leftParent && !leftChild){ //LR
				root = rotateLeft(parent);
				root = rotateRight(grandparent);
			}
			else if (!leftParent && !leftChild){ //RR
				root = rotateLeft(grandparent);
			}
			else if (!leftParent && leftChild){ //RL
				root = rotateRight(parent);
				root = rotateLeft(grandparent);
			}

			//Overlapping color updates associated with each case
			char grandColor = grandparent->color;
			if (!(leftParent ^ leftChild)){
				grandparent->color = parent->color;
				parent->color = grandColor;
			}
			else{
				grandparent->color = addedNode->color;
				addedNode->color = grandColor;
			}
		}
	}
	
}
void addNode(node* &curr, node* &newNode, node* &root){
	//If adding the root
	if (curr == nullptr){
		newNode->color = 'B';
		curr = newNode;
	}
	//Add the node to appropriate location, and then update the tree
	else{
		node* current = curr;
		if (newNode->value <= current->value){
			if (current->left == nullptr){
				current->left = newNode;
				newNode->parent = current;
				update(root, newNode);
			}
			else{
				addNode(current->left, newNode, root);
			}
		}
		else{
			if (current->right == nullptr){
				current->right = newNode;
				newNode->parent = current;
				update(root, newNode);
			}
			else{
				addNode(current->right, newNode, root);
			}
		}
	}

}

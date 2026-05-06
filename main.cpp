#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "functions.h"

//Function prototypes
node* returnSuccessor(node*);
void deleteNode(node* &, int);
void replaceNode(node*, node*);
node* doesExist(node*, int);
node* getSibling(node*);
node* getFarNephew(node*);
node* getNearNephew(node*);
void deleteUpdate(node*, node*, node*&);
void deleteRecolor(node*&, node*, node*, node*);

int main(){

	//Basic var instantiation
	string command;
	node* root = nullptr;
	bool runProgram = true;
	//Program runs
	while (runProgram){
		cout << "\nEnter Command(ADD, PRINT, READ, SEARCH, QUIT): ";
		cin >> command;
		
		//Command recognition
		if (command == "ADD"){ //Adds node objects

			//Get value
			int value;
			cout << "Value: ";
			cin >> value;

			//Create node
			node* addMe = new node();
			addMe->value = value;

			addNode(root, addMe, root);
		}
		else if (command == "READ"){ //Adds file nodes to tree
			//Adds file
			string fileName;
			cout << "Please enter filename: " << endl;
			cin >> fileName;
			
			ifstream numFile(fileName);

			if (!numFile) {
				cout << "Invalid file name." << endl;
			}
			else{
				int number;
				while (numFile >> number) {
					node* addMe = new node();
					addMe->value = number;
					addNode(root, addMe, root);
			
				}
				numFile.close();
			}

		}
		else if (command == "DELETE"){
			int value;
			cout << "Value: ";
			cin >> value;
			node* searchedNode = doesExist(root, value);
			if (!searchedNode){cout << "Cannot delete nonexistent node." << endl;}
			else{
				deleteNode(root, value);
			}
		}
		else if (command == "SEARCH"){ //searches if a node exists in tree
			int value;
			cout << "Value: ";
			cin >> value;
			node* searchedNode = doesExist(root,value);
			if (root && searchedNode){cout << "Node exists." << endl;}
			else {cout << "Node dosen't exist." << endl;}
		}
		else if (command == "PRINT"){ //Prints our tree
			if (root != nullptr){
				printTree(root, 0);
			}
		}
		else if (command == "QUIT"){
			//Exits search tree
			runProgram = false;
		}
		else{
			cout << "Command not recognized" << endl;
		}
	}		

	return 0;
}

//Gets successor
node* returnSuccessor(node* original){
	node* current = original->right;
	while (current && current->left != nullptr){
		current = current->left;
	}
	return current;
}

//replaces node
void replaceNode(node* current, node* replacingNode){
	if (current->value <= current->parent->value){
		current->parent->left = replacingNode;
	}
	else{
		current->parent->right = replacingNode;
	}
}

//Returns if a node of given value exists
node* doesExist(node* current, int value){
	if (current->value == value){
		return current;
	}
	else{
		if (value < current->value){
			if (current->left == nullptr){
				return nullptr;
			}
			else{
				return doesExist(current->left, value);
			}
		}
		else{
			if (current->right == nullptr){
				return nullptr;
			}
			else{
				return doesExist(current->right, value);
			}
		}
	}
}

node* getSibling(node* current){
	if (current->parent->left != current){
		return current->parent->left;
	}
	else{
		return current->parent->right;
	}
}

node* getFarNephew(node* current){
	node* sibling = getSibling(current);
	if (current->parent->right == sibling){
		return sibling->right;
	}
	else if (current->parent->left == sibling){
		return sibling->left;
	}
}

node* getNearNephew(node* current){
	node* sibling = getSibling(current);
	if (current->parent->right == sibling){
		return sibling->left;
	}
	else if (current->parent->left == sibling){
		return sibling->right;
	}
}


void deleteNode(node* &root, int value){
	node* deleteMe = doesExist(root,value);
	
	if (deleteMe != nullptr){
		//No child case
		if (deleteMe->left == nullptr && deleteMe->right == nullptr){
			if (deleteMe->parent != nullptr){
				replaceNode(deleteMe, nullptr);
				deleteUpdate(deleteMe, returnSuccessor(deleteMe), root);
			}
			else{
				root = nullptr;
			}
		}
		//One child cases
		else if (deleteMe->right == nullptr){
			node* successor = deleteMe->left;
			successor->parent = deleteMe->parent;
			if (successor->parent != nullptr){
				replaceNode(deleteMe, successor);
			}
			else{
				root = successor;
			}
			deleteUpdate(deleteMe, successor, root);
		}
		else if (deleteMe->left == nullptr){
			node* successor = deleteMe->right;
			successor->parent = deleteMe->parent;
			if (successor->parent != nullptr){
				replaceNode(deleteMe, successor);
			}
			else{
				root = successor;
			}
			deleteUpdate(deleteMe, successor, root);
		}
		//Two child case
		else{
			//Get successor
			node* successor = returnSuccessor(deleteMe);

			//Set successor's parent to not recognize child
			replaceNode(successor, nullptr);

			//If there are right nodes of successor, make the right node take successor's place
			if (successor->right != nullptr){
				replaceNode(successor, successor->right);
				successor->right->parent = successor->parent;
				successor->right = nullptr;
			}

			//Put successor into place of current in tree
			successor->parent = deleteMe->parent;
			successor->left = deleteMe->left;
			successor->left->parent = successor;
			if (deleteMe->right != nullptr){
				successor->right = deleteMe->right;
				successor->right->parent = successor;
			}
			else{
				successor->right = nullptr;
			}
			if (successor->parent != nullptr){
				replaceNode(deleteMe,successor);
			}
			else{
				root = successor;
			}
			deleteUpdate(deleteMe, successor, root);
		}
		delete deleteMe;
	}
}
void deleteRecolor(node*& root, node* parent, node* sibling, node* successor){
	sibling->color = 'R';
	if (parent->color == 'R'){
		parent->color = 'B';
		successor->color = 'B'; //just added
	}
	else{
		if (parent != root){
			parent->color = 'b';
			successor->color = 'B'; //just added
			deleteRecolor(root, parent->parent, getSibling(parent), parent);
		}
		else{
			successor->color = 'B';
		}
	}	
}
void deleteUpdate(node* deleteMe, node* successor, node*& root){
	if (!successor) {successor = new node(); successor->color = 'B'; successor->parent = deleteMe;}
	//Double black case
	if (deleteMe->color == 'B' && (successor == nullptr || successor->color == 'B')){
		successor->color = 'b';
		while (successor->color == 'b' && successor != root){
			node* sibling = getSibling(successor);
			if (!sibling){
				sibling = new node();
				sibling->parent = deleteMe;
				sibling->color = 'B';
			}

			if (sibling->color == 'B' and ((sibling->left && sibling->left->color == 'R') || (sibling->right && sibling->right->color == 'R'))){
				node* redChild = getFarNephew(successor);
				//LL case
				if (sibling == sibling->parent->left && (sibling->left == redChild || (sibling->left->color == 'R' && sibling->right->color == 'R'))){
					root = rotateRight(sibling->parent);
				}
				//LR case
				else if (sibling == sibling->parent->left && sibling->right == redChild){
					root = rotateLeft(sibling);
					root = rotateRight(successor->parent);
				}
				//RR case
				else if (sibling == sibling->parent->right && (sibling->right == redChild || (sibling->left->color == 'R' && sibling->right->color == 'R'))){
					root = rotateLeft(sibling->parent);
				}
				//RL case
				else if (sibling->parent->right == sibling && sibling->left == redChild){
					root = rotateRight(sibling);
					root = rotateLeft(successor->parent);
				}
			}
			else if (sibling->color == 'B' && ((sibling->left && sibling->left->color == 'B') || !sibling->left) && ((sibling->right && sibling->right->color == 'B') || !sibling->right)){
				deleteRecolor(root, successor->parent, sibling, successor); //Recolors tree
			}
			else if (sibling->color == 'R'){
				if (sibling == sibling->parent->left){
					root = rotateRight(sibling->parent);
				}
				else if (sibling == sibling->parent->right){
					root = rotateLeft(sibling->parent);
				}
				deleteRecolor(root, successor->parent, sibling, successor);
			}
		}
		if (successor == root){
			successor->color = 'B';
		}
	}
	//Red-black case
	else if ((deleteMe->color == 'R' && (successor || successor->color == 'B')) || (deleteMe->color == 'B' && (successor && successor->color == 'R'))){
		successor->color = 'B';
	}
}


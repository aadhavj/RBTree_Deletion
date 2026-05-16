#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "functions.h"

//Function prototypes
node* returnSuccessor(node*);
void deleteNode(node* &, node*);
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
				deleteNode(root, searchedNode);
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
	if (current == current->parent->left){
		current->parent->left = replacingNode;
	}
	else{
		current->parent->right = replacingNode;
	}
}

//Returns if a node of given value exists
node* doesExist(node* current, int value){
	if (!current) {return nullptr;}
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
	if (current == current->parent->left){
		return current->parent->right;
	}
	else{
		return current->parent->left;
	}
}

node* getFarNephew(node* current){
	node* sibling = getSibling(current);
	if (sibling == nullptr){
		cout << "Sibling is nullptr";
	}
	if (current->parent->right == sibling){
		return sibling->right;
	}
	else{
		return sibling->left;
	}
}

node* getNearNephew(node* current){
	node* sibling = getSibling(current);
	if (current->parent->right == sibling){
		return sibling->left;
	}
	else{
		return sibling->right;
	}
}


void deleteNode(node* &root, node* searchedNode){
	node* deleteMe = searchedNode;
	
	if (deleteMe != nullptr){
		//No child case
		if (deleteMe->left == nullptr && deleteMe->right == nullptr){
			if (deleteMe->parent != nullptr){
				//Create dummy successor node
				node* successor = new node();
				successor->color = 'B';
				successor->parent = deleteMe->parent;

				//Parent now recognizes dummy successor instead of deleteMe
				replaceNode(deleteMe, successor);
				
				//Call update
				deleteUpdate(deleteMe, successor, root);

				replaceNode(successor, nullptr);
				delete successor;
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
			
			deleteMe->value = successor->value;

			deleteNode(root, successor);
	
			/*			
			//If there are right nodes of successor, make the right node take successor's place
			if (successor->right != nullptr){
				replaceNode(successor, successor->right);
				successor->right->parent = successor->parent;
				successor->right = nullptr;
			}
			*/
			
			
			cout << "BP HERE" << endl;
			

		}
		//delete deleteMe;
	}
}

void deleteUpdate(node* deleteMe, node* successor, node*& root){
	
	//Simple Case : Red-black case
	if ((deleteMe->color == 'R' && (!successor || successor->color == 'B')) || (deleteMe->color == 'B' && (successor && successor->color == 'R'))){
		successor->color = 'B';
	}
	//Double black cases
	else if (deleteMe->color == 'B' && (successor == nullptr || successor->color == 'B'))
	{
		
		cout << "BB case initiatied: " << deleteMe->value << " and " << successor->value << " are both black" << endl;
		
		successor->color = 'b';
		
		while (successor->color == 'b' && successor != root){

			printTree(root, 0);

			//Create dummy sibling
			node* sibling = getSibling(successor);
			if (sibling == nullptr){
				sibling = new node();
				sibling->parent = successor->parent;
				sibling->color = 'B';
			}

			cout << "Sibling is " << sibling->value << ", with a successor of " << successor->value << endl;

			if (sibling->color == 'B'){ //sibling black
				
				//Atleast one nephew is red
				if ((sibling->left && sibling->left->color == 'R') || (sibling->right && sibling->right->color == 'R')){
					
					//Get far nephew if exists, else near nephew
					node* redChild = getFarNephew(successor);
					if (redChild == nullptr){
						redChild = getNearNephew(successor);
					}

					if (redChild == nullptr){
						cout << "What the heck happened?" << endl;
						cout << deleteMe->value << " has no red nephew." << endl;
					}

					cout << "ROTATIONS STARTED";
					bool siblingIsLeftChild = (sibling == sibling->parent->left);
					bool nephewIsLeftChild = (sibling->left == redChild);

					//LL or RR case
					if (siblingIsLeftChild == nephewIsLeftChild){
						sibling->color = sibling->parent->color;
						sibling->parent->color = 'B';
						redChild->color = 'B';
						if (siblingIsLeftChild && nephewIsLeftChild){
							cout << "Left left performed. Right rotating " << sibling->parent->value << endl;
							root = rotateRight(sibling->parent);
						}
						else{
							cout << "Right Right performed. Left rotating " << sibling->parent->value << endl;
							root = rotateLeft(sibling->parent);
						}
						successor->color = 'B';
					}
					//LR or RL
					else{
						sibling->color = 'R';
						redChild->color = 'B';
						if (siblingIsLeftChild && !nephewIsLeftChild){ //LR
							cout << "Left right performed. Left rotating " << sibling->value << " Right rotating " << successor->parent->value << endl;
							root = rotateLeft(sibling);
							root = rotateRight(successor->parent);}
						else{ //RL
		
							cout << "Right Left performed. Right rotating " << sibling->value << " and left rotating " << successor->parent->value << endl;
							root = rotateRight(sibling);
							root = rotateLeft(successor->parent);}
					}
					
					cout << "Rotations completed" << endl;
				} 
				//Both nephews are black
				else if ((!sibling->left || sibling->left->color == 'B') && (!sibling->right || sibling->right->color == 'B')){
					cout << "Both nephews are black" << endl;

					//Recoloring, push up blackness
					sibling->color = 'R';
					if (sibling->parent->color == 'R'){
						sibling->parent->color = 'B';
						successor->color = 'B';
					}
					else{
						sibling->parent->color = 'b';
						successor->color = 'B';
						successor = sibling->parent;
					}
				}
			}
			else if (sibling->color == 'R'){ //sibling red
				
				cout << "SIBLING RED, PERFORMING ROTATIONS" << endl;
				

				sibling->color = 'B';
				sibling->parent->color = 'R';

				if (sibling == sibling->parent->left){
					
					cout << "RIGHT ROTATING " << sibling->parent->value << endl;
					root = rotateRight(sibling->parent);
				}
				else if (sibling == sibling->parent->right){
					
					cout << "LEFT ROTATING " << sibling->parent->value << endl;
					root = rotateLeft(sibling->parent);
				}

				sibling = getSibling(successor);

			}
		}
		if (successor == root){
			successor->color = 'B';
		}
	}
}


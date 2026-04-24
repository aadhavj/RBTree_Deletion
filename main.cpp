#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//Important node structure
struct node{
	node* parent = nullptr;
	node* left = nullptr;
	node* right = nullptr;
	char color = 'R';
	int value;
};
#include "functions.cpp"

//Function prototypes
node* returnSuccessor(node*);
void deleteNode(node* &, int);
void replaceNode(node*, node*);
node* doesExist(node*, int);

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
	while (current->left != nullptr){
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

void deleteNode(node* &root, int value){
	node* deleteMe = doesExist(root,value);
	if (deleteMe != nullptr){
		//No child case
		if (deleteMe->left == nullptr && deleteMe->right == nullptr){
			if (deleteMe->parent != nullptr){
				replaceNode(deleteMe, nullptr);
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
		}
		delete deleteMe;
	}
}

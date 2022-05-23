#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

/*
 * Author: Fiona Wang
 * Date: April 18, 2022
 * This program takes an input from the console or a file and builds a red black tree
 * This code uses parts from the GeeksforGeeks red black tree page
 */

//Class for the nodes used in the binary tree
class BNode {
  int data;
  char color;
  BNode* left;
  BNode* right;
  BNode* parent;
  

public:
  //Constructors
  BNode(int newData){
    data = newData;
    color = 'R';
    left = NULL;
    right = NULL;
    parent = NULL;
  }

  BNode(){
    color = 'R';
    left = NULL;
    right = NULL;
    parent = NULL;
  }

  //Set and get functions
  void setLeft(BNode* newLeft){
    left = newLeft;
  }

  void setRight(BNode* newRight){
    right = newRight;
  }

  BNode* getLeft(){
    return left;
  }

  BNode* getRight(){
    return right;
  }

  void setData(int newData){
    data = newData;
  }

  int getData(){
    return data;
  }

  void setColor(char newColor){
    color = newColor;
  }
  
  char getColor(){
    return color;
  }

  void setParent(BNode* newParent){
    parent = newParent;
  }

  BNode* getParent(){
    return parent;
  }
};

//Declaring the functions
void addConsole(BNode* &root, bool &LL, bool &RR, bool &LR, bool &RL);
void addFile(BNode* &root, bool &LL, bool &RR, bool &LR, bool &RL);
BNode* add(BNode* &root, int data, bool &LL, bool &RR, bool &LR, bool &RL);
void printTree(BNode* root,  int count);
int search(BNode* root, int searchNum);
BNode* deleteOne(BNode* root, int deleteNum);
BNode* nextValue(BNode* root);
void checkTree(BNode* &root, BNode* &node);
void rotateLeft(BNode* &root, BNode* &node);
void rotateRight(BNode* &root, BNode* &node);
void checkDelete(BNode* &actualRoot, BNode* &root, BNode* &node);

int main() {
  //Initializing variables
  BNode* root = NULL;
  //root = NULL;
  char command[15];
  char fileType[10];
  bool running = true;
  bool LL = false;
  bool RR = false;
  bool LR = false;
  bool RL = false;

  //Ask the user for a command
  cout << "Enter a command (enter add, search, print, or quit)" << endl;
  while(running == true){
    cin >> command;
    
    //Allow both upper and lower case
    for(int i=0; i < strlen(command); i++) {
      command[i] = toupper(command[i]);
    }
    
    if(strcmp(command, "ADD") == false){
      cout << "Enter a input type (console or file)" << endl;
      cin >> fileType;
      
      //Allow both upper and lower case
      for(int i=0; i < strlen(fileType); i++) {
	fileType[i] = toupper(fileType[i]);
      }

      //Ask the user if they want to add from the console or a file
      if(strcmp(fileType, "CONSOLE") == false){
	addConsole(root, LL, RR, LR, RL);
      }
      else if(strcmp(fileType, "FILE") == false){
	addFile(root, LL, RR, LR, RL);
      }
      else{
	cout << "Not a valid file type" << endl;
      }

      cout << "Enter a command (enter add, search, print, or quit)" << endl;
    }
    else if(strcmp(command, "SEARCH") == false){
      //Ask what number to fine
      cout << "What number do you want to find?" << endl;
      int searchNum;
      cin >> searchNum;
      int found = search(root, searchNum);
      //Search for the number
      if(found == searchNum)
	cout << searchNum << " exists in the tree!" << endl;
      else
	cout << searchNum << " does not exist in the tree!" << endl;
      cout << "Enter a command (enter add, search, print, or quit)" << endl;
    }
    else if(strcmp(command, "DELETE") == false){
      //Ask what number to delete
      cout << "What number do you want to delete?" << endl;
      int deleteNum;
      cin >> deleteNum;
      int found = search(root, deleteNum);
      int rootNum = root->getData();
      //Check if the number exists in the tree
      if(found == deleteNum)
        root = deleteOne(root, deleteNum);
      else
        cout << deleteNum << " does not exist in the tree!" << endl;

	cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
    }
    else if(strcmp(command, "PRINT") == false){
      //Print the whole tree
      printTree(root, 0);
      cout << "Enter a command (enter add, search, print, or quit)" << endl;
      //root=rotateRight(root);
    }
    else if(strcmp(command, "QUIT") == false){
      running = false;
    }
    else{
      cout << "Please enter a valid command:" << endl;
    }
  }

  return 0;
}

//Function to take an input from a file
void addFile(BNode* &root, bool &LL, bool &RR, bool &LR, bool &RL){
  char* fileName = new char[20];

  //Ask the user to enter the file name
  cout << "Enter the name of the file: (include the '.txt')" << endl;
  cin >> fileName;
  fstream file;
  file.open(fileName);

  //Extract numbers from the file
  int input = 0;
  while(file >> input){
    BNode* current = new BNode;
    current = add(root, input, LL, RR, LR, RL);
    checkTree(root, current);
  }
  file.close();
}

//Function to take an input from the console
void addConsole(BNode* &root, bool &LL, bool &RR, bool &LR, bool &RL){
  bool entering = true;
  int input;

  //Ask the user for numbers or the word "done"
  cout << "Enter a number (1-999)" << endl;
  cin >> input;
  BNode* current = new BNode;
  current = add(root, input, LL, RR, LR, RL);
  checkTree(root, current);
}

//Function to add into the tree
BNode* add(BNode* &root, int data, bool &LL, bool &RR, bool &LR, bool &RL){
  //Check if you are adding at the root
  if(root==NULL) {
    BNode* newRoot = new BNode(data);
    root = newRoot;
    return root;
  }
  else {
    bool isRedRed = false;
    //Check if the new node should go to the right or left
    //If to the left, check if it is at the base of the tree
    if(data < root->getData() && root->getLeft()){
      BNode* temp = root->getLeft();
      return add(temp, data, LL, RR, LR, RL);
    }
    //Add the new node
    else if(data < root->getData() && !root->getLeft()){
      BNode* newNode = new BNode(data);
      root->setLeft(newNode);
      newNode->setParent(root);
      return newNode;
    }

    //If to the right, check if it is at the base of the tree
    if(data > root->getData() && root->getRight()){
      BNode* temp = root->getRight();
      return add(temp, data, LL, RR, LR, RL);
    }
    //Add the new node
    else if(data > root->getData() && !root->getRight()){
      BNode* newNode = new BNode(data);
      root->setRight(newNode);
      newNode->setParent(root);
      return newNode;
    }
  }
  return NULL;
}

//Function to check if the tree violates any of the RBT properties
void checkTree(BNode* &root, BNode* &node){
  //Initialize the parent, grandparent, and uncle
  BNode* parent = NULL;
  BNode* grandpa = NULL;
  BNode* uncle = NULL;

  //While loop to ensure that the tree is perfect
  while((node != root) && (node->getColor() == 'R') && (node->getParent()->getColor() == 'R')) {
    parent = node->getParent();
    grandpa = node->getParent()->getParent();
    if(parent == grandpa->getLeft()) { //If the parent is the left child
      uncle = grandpa->getRight();
      if(uncle && uncle->getColor() == 'R'){ //If the uncle is red (case 3)
	grandpa->setColor('R');
	parent->setColor('B');
	uncle->setColor('B');
	node = grandpa;
      }
      else { //If the uncle is not red
	if(node == parent->getRight()) { //Needs a left rotation (case 4)
	  rotateLeft(root, parent);
	  node = parent;
	  parent = node->getParent();
	}
	//Needs a right rotation (case 5)
	rotateRight(root, grandpa); 
	parent->setColor('B');
	grandpa->setColor('R');
	node = parent;
      }
    }
    else { //If the parent is the right child
      uncle = grandpa->getLeft();
      if(uncle != NULL && uncle->getColor() == 'R'){ //If the uncle is red (case 3)
        grandpa->setColor('R');
        parent->setColor('B');
        uncle->setColor('B');
        node = grandpa;
      }
      else { //If the uncle is not red
        if(node == parent->getLeft()){ //Needs a right rotation (case 4)
          rotateRight(root, parent);
          node = parent;
          parent = node->getParent();
        }
	//Needs a left rotation (case 5)
        rotateLeft(root, grandpa);
        parent->setColor('B');
	grandpa->setColor('R');
        node = parent;
      }
    }
  }
  //Make sure the root is black
  root->setColor('B');
}

//Function to print out the binary search tree
void printTree(BNode* root, int count) {
  //Check if there is anything in the tree
  if(!root || root->getData() == -1){
    cout << "The tree is empty!" << endl;
    return;
  }

  //Go through the right side of the tree
  if(root->getRight())
    printTree(root->getRight(), count+1);

  //Print with right amount of tabs
  for(int i=0; i<count; i++){
    cout << "\t";
  }
  if(root->getColor() == 'B')
    cout << root->getData() << endl;
  else
    cout << "\033[1;31m" << root->getData() << "\033[0m" << endl;

  //Go through the left side of the tree
  if(root->getLeft())
    printTree(root->getLeft(), count+1);
}

//Function to check if a certain number exists in the tree
int search(BNode* root, int searchNum) {
  //If the number has been found or the tree is empty
  if(!root || root->getData() == searchNum){
    return root->getData();
  }

  //If the number is bigger, then traverse to the right
  if(root->getData() < searchNum){
    //If you are at the bottom of the tree
    if(!root->getRight())
      return root->getData();
    return search(root->getRight(), searchNum);
  }

  //If the number is smaller, then traverse to the left
  else {
    //If you are at the bottom of the tree
    if(!root->getLeft())
      return root->getData();
    return search(root->getLeft(), searchNum);
  }
}

//Function to delete a specific node in the tree
BNode* deleteOne(BNode* actualRoot, BNode* root, int deleteNum){
  bool dB = false;
  //If the tree is empty
  if(!root){
    return root;
  }
  //If the number is bigger, traverse to the right node
  if(root->getData() < deleteNum){
    root->setRight(deleteOne(root->getRight(), deleteNum));
  }
  //If the number is smaller, traverse to the left node
  else if(root->getData() > deleteNum){
    root->setLeft(deleteOne(root->getLeft(), deleteNum));
  }

  //If the correct node is found
  else if(root->getData() == deleteNum){
    //If it has no children nodes
    if(!root->getLeft() && !root->getRight()){
      checkDelete(actualRoot, root);
      return NULL;
    }

    //If it has one child node (left or right)
    else if(!root->getLeft()){
      BNode* temp = root->getRight();
      if(root->getColor() == 'R' || root->getRight()->getColor() == 'R'){
	temp->setColor('B');
      }
      else {
	dB = true;
      }
      temp->setParent(root->getParent());
      delete root;
      if(dB)
	checkDelete(actualRoot, temp);
      return temp;
    }
    else if(!root->getRight()){
      BNode* temp = root->getLeft();
      if(root->getColor() == 'R' || root->getLeft()->getColor() == 'R'){
        temp->setColor('B');
      }
      else {
	dB = true;
      }
      temp->setParent(root->getParent());
      delete root;
      if(dB)
	checkDelete(actualRoot, temp);
      return temp;
    }

    //If it has two children
    BNode* temp = nextValue(root->getRight());
    root->setData(temp->getData());
    root->setRight(deleteOne(root->getRight(), temp->getData()));
  }
  return root;
}

void checkDelete(BNode* &actualRoot, BNode* &root, BNode* &node){
  cout << "db" << endl;
}

//Function to find the next smallest number in the tree
BNode* nextValue(BNode* root){
  BNode* current = root;
  //Traverse to the bottom of the tree
  while(current && current->getLeft()){
    current = current->getLeft();
  }
  return current;
}

//Function to rotate the tree right
//Referenced from GeeksforGeeks
void rotateRight(BNode* &root, BNode* &node){
  BNode* y = node->getLeft();
  node->setLeft(y->getRight());
  if(node->getLeft()) //Update the node's parent
    node->getLeft()->setParent(node);
  y->setParent(node->getParent());
  if(!node->getParent())
    root = y; //Update the root
  else if(node == node->getParent()->getLeft()) //If is the left child
    node->getParent()->setLeft(y);
  else //If is the right child
    node->getParent()->setRight(y);
  //Update the node's child
  y->setRight(node);
  node->setParent(y);
}

//Function to rotate the tree left
//Referenced from GeeksforGeeks
void rotateLeft(BNode* &root, BNode* &node){
  BNode* y = node->getRight();
  node->setRight(y->getLeft());
  if(node->getRight()) //Update the node's parent
    node->getRight()->setParent(node);
  y->setParent(node->getParent());
  if(!node->getParent()) //Update the root
    root = y;
  else if(node == node->getParent()->getLeft()) //If is left child
    node->getParent()->setLeft(y);
  else //If is right child
    node->getParent()->setRight(y);
  //Update the node's child
  y->setLeft(node);
  node->setParent(y);
}

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

/*
 * Author: Fiona Wang
 * Date: April 18, 2022
 * This program takes an input from the console or a file and builds a binary search tree
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
void addConsole(BNode* &root);
void addFile(BNode* &root);
void add(BNode* root, int data);
void printTree(BNode* root,  int count);
int search(BNode* root, int searchNum);
//void deleteOne(BNode* tree, BNode* previous, int deleteNum, int root);
BNode* deleteOne(BNode* root, int deleteNum);
BNode* nextValue(BNode* root);
void checkTree(BNode* root);
BNode* rotateLeft(BNode* node);
BNode* rotateRight(BNode* node);

int main() {
  //Initializing variables
  BNode* root = new BNode(-1);
  char command[15];
  char fileType[10];
  bool running = true;

  //Ask the user for a command
  cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
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
	addConsole(root);
      }
      else if(strcmp(fileType, "FILE") == false){
	addFile(root);
      }
      else{
	cout << "Not a valid file type" << endl;
      }

      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
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
      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
    }
    else if(strcmp(command, "DELETE") == false){
      /*//Ask what number to delete
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

	cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;*/
    }
    else if(strcmp(command, "PRINT") == false){
      //Print the whole tree
      printTree(root, 0);
      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
      root=rotateRight(root);
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
void addFile(BNode* &root){
  char* fileName = new char[20];

  //Ask the user to enter the file name
  cout << "Enter the name of the file: (include the '.txt')" << endl;
  cin >> fileName;
  fstream file;
  file.open(fileName);

  //Extract numbers from the file
  int input = 0;
  while(file >> input){
    add(root, input);
  }
  file.close();
}

//Function to take an input from the console
void addConsole(BNode* &root){
  bool entering = true;
  int input;

  //Ask the user for numbers or the word "done"
  cout << "Enter a number (1-999) or enter \"done\"" << endl;
  cin >> input;
  add(root, input);
  /*
  //Keep asking for input until the user enters "done"
  while(entering){
    cin >> input;
    //Allow both upper and lower case
    for(int i=0; i < strlen(input); i++) {
      input[i] = toupper(input[i]);
    }

    //Check if the input is a number or "done"
    bool isNum = true;
    for(int i=0; i<strlen(input); i++){
      if(!isdigit(input[i])){
	isNum = false;
      }
    }

    //If the user is done, leave the loop
    if(strcmp(input, "DONE") == false){
      entering = false;
    }
    //Keep asking for numbers
    else if(!isNum){
      cout << "Enter a number (1-999) or enter \"done\"" << endl;
    }
    else{
      int num = atoi(input);
      add(tree, num);
      cout << "Enter a number (1-999) or enter \"done\"" << endl;
    }
    }*/
}


//Function to add into the tree
void add(BNode* root, int data){
  //Check if you are adding at the root
  if(root->getData() == -1) {
    root->setData(data);
    return;
  }
  else {
    //Check if the new node should go to the right or left
    //If to the left, check if it is at the base of the tree
    if(data < root->getData() && root->getLeft()){
      root = root->getLeft();
      add(root, data);
    }
    //Add the new node
    else if(data < root->getData() && !root->getLeft()){
      BNode* newNode = new BNode(data);
      root->setLeft(newNode);
      return;
    }

    //If to the right, check if it is at the base of the tree
    if(data > root->getData() && root->getRight()){
      root = root->getRight();
      add(root, data);
    }
    //Add the new node
    else if(data > root->getData() && !root->getRight()){
      BNode* newNode = new BNode(data);
      root->setRight(newNode);
      return;
    }
  }
}

void checkTree(BNode* root) {
  
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
  cout << root->getData() << endl;

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
BNode* deleteOne(BNode* root, int deleteNum){
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
      return NULL;
    }

    //If it has one child node (left or right)
    else if(!root->getLeft()){
      BNode* temp = root->getRight();
      delete root;
      return temp;
    }
    else if(!root->getRight()){
      BNode* temp = root->getLeft();
      delete root;
      return temp;
    }

    //If it has two children
    BNode* temp = nextValue(root->getRight());
    root->setData(temp->getData());
    root->setRight(deleteOne(root->getRight(), temp->getData()));
  }
  return root;
}

//Function to fine the next smallest number in the tree
BNode* nextValue(BNode* root){
  BNode* current = root;
  //Traverse to the bottom of the tree
  while(current && current->getLeft()){
    current = current->getLeft();
  }
  return current;
}

BNode* rotateRight(BNode* node){
  BNode* x = node->getLeft();
  BNode* y = x->getRight();
  x->setRight(node);
  node->setLeft(y);
  node->setParent(x);
  if(y)
    y->setParent(node);
  return x;
}

BNode* rotateLeft(BNode* node){
  BNode* x = node->getRight();
  BNode* y = x->getLeft();
  x->setLeft(node);
  node->setRight(y);
  node->setParent(x);
  if(y)
    y->setParent(node);
  return x;
  
  /*BNode* y = x->getRight();
  x->setRight(y->getLeft());
  if(y->getLeft())
    y->getLeft()->setParent(x);
    y->setParent(x->getParent());*/
  //if(!x->getParent()) //x is root
}
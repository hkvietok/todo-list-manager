//Henry Kvietok
//CSCI 2270: Data Structures
//Final Project
//To-List Manager

#ifndef TODOTREE_HPP
#define TODOTREE_HPP
#include <string>


struct ToDoNode //struct for each individual to do item, instances of structs will be store in BST
{
  int urgency; //how urgent the task is to complete
  std::string note; //what the task is (ex: "calc hw")
  std::string creationTime; //when the notes was created

  //pointers to connected notes
  ToDoNode *parent;
  ToDoNode *leftChild;
  ToDoNode *rightChild;

  ToDoNode() //constructor, no parameters
  {
    parent = leftChild = rightChild = nullptr;
  }

  ToDoNode(int u, std::string n, std::string c) //parametrized constructor
  {
    urgency = u;
    note = n;
    creationTime = c;
    parent = leftChild = rightChild = nullptr;
  }
};

class ToDoTree //class for our BST (Binary Search Tree) of to-dos organized by urgency
{
  
public:
  ToDoTree(); //constructor
  ~ToDoTree(); //destructor
  void printAllToDo(); //print entire tree
  void filePrint(std::string name); //print data to file for saving
  void editToDo(int oldUrgency, std::string note, int newUrgency, std::string newNote); //edit a todo item
  void addToDo(int urgency, std::string note, std::string time, bool newTime); //add new item to tree
  void findToDo(int urgency, std::string note); //find a to do
  bool deleteToDo(int urgency, std::string note); //delete from the tree
  void countToDo(); //count up todos
  void setTasks(int com, int curr); //set the number of completed and current tasks from our file data
  int getCompleted(); //get the number of completed tasks
  int getCurrent(); //get the number of current/incomplete tasks
  ToDoNode* getRoot(); //get the root
  void getMaxInfo(); //get the most urgent task's information
  void random(); //find random task in the tree

private:
  ToDoNode *search(int urgency, std::string note); //search for a node
  ToDoNode *findRand(ToDoNode *root, int count); //find a random node
  ToDoNode *root; //root of tree
  int completed; //number of completed to-dos
  int current; //number of current tasks that are not yet completed
};



#endif

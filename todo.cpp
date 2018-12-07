//Henry Kvietok
//CSCI 2270: Data Structures
//Final Project
//To-List Manager

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime> //for getting the current time
#include <stack>

#include "ToDoTree.hpp" //header file

using namespace std;

ToDoTree::ToDoTree() //constructor
{
  root=NULL;
  completed=0;
  current=0;
}


void deleteToDoTree(ToDoNode* root) //recursive function to delete nodes in the tree in postorder
{
    if (root == NULL) //go to end of branch
    {
      return;
    }

    deleteToDoTree(root->leftChild); //go to left tree
    deleteToDoTree(root->rightChild); //go to right tree
    delete root; //delete node
}

ToDoTree::~ToDoTree() //destructor
{
  deleteToDoTree(root); //delete all nodes
  root = NULL; //root is now null
}

ToDoNode* ToDoTree::findRand(ToDoNode *root, int count) //iterative method to find a random node given a random integer between 0 and current-1
{
    stack<ToDoNode *> stack; //stack to store nodes
    ToDoNode *current = root; //set current at root

    while (current != NULL || stack.empty() == false) //go through whole tree
    {
        while (current !=  NULL) //go all the way to the left
        {
            stack.push(current); //put in stack
            current = current->leftChild; //move down to the left again
        }

        //current is now null

        current = stack.top(); //go back up the tree to last node
        stack.pop(); //pop stack

        if (count==0) //we have reached our random node
        {
          return current; //exit and return
        }

        current = current->rightChild; //now we go to the right

        count--; //decrement our random count
    }
}

ToDoNode* ToDoTree::getRoot() //get root (private member)
{
  return root;
}

void ToDoTree::getMaxInfo() //get root info (the most urgent task)
{
  ToDoNode* mostUrgent = getRoot(); //get the root of the tree

  if (mostUrgent==NULL)
  {
    std::cout << "Empty List" << std::endl;
  }

  if (mostUrgent!=NULL)
  {
    while (mostUrgent->rightChild != NULL) //go to most urgent (rightmost child)
    {
      mostUrgent = mostUrgent->rightChild;
    }
    std::cout << std::endl;
    std::cout << "Most Urgent To-Do:" << std::endl;
    std::cout << "===========" << std::endl;
    std::cout << "Urgency: " << mostUrgent->urgency << std::endl;
    std::cout << "Task: " << mostUrgent->note << std::endl;
    std::cout << "Time Created: " << mostUrgent->creationTime<<std::endl;
    std::cout << std::endl;
  }
}

void ToDoTree::random() //random function
{
  int curr=getCurrent(); //current number of nodes in the tree
  srand(time(0)); //seed the function
  if (curr != 0) //tree is not empty
  {
    int randomNum = rand() % curr; //between 0 and current-1
    ToDoNode* randNode = findRand(getRoot(),randomNum); //get random node and print out it's data
    std::cout << std::endl;
    std::cout << "Random To-Do:" << std::endl;
    std::cout << "===========" << std::endl;
    std::cout << "Urgency: " << randNode->urgency << std::endl;
    std::cout << "Task: " << randNode->note << std::endl;
    std::cout << "Time Created: " << randNode->creationTime<<std::endl;
    std::cout << std::endl;
  }
  else
  {
    std::cout << "Empty To-Do List!" << std::endl;
  }
}


void ToDoTree::editToDo(int urgency, std::string note, int newUrgency, std::string newNote) //edit a task
{
  ToDoNode* foundToDo = search(urgency, note); //search for task
  if (foundToDo == NULL) //null returned by search function
  {
    std::cout << "To-Do not found." << std::endl;
  }
  else
  {
    string time = foundToDo->creationTime; //reset the time it was created to now
    deleteToDo(urgency, note); //get rid of old note
    addToDo(newUrgency, newNote, time, true); //make new note
  }
}

void ToDoTree::findToDo(int urgency, std::string title) //find to do in tree
{
  ToDoNode* foundToDo = search(urgency, title); //search for to do
  if (foundToDo == NULL)
  {
    std::cout << "To-Do not found." << std::endl;
  }
  else
  {
    std::cout << std::endl;
    std::cout << "To-Do Info:" << std::endl;
    std::cout << "===========" << std::endl;
    std::cout << "Urgency: " << foundToDo->urgency << std::endl;
    std::cout << "Task: " << foundToDo->note << std::endl;
    std::cout << "Time Created: " << foundToDo->creationTime<<std::endl;
    std::cout << std::endl;
  }
}

ToDoNode* searchToDoTree(ToDoNode* root, int searchUrgency, std::string searchNote) //helper function for search
{
    if (root == NULL || root->note == searchNote) //empty tree (null root), task is the root, or we have reached end of tree and did not find task
    {
      return root;
    }


    if (root->urgency <= searchUrgency) //urgency is lesser or equal to current node
    {
       return searchToDoTree(root->rightChild, searchUrgency, searchNote); //go to the right
    }

    if (root->urgency > searchUrgency) //urgency is greater than current node
    {
      return searchToDoTree(root->leftChild, searchUrgency, searchNote); //go to the left
    }

}

ToDoNode* ToDoTree::search(int urgency, std::string note) //calls recursive helper function above
{
  return searchToDoTree(root, urgency, note); //return node from recursive search function above
}

void ToDoTree::addToDo(int urgency, std::string note, std::string myTime, bool oldTime)
{

  //-----------------------------------------------------

  // from: https://stackoverflow.com/a/16358224
  time_t _tm =time(NULL);

  struct tm * curtime = localtime ( &_tm );

  //-----------------------------------------------------

  if (!oldTime) //this is a brand new todo (not one loaded from the file) so we need the current time
  {
    myTime=asctime(curtime);
  }


  ToDoNode *newToDo = new ToDoNode(urgency, note, myTime); //create new todo node
  ToDoNode *currentNode = root; //set current to root

  newToDo->parent = NULL; //parent is set to NULL

  if (root == NULL) //first todo in tree
  {
      root = newToDo; //set root
      return;
  }
  while (currentNode != NULL)
  {
      newToDo->parent = currentNode; //move down parent
      if (newToDo->urgency <= currentNode->urgency) //urgency is less than or equal to current
      {
        currentNode = currentNode->leftChild; //move down to left
      }
      else //greater urgency than current
      {
        currentNode = currentNode->rightChild; //move down to right
      }
  }

  //We have now reached the leaves

  if (newToDo->urgency <= newToDo->parent->urgency) //urgency is less than or equal to current
  {
    newToDo->parent->leftChild = newToDo; //set left child
  }
  else //greater urgency than current
  {
    newToDo->parent->rightChild = newToDo; //set right child
  }
}

void inOrderTraverse(ToDoNode *root) //in order traversal of tree based on urgency
{
  if(root != NULL) //"go back up" in stack frame
  {
    inOrderTraverse(root->rightChild); //we print max to min urgency
    std::cout << "Task: " << root->note <<std::endl;
    std::cout<< "Urgency: " << root->urgency <<std::endl;
    std::cout<< "Time Created: " << root->creationTime << std::endl <<std::endl;
    inOrderTraverse(root->leftChild);
  }
}

void ToDoTree::printAllToDo()
{
  if (root == NULL)
  {
    std::cout << "No Tasks." << std::endl;
  }
  inOrderTraverse(root); //call recursive function to print out todos
}

void ToDoTree::filePrint(std::string name) //"save" the todos to a file which we can reload in the future
{

  ofstream myFileStream; //output file stream variable
  myFileStream.open(name, std::ofstream::out | std::ofstream::trunc); //open file
  if (myFileStream.is_open()) //successfully opened
  {

    //Print to file
    myFileStream << completed << "," << current << std::endl; //put completed and current numbers at the top

    stack<ToDoNode *> stack;
    ToDoNode *current = root;

    while (current != NULL || stack.empty() == false) //use stack to print entire tree
    {
        while (current !=  NULL) //go all the way to the rightmost node
        {
            stack.push(current); //put in stack
            current = current->rightChild; //go to right child
        }

        //current is now null

        current = stack.top(); //reset current to the previous parent
        stack.pop(); //pop

        myFileStream << current->urgency << "," << current->note << "," << current->creationTime << "\n"; //print to file

        //Note: The way we print means that when we rebuild the tree, the most urgent task will be the root,
        //this allows for a faster access time in the future

        current = current->leftChild; //now go to the left

    }

    std::cout << "To-Do List saved to file. Goodbye!" << std::endl; //Success message
  }
  else //failed to open
  {
    std::cout << "Error! Could not open file to save data" << std::endl; //Error message
  }
  myFileStream.close(); //close file
}

bool ToDoTree::deleteToDo(int deleteUrgency, std::string deleteNote) //delete a todo in the tree
{
  //Nodes for going through and deleting an element of the movie tree

  ToDoNode *temp;
  ToDoNode *tempParent;
  ToDoNode *newParent;
  ToDoNode *temp2;
  ToDoNode *tempChild;

  bool left = false; //true for left, false for right

  //Step 1: Find the node to delete. Using this method as opposed to search function so we know which child (left or right) we have
  tempParent = NULL;
  temp=root; //start at root
  while(temp != NULL) //go through whole tree
  {
     if (deleteNote == temp->note)
     {
       break; // Found the todo to delete
     }
     else if (deleteUrgency <= temp->urgency) //go to left
     {
         tempParent = temp;
         left = true;
         temp = temp->leftChild;
     }
     else //go to right
     {
         tempParent = temp;
         left = false;
         temp = temp->rightChild;
     }
  }

  //Now, our temp is the todo to be deleted, or NULL if it was not found

  if (temp == NULL) //not found
  {
      std::cout << "To-Do not found." << std::endl;
      return false;
  }

  //if here, todo is the movie to delete

   if ((temp->leftChild == NULL) && (temp->rightChild == NULL)) //leaf node
   {
       if (tempParent == NULL) //it is the root
       {
            delete temp;
            root = NULL;
       }
       else //has a parent
       {
             delete temp; //delete the node
             if (left==true) //it was the left child of the parent
             {
               tempParent->leftChild = NULL;
             }
             else //right child
             {
               tempParent->rightChild = NULL;
             }
       }
   }
   else if ((temp->leftChild == NULL) || (temp->rightChild == NULL)) //only 1 child
   {
     if (temp->leftChild != NULL) //has a left child
     {
       tempChild = temp->leftChild;
     }
     else //has a right child
     {
       tempChild = temp->rightChild;
     }
     if (tempParent == NULL) //root
     {
        root = tempChild;
        delete temp;
     }
     else //not the root, the parent of the deleted movie will take the child of the deleted movie
     {
        if (left==true)
        {
          tempParent->leftChild = tempChild;
        }
        else
        {
          tempParent->rightChild =  tempChild;
        }
        delete temp;
      }
   }
   else //both children exist, find max urgency in the left subtree
   {
      newParent = temp;
      temp2 =  temp->leftChild; //go to left so we find the max in the left subtree
      left = true; //left child
      while(temp2->rightChild != NULL) //go down to bottom of right branch, or do not go in this loop at all if it is the left child
      {
        newParent = temp2; //increment
        temp2 = temp2->rightChild;
        left = false; //right child
      }

      //now we have found the max in the left subtree, we now copy the data to our node to delete
      temp->note = temp2->note;
      temp->urgency = temp2->urgency;
      temp->creationTime = temp2->creationTime;

       //now, our todo data has been copied and temp2 is either the left child of temp or is a right child of temp->leftChild

      if (left==true) //left child case
      {
            temp->leftChild = temp2->leftChild; //move the branch up
            delete temp2; //get rid of old node we copied data from
      }
      else //other case
      {
          newParent->rightChild = temp2->leftChild; //move the branch up in order to delete the old node we copied from
          delete temp2;
      }
    }

    std::cout << "Task Completed. Nice Job!" << std::endl;
    return true; //return type for function is void
}

void ToDoTree::setTasks(int com, int curr) //given data from file, set the num of tasks
{
  completed=com;
  current=curr;
}

int ToDoTree::getCompleted()
{
  return completed;
}

int ToDoTree::getCurrent()
{
  return current;
}

int countFunc(ToDoNode* root) //count up nodes recursively
{
  if( root == NULL) //empty
  {
    return 0;
  }
  else //not empty
  {
    if (root->rightChild == NULL && root->leftChild == NULL) //only the root in the tree
    {
      return 1;
    }
    else //not just the root
    {
      return (1 + (countFunc(root->leftChild) + countFunc(root->rightChild))); //1 is the root and then recursively call for right and left children to add up
    }
  }
}

void ToDoTree::countToDo() //count movie function
{
  std::cout<< "Tasks = " << countFunc(root) << std::endl; //print out result from above recursive function
}

int main(int argc, char *argv[])
{

  ToDoTree myTree; //create instance of the class (our BST of todos)

  if (argc != 2) //program name and text file name
  {
    std::cout << "Error with filename."; //no filename or more than one
    return 0;
  }

  std::ifstream inFile; //input file stream object
  inFile.open(argv[1]); //open text file

  if (! inFile) //failed to open
  {
    std::cout << "Error";
    return -1;
  }

  else //is open
  {

    std::string line1;
    getline(inFile, line1);
    std::stringstream ss2; //string stream
    int complete;
    int current;
    ss2<<line1; //put line into string stream
    //parse line by comma
    std::string item2;
    getline(ss2,item2,',');
    int myCompleted = stoi(item2);

    getline(ss2,item2,',');
    int myCurrent = stoi(item2);
    myTree.setTasks(myCompleted,myCurrent); //set the number of tasks from given file data

    while (getline(inFile, line1)) //go through lines
    {
      if (line1 != "")
      {
        std::stringstream ss1; //string stream
        int tempUrgency;
        std::string tempNote;
        std::string tempTime;
        ss1<<line1; //put line into string stream
        //parse line by comma
        std::string item;
        getline(ss1,item,',');
        tempUrgency = stoi(item);
        getline(ss1,item,',');
        tempNote=item;
        getline(ss1,item,',');
        tempTime=item;

        myTree.addToDo(tempUrgency, tempNote, tempTime, true); //create instance of todo struct and add to the tree
      }
    }
  }

  inFile.close(); //open text file


  std::string stringInput = ""; //user's choice
  int input = 0; //integer for input

  //Welcome message
  std::cout<<"Welcome to your To-Do List!"<<std::endl;
  std::cout << "Current: " << myTree.getCurrent() <<std::endl;
  std::cout << "Completed: " << myTree.getCompleted() <<std::endl;

  while (input != 9) //main menu display for user interaction
  {

    std::cout << "======Main Menu======" << std::endl;
    std::cout << "1. Find a Task" << std::endl;
    std::cout << "2. Edit a Task" << std::endl;
    std::cout << "3. View All Tasks" << std::endl;
    std::cout << "4. Complete a Task" << std::endl;
    std::cout << "5. View Task Count" << std::endl;
    std::cout << "6. Add a Task" << std::endl;
    std::cout << "7. Random Task" << std::endl;
    std::cout << "8. Most Urgent" << std::endl;
    std::cout << "9. Exit and Save" << std::endl;

    getline(std::cin, stringInput); //get input
    while (stringInput != "1" && stringInput != "2" && stringInput != "3" && stringInput != "4"&& stringInput != "5"&& stringInput != "6" && stringInput != "7"&& stringInput != "8"&& stringInput != "9")
    {
      std::cout << "Incorrect Menu Input. Try again:"<<std::endl;
      getline(std::cin, stringInput);
    }

    input = stoi(stringInput); //convert to int

    if (input == 1) //find a task
    {
      std::cout << "Enter Urgency (1-10):" << std::endl;
      std::string myUrgency;
      getline(std::cin, myUrgency);
      if (myUrgency == "1" ||myUrgency == "2" ||myUrgency == "3" ||myUrgency == "4" ||myUrgency == "5" ||myUrgency == "6" ||myUrgency == "7" ||myUrgency == "8" ||myUrgency == "9" ||myUrgency == "10")
      {
        int urg = stoi(myUrgency);
        std::cout << "Enter Note:" << std::endl;
        std::string myNote;
        getline(std::cin, myNote);
        myTree.findToDo(urg, myNote);
      }
      else
      {
        std::cout << "Invalid Input :("<< std::endl;
      }
    }

    if (input == 2) //edit a task, requires new and old info for the todo
    {
      std::cout << "Enter Old Urgency (1-10):" << std::endl;
      std::string oldUrgency;
      getline(std::cin, oldUrgency);
      if (oldUrgency == "1" ||oldUrgency == "2" ||oldUrgency == "3" ||oldUrgency == "4" ||oldUrgency == "5" ||oldUrgency == "6" ||oldUrgency == "7" ||oldUrgency == "8" ||oldUrgency == "9" ||oldUrgency == "10")
      {
        int oldUrg = stoi(oldUrgency);
        std::cout << "Enter Old Note:" << std::endl;
        std::string oldNote;
        getline(std::cin, oldNote);
        std::cout << "Enter New Urgency (1-10):" << std::endl;
        std::string newUrgency;
        getline(std::cin, newUrgency);
        if (newUrgency == "1" ||newUrgency == "2" ||newUrgency == "3" ||newUrgency == "4" ||newUrgency == "5" ||newUrgency == "6" ||newUrgency == "7" ||newUrgency == "8" ||newUrgency == "9" ||newUrgency == "10")
        {
          int newUrg = stoi(newUrgency);
          std::cout << "Enter New Note:" << std::endl;
          std::string newNote;
          getline(std::cin, newNote);
          myTree.editToDo(oldUrg,oldNote,newUrg,newNote);
        }
        else
        {
          std::cout << "Invalid Input :("<< std::endl;
        }
      }
      else
      {
        std::cout << "Invalid Input :("<< std::endl;
      }
    }

    if (input == 3) //see all current tasks
    {
      myTree.printAllToDo();
    }

    if (input == 4) //delete a task (it is now completed)
    {
      std::cout << "Enter Urgency (1-10):" << std::endl;
      std::string delUrgency;
      getline(std::cin, delUrgency);
      if (delUrgency == "1" ||delUrgency == "2" ||delUrgency == "3" ||delUrgency == "4" ||delUrgency == "5" ||delUrgency == "6" ||delUrgency == "7" ||delUrgency == "8" ||delUrgency == "9" ||delUrgency == "10")
      {
        int delUrg = stoi(delUrgency);
        std::cout << "Enter Note:" << std::endl;
        std::string delNote;
        getline(std::cin, delNote);
        if (myTree.deleteToDo(delUrg, delNote))
        {
          int newComp = myTree.getCompleted() + 1;
          int newCurr = myTree.getCurrent() - 1;
          myTree.setTasks(newComp, newCurr);
        }
      }
      else
      {
        std::cout << "Invalid Input :("<< std::endl;
      }
    }

    if (input == 5) //see how many tasks you have and how many have been completed
    {
      std::cout << "Current: " << myTree.getCurrent() <<std::endl;
      std::cout << "Completed: " << myTree.getCompleted() <<std::endl;
    }

    if (input == 6) //add new task
    {
      std::cout << "Enter Urgency (1-10):" << std::endl;
      std::string addUrgency;
      getline(std::cin, addUrgency);
      if (addUrgency == "1" ||addUrgency == "2" ||addUrgency == "3" ||addUrgency == "4" ||addUrgency == "5" ||addUrgency == "6" ||addUrgency == "7" ||addUrgency == "8" ||addUrgency == "9" ||addUrgency == "10")
      {
        int addUrg = stoi(addUrgency);
        std::cout << "Enter Note:" << std::endl;
        std::string addNote;
        getline(std::cin, addNote);
        myTree.addToDo(addUrg, addNote, "", false);
        int add = myTree.getCurrent() + 1;
        int comp = myTree.getCompleted();
        myTree.setTasks(comp, add);
      }
      else
      {
        std::cout << "Invalid Input :("<< std::endl;
      }
    }

    if (input == 7) //see a random task
    {
      myTree.random();
    }

    if (input == 8) //see your most urgent task
    {
      myTree.getMaxInfo();
    }

    if (input == 9) //print all the tree data to the original txt file for future use
    {
      std::cout << "Saving your To-List..." << std::endl;

      myTree.filePrint(argv[1]);
    }

  }

  return 0;
}

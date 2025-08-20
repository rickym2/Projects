/* In this MP, I was to create a binary tree that deals with anagrams. In order to do so, the binary.cpp first lays out the BinaryTree class with constructors and various helper functions, both private and public.  
* Due to the nature of trees, the best way to go through solving and creating these functions is through recursion. For instance, insert_node and delete_node both utilize recursive functions to go through each node
* in the tree. In some of the functions, there are two versions of it, one private and one public. For instance, find_node_parent. The reason this is done is to allow us to call a public function without needing
* an argument for the actual tree, but instead just the key. The private function holds the argument of the tree which the public function will call in order to help it. This is a clever way to utilize encapsulation
* within this project. One difficulty I had was understanding how encapsulation worked and how to utilize both the private and public functions to work together. In order to figure it out, I realized that I just need
* to call the private function in the public function as discussed.
*/



#include "binary.h"
using namespace std;

/***
A Tree Node constructor. Set all the
member variables (key_, data_, left, right)
of the Node class. 

Input: 
T key - The key for the node
Y data - The actual value from the node

Output:
N/A
***/
template <class T, class Y>
Node<T, Y>::Node(T key, Y data){
    key_ = key; //Set the parameters to our member variables
    data_ = data;
    left = NULL; //Make them tree start null
    right = NULL;
} 

/***
A Node member getter function. This returns the
key of the node. 

Input: 
N/A

Output:
T - returns the key of the node that is of type T
***/
template <class T, class Y>
T Node<T, Y>::getKey(){ 
   return key_; //simply returns key
}

/***
A Node member getter function. This returns the
actual data of the node. 

Input: 
N/A

Output:
Y - Returns the data of the node that is of type Y
***/
template <class T, class Y>
Y Node<T, Y>::getData(){
   return data_; //simply returns data
}

/***
A Node member setter function. This sets the key
of a Node.

Input: 
T key - Sets the key of the Node of type T

Output:
N/A
***/
template <class T, class Y>
void Node<T, Y>::setKey(T key){
    key_ = key; //sets key
}

/***
A Node member setter function. This sets the actual
data of a Node.

Input: 
Y data - Sets the value of the Node of type Y

Output:
N/A
***/
template <class T, class Y>
void Node<T, Y>::setData(Y data){
    data_ = data; //sets data
}


/***
The BinaryTree constructor. Implicitly set the 
values of the member variables


Input: 
N/A

Output:
N/A
***/

template <class T, class Y>
BinaryTree<T,Y>::BinaryTree(){
    root = NULL; //start the tree empty
}


/***
A BinaryTree Member fuction. This function takes in two arguments
and inserts a node. If the key already exists in the tree do nothing.

Input: 
Node<T,Y> *parent_node - The current root node
Node<T,Y> *data - The Node that you want to insert into the tree

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::insert_node(Node<T,Y> *parent_node, Node<T,Y> *data){
   if(root == NULL) { //First check if the tree is empty, this will be our first insert if so.
    root = data; 
    return;
   }
   if(data->getKey() < parent_node->getKey()) { //This logic will start going left.
        if(parent_node->left == NULL) {
            parent_node->left = data; //We found an empty spot on the left.
        }
        else {
            insert_node(parent_node->left,data); //We did not find an empty node. Recursively continue down this left side
        }
   }
   else if(data->getKey() > parent_node->getKey()) { //Key is larger, check right
    if(parent_node->right == NULL) {
        parent_node->right = data; //Found empty spot on right
    }
    else {
        insert_node(parent_node->right, data); //NO empty node, recursively check right side
    }
   }
   else { return; } //key is equal, just return.
}

/***
A BinaryTree Member fuction. This function takes in one arguments
and finds the parent of a node given a key.
If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are searching for.

Output:
Node<T,Y> * - returns the parent of the node you are searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node_parent(T key){
    if(root == NULL || root->getKey() == key) {
        return NULL; //This handles special case that tree is empty or key is in root
    }
   return find_node_parent(root, key); //calls private recursive helper function
}

/***
A BinaryTree Member fuction. This function takes in one arguments
and finds the node given a key.
If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are searching for.

Output:
Node<T,Y> * - returns the node you are searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node(T key){
   return find_node(root, key); //Just call the private helper function
}


/***
A BinaryTree Member fuction. This function deletes a node in
the binary tree. If the key does not exist do not do anything.

Input: 
T key - The key of the node you are trying to delete.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::delete_node(T key){
    root = delete_node(root, key); //calls private recursive helper function and saves it to root
}


/***
A BinaryTree helper fuction. This function deletes a node in
the binary tree. If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are trying to delete.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - The new root since the root can be deleted.
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::delete_node(Node<T,Y>* node, T key){
   if(node == NULL) {
    return NULL; //If current node is null, key not found (base case)
   }

   if(key < node->getKey()) {
    node->left = delete_node(node->left, key); //We go left since its less
   }
   else if(key > node->getKey()) {
    node->right = delete_node(node->right, key); //We go right since its greater
   }
   else { //It has to be equal, so its found. Begin main logic
    if(node->left == NULL) { //Node has 1 or no children
        Node<T,Y> *temp = node->right;
        delete node;
        return temp; //New subtree root is right
    }
    else if(node->right == NULL) {
        Node<T,Y> *temp = node->left;
        delete node;
        return temp; //New subtree root is left.
    }
    //Has two children
    Node<T,Y> * temp = node->right;
    while(temp->left != NULL) {
        temp = temp->left; //Find smallest node in right subtree of the node that will replace the key node
    }
    node->setKey(temp->getKey()); 
    node->setData(temp->getData()); //Copy successors data to current node
    node->right = delete_node(node->right, temp->getKey());//Recursively delete successor of right
   }
   return node;
}


/***
A BinaryTree helper fuction. This function finds the parent node of
a node with a given key for the binary tree. If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are trying to find.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - Returns the parent of the key you were searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node_parent(Node<T,Y> *node,T key){
  if(node == NULL) {
    return NULL; //Base case 1, if null key not found
  }
  if((node->left != NULL && node->left->getKey() == key)  ||  (node->right != NULL && node->right->getKey() == key)) {
    return node; //Base case 2, We found parent (We checked if key is in node's children)
  }

  if(key < node->getKey()) {
    return find_node_parent(node->left, key); //Key is smaller, go left
  }
  else {
    return find_node_parent(node->right, key); // Key is bigger, go right
  }
}

/***
A BinaryTree helper fuction. This function finds the node with 
a given key for the binary tree. If the key does not exist in 
the tree return NULL.

Input: 
T key - The key of the node you are trying to find.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - Returns the Node in the Tree
              for the key you were searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node(Node<T,Y> *node,T key){
   //base steps
    if(node == NULL) {
    return NULL; //Key is not in tree
   }
   if(node->getKey() == key) {
    return node; //Found node
   }
   //recursive steps
   if(key < node->getKey()) {
    return find_node(node->left, key); //Key is smaller, go left
   }
   else {
    return find_node(node->right, key); //Key is bigger, go right.
   }
}

/***
A BinaryTree member fuction. This function performs
pre_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::pre_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if(node == NULL) { return; } //If node is null, return

    //Pre order is node, left, right
    list.push_back(*node);
    pre_order(node->left, list);
    pre_order(node->right, list);
}

/***
A BinaryTree member fuction. This function performs
in_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::in_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if(node == NULL) { return; } //If node is null, return

    //in order is left, node, right
    in_order(node->left, list);
    list.push_back(*node);
    in_order(node->right, list);
}

/***
A BinaryTree member fuction. This function performs
post_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::post_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if(node == NULL) { return; } //If node is null, return

    //post order is left, right, node
    post_order(node->left, list);
    post_order(node->right, list);
    list.push_back(*node);
}

/***
A BinaryTree getter fuction. This function gets
the current root of the binary tree.

Input: 
N/A
Output:
Outputs the root of the binary tree
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::getRoot(){
   return root; //just returns root
}


/***
The BinaryTree destructor. Make sure all
the TreeNodes that have been created get destroyed
and make sure there are no memory leaks.

Input: 
N/A

Output:
N/A
***/
template <class T, class Y>
BinaryTree<T,Y>::~BinaryTree(){
   destructor_helper(root); //calls upon helper function to destruct
   
}
//Created helper function
template <class T, class Y>
void destructor_helper(Node<T,Y> *node){
    if (node != NULL) { // Using NULL instead of NULL is good practice
        destructor_helper(node->left); //recursively deletes left
        destructor_helper(node->right); //recursively deletes right
        delete node; //the line that actually frees memory
    }
}

template class Node<int, std::string>;
template class Node<std::string, std::string>;
template class Node<int, int>;
template class Node<double, double>;
template class Node<std::string, long>;
template class Node<std::string, std::list<std::string> >;
template class Node<std::string, int >;

template class BinaryTree<std::string, std::list<std::string> >;
template class BinaryTree<std::string, std::string>;
template class BinaryTree<int, int>;
template class BinaryTree<int, std::string>;
template class BinaryTree<std::string, int>;


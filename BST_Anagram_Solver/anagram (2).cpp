/* In the anagram file, I had various helper functions that deal with constructing, adding words, and saving the anagrams. The first step is to take a file and copy the anagrams into the binary trees using file I/O in cpp.
* addWord and saveAnagrams were more tricky to implement. There were a few challenges initially that I had especially with syntax in creating nodes. It was necessary to use lists to store the nodes in saveAnagrams
* and I found it initially tricky with how to write it in cpp. Once I figured it out, the code made more sense. 
*/







#include "anagram.h"
#include <iostream>
#include <fstream>
#include <string>    // For std::string
#include <list>      // For std::list
#include <algorithm> // For std::sort

/***
An AnagramDict constructor. This Constructor takes in a filepath
and then creates a binary tree out of all the words inside the 
binary tree. 

Input: 
std::string filename - the path to the file

Output:
N/A
***/
AnagramDict::AnagramDict(std::string filename) {
    filename_ = filename;
    std::string path = "dictionaries/" + filename_;
    std::ifstream file;
    file.open(path.c_str());
    
    std::string word;
    while(file >> word) {
        addWord(word); //reads the file one by one and adds the word
    }
    file.close(); // closes file

}

/***
An AnagramDict member function. This function sorts the given word in 
lexicographical order

Input: 
std::string word - the word that needs to be sorted

Output:
std::string - the sorted version of the input
***/
std::string AnagramDict::sortWord(std::string word) {
    std::sort(word.begin(), word.end());
   return word; //does the sorting and returns
}

/***
An AnagramDict member function. This function adds a word to the tree.
If the words sorted version already exists in the tree add it to the linked
list of that node. If the sorted word does not exist in the tree create a new
node and insert it into the tree
Duplicated words should not be added to the tree.
Input: 
std::string word - the word that needs to inserted

Output:
N/A
***/
void AnagramDict::addWord(std::string word) {
   if(word.empty()) { return; } //Skipy empty words

   std::string sorted_word = sortWord(word); //create a sorted word using sortWord

    Node<std::string, std::list<std::string> >* found_node = tree.find_node(sorted_word); //Searches tree for node with sorted word as key

    if(found_node != NULL) {
        //A node with key exists, so its anagram
        //Get list of words from node
        std::list<std::string> words_list = found_node->getData();

        //Check if its a duplicate
        bool is_duplicate = false;
        for(std::list<std::string>::const_iterator it = words_list.begin(); it != words_list.end(); it++) { 
            if(*it == word) { //Dereference to get string
                is_duplicate = true; 
                break; //break out of loop
            }
        }
        if(!is_duplicate) {
            words_list.push_back(word); //Add the word since its not a duplicate
            found_node->setData(words_list);
        }
    }
    
    else {
        std::list<std::string> new_word_list; //the key does not exist so create new node
        new_word_list.push_back(word);

        Node<std::string, std::list<std::string> >* new_node = new Node<std::string, std::list<std::string> >(sorted_word, new_word_list); //create new node
        tree.insert_node(tree.getRoot(), new_node); //inserts
    }
}

/***
An AnagramDict member function. Does a preorder, postorder, or inorder traversal
and then prints out all the anagrams and words.

The output file should be the traversal order of the tree, but only the data on each line. 
View on wiki for more information. 

Input: 
std::string order - The type of order you want to traverse. Can be "pre", "post", "in"

Output:
N/A
***/
void AnagramDict::saveAnagrams(std::string order) {
    std::string path = "output/" + order+"_"+ filename_;
    std::ofstream file;
    file.open(path.c_str());
    if(!file.is_open()) {
        //create new file
        file.open(path.c_str(),std::ios::out);
    }
    
    std::list<Node<std::string, std::list<std::string> > > traversal_list; //Create a list to store nodes from traversal

    if(order == "in") {
        tree.in_order(tree.getRoot(), traversal_list);  //does in_order traversal
    }
    else if(order == "pre") {
        tree.pre_order(tree.getRoot(), traversal_list); //does pre_order traversal 
    }
    else if(order == "post") {
        tree.post_order(tree.getRoot(), traversal_list); //does post_order traversal
    }

    for(std::list<Node<std::string, std::list<std::string> > >::iterator node_it = traversal_list.begin(); node_it != traversal_list.end(); node_it++) { 
        std::list<std::string> words = node_it->getData(); //contains the anagrams
        if(words.size() > 1) { //only saves anagrams
            for(std::list<std::string>::const_iterator w_it = words.begin(); w_it != words.end(); w_it++) { 
                file << *w_it << " "; //sends w into the file and places a space after each word
            }
            file << std::endl; //endline
        }
    }
    file.close(); //closes file
}

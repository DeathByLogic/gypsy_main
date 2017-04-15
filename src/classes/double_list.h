#ifndef _DOUBLE_LIST_H
#define _DOUBLE_LIST_H

#include <iostream>

#include "double_node.h"

template<typename Object>
class DoubleList {
  public:
    // Constructor & Deconstructor
    DoubleList();
    virtual ~DoubleList();

    // Return node count 
    int count() const;
    bool empty() const;
    
    // Return first or last node
    DoubleNode<Object> *first() const;
    DoubleNode<Object> *last() const;
 
    // Add before or after node
    void addAfter(DoubleNode<Object> *, const Object &);
    void addBefore(DoubleNode<Object> *, const Object &);
   
    // Add first or last node
    void addFirst(const Object &);
    void addLast(const Object &);
   
    // Remove all nodes
    void clear();
    //list copy();
    
    // Remove specific node
    void remove(DoubleNode<Object> *);
    
    // Remove first or last node
    void removeFirst();
    void removeLast();
	
	virtual void print();
    
  private:
    // Number of nodes in list
    int node_count;
    
    // Pointers to first and last nodes
    DoubleNode<Object> *firstNode;
    DoubleNode<Object> *lastNode;
};

//
// Constructor & Deconstructor
//

// Constructor
template <typename Object>
DoubleList<Object>::DoubleList() {
  // Set initial size of list to 0
  node_count = 0;
  
  // Set first and last nodes to null
  firstNode = NULL;
  lastNode = NULL;
}

// Destructor
template <typename Object>
DoubleList<Object>::~DoubleList() {
  clear();
}

//
// List Properties
//

// Return node count
template<typename Object>
int DoubleList<Object>::count() const {
  return node_count;
}

// Is list empty
template<typename Object>
bool DoubleList<Object>::empty() const {
  return (firstNode == NULL);
}

// Return first node
template<typename Object>
DoubleNode<Object> *DoubleList<Object>::first() const {
  return firstNode;
}

// Return last node
template<typename Object>
DoubleNode<Object> *DoubleList<Object>::last() const{
  return lastNode; 
}

//
// Add node functions
//

// Add after node
template<typename Object>
void DoubleList<Object>::addAfter(DoubleNode<Object> * prevNode, const Object &obj) {
  // Create node pointers
  DoubleNode<Object> * newNode;
  
  // Save pointer to next node
  DoubleNode<Object> * nextNode = prevNode->next();
  
  // Create a new node
  newNode = new DoubleNode<Object>(obj, prevNode, nextNode);
  
  // If not the last pointer
  if (prevNode == lastNode) {
    // Update last node pointer
    lastNode = newNode;
  } else {
    // Set next node previous pointer
    nextNode->prev_node = newNode;
  }
  
  // Set prev node next pointer
  prevNode->next_node = newNode;
  
  // Increase size      
  node_count++;
}

// Add before node
template<typename Object>
void DoubleList<Object>::addBefore(DoubleNode<Object> *nextNode, const Object &obj) {
  // Create node pointers
  DoubleNode<Object> * newNode;
  
  // Save pointer to next node
  DoubleNode<Object> * prevNode = nextNode->prev();
  
  // Create a new node
  newNode = new DoubleNode<Object>(obj, prevNode, nextNode);
  
  if (nextNode == firstNode) {
    // Change first node pointer
    firstNode = newNode;
  } else {
    // Set next node previous pointer
    prevNode->next_node = newNode;
  }
  
  // Set prev node next pointer
  nextNode->prev_node = newNode;
  
  // Increase Size
  node_count++;
}

// Add new first node
template<typename Object>
void DoubleList<Object>::addFirst(const Object &obj) {
  // Create a new node
  DoubleNode<Object> *newNode;
    
  // If new node created
  if (count() == 0) {
    // Set first & last node pointer
    firstNode = new DoubleNode<Object>(obj, 0, 0);
    lastNode = firstNode;
  } else {
    newNode = new DoubleNode<Object>(obj, 0, 0);
    
    // Update pointers
    firstNode->prev_node = newNode;
    newNode->next_node = firstNode;
    
    // Set new first node
    firstNode = newNode;
  }
  
  // Increase size
  node_count++;
}

// Add new last node
template<typename Object>
void DoubleList<Object>::addLast(const Object &obj) {
  // Create a new node
  DoubleNode<Object> *newNode;
    
  // If new node created
  if (count() == 0) {
    // Set first & last node pointer
    lastNode = new DoubleNode<Object>(obj, 0, 0);
    firstNode = lastNode;
  } else {
    newNode = new DoubleNode<Object>(obj, 0, 0);
    
    // Update pointers
    lastNode->next_node = newNode;
    newNode->prev_node = lastNode;
    
    // Set new first node
    lastNode = newNode;
  }
  
  // Increase size
  node_count++;
}

//
// Remove and clear node functions
//

// Remove all nodes
template<typename Object>
void DoubleList<Object>::clear() {
  // Delete all nodes in list
  while ( !empty() ) {
    removeFirst();
  }
  
  // Reset size
  node_count = 0;
  
  // Reset pointers
  firstNode = NULL;
  lastNode = NULL;
}

// Remove specific node
template<typename Object>
void DoubleList<Object>::remove(DoubleNode<Object> * removeNode) {
  DoubleNode<Object> * prevNode = removeNode->prev();
  DoubleNode<Object> * nextNode = removeNode->next();
  
  // Remove node from list
  if (removeNode == firstNode) {
    firstNode = nextNode;
  } else {
    prevNode->next_node = nextNode;
  }
    
  // Remove node from list
  if (removeNode == lastNode) {
    lastNode = prevNode;
  } else {
    nextNode->prev_node = prevNode;
  }
    
  // Delete node from memory
  delete removeNode;
    
  // Decrement size
  node_count--;
}
    
// Remove first or last node
template<typename Object>
void DoubleList<Object>::removeFirst() {
  remove(firstNode);
}

template<typename Object>
void DoubleList<Object>::removeLast() {
  remove(lastNode);
}

// Print list
template<typename Object>
void DoubleList<Object>::print() {
	std::cerr << "Error. Print not available for base type." <<  std::endl;
}

#endif

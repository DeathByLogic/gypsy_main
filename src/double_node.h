#ifndef _DOUBLE_NODE_H
#define _DOUBLE_NODE_H

template <typename Object>
class DoubleList;

template <typename Object>
class DoubleNode {
  public:
    // Constructor
    DoubleNode( const Object & = Object(), DoubleNode * = 0, DoubleNode * = 0 );
    
    // Node data
    Object      value;
    
    // Prev & Next nodes
    DoubleNode *prev() const;
    DoubleNode *next() const;

    friend class DoubleList<Object>;
	
  private:
    DoubleNode  *prev_node;
    DoubleNode  *next_node;
};

// Class constructor
template <typename Object>
DoubleNode<Object>::DoubleNode( const Object &e, DoubleNode<Object> *p, DoubleNode<Object> *n ):value( e ), prev_node( p ), next_node( n ) {
  // empty constructor
}

// Return previous node
template <typename Object>
DoubleNode<Object> *DoubleNode<Object>::prev() const {
  return prev_node;
}

// Return next node
template <typename Object>
DoubleNode<Object> *DoubleNode<Object>::next() const {
  return next_node;
}

#endif

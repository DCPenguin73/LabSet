/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Daniel Carr, Jarom Anderson, Arlo Jolly
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements; }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) { }
   BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) { }
   BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) { }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode* pNode) const { return (pParent->pRight == this) ? true : false; }
   bool isLeftChild(BNode* pNode) const { return (pParent->pLeft == this) ? true : false; }

   // balance the tree
   void balance();

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff

   void assign(BNode*& pDest, const BNode* pSrc)
   {
      if (pSrc == nullptr)
      {
         clear(pDest);  // Only clear if source is null
         return;
      }

      if (pDest == nullptr)
      {
         pDest = new BNode(pSrc->data);
         pDest->isRed = pSrc->isRed;
      }
      else
      {
         pDest->data = pSrc->data;
         pDest->isRed = pSrc->isRed;
      }

      // Recursively assign left and right subtrees
      assign(pDest->pLeft, pSrc->pLeft);
      if (pDest->pLeft)
         pDest->pLeft->pParent = pDest;

      assign(pDest->pRight, pSrc->pRight);
      if (pDest->pRight)
         pDest->pRight->pParent = pDest;
   }


   void clear(BNode*& pThis)
   {
      if (pThis)
      {
         clear(pThis->pLeft);
         clear(pThis->pRight);
         delete pThis;
         pThis = nullptr;
      }
   }

   friend class BST <T>;
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode* p = nullptr) : pNode(p) { }
   iterator(const iterator& rhs) : pNode(rhs.pNode) { }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator& rhs) const { return pNode == rhs.pNode; }
   bool operator != (const iterator& rhs) const { return pNode != rhs.pNode; }

   // de-reference. Cannot change because it will invalidate the BST
   const T& operator * () const { return pNode->data; }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      // save old value
      // run prefix++
      // return old value
      BNode* pOld = *this;
      ++this;
      return pOld;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {

      BNode* pOld = *this;
      --this;
      return pOld;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST() : numElements(0), root(nullptr)
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> ::BST(const BST<T>& rhs) : numElements(0), root(nullptr)
{
   root = nullptr;
   numElements = 0;
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T> 
BST <T> ::BST(BST <T>&& rhs) : numElements(0), root(nullptr)
{
   root = rhs.root;
   numElements = rhs.numElements;

   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il) : numElements(0), root(nullptr)
{
   root = nullptr;
   numElements = 0;
   *this = il;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   if (this != &rhs)
   {
      // Assign new values
      root->assign(root, rhs.root);
      numElements = rhs.numElements;
   }
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();
   for (const T& t : il) // Iterate through each element in the initializer list
      insert(t); // Insert each element into the BST
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   BNode* tempRoot = rhs.root;
   rhs.root = root;
   root = tempRoot;

   size_t tempNumElements = rhs.numElements;
   rhs.numElements = numElements;
   numElements = tempNumElements;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{  
   BNode* nodeToDelete = it.pNode; // Access the node through the iterator's pNode member

   // Check if the node to delete is nullptr
   if (nodeToDelete == nullptr)
      return end();

   iterator returnValue = it;
   ++returnValue; // Move to the next node in in-order traversal

   if (nodeToDelete->pLeft == nullptr && nodeToDelete->pRight == nullptr)
   {
      // Case 1: Node to delete has no children
      if (nodeToDelete->pParent != nullptr)
      {
         if (nodeToDelete->pParent->pLeft == nodeToDelete)
            nodeToDelete->pParent->pLeft = nullptr;
         else
            nodeToDelete->pParent->pRight = nullptr;
      }
      delete nodeToDelete;
   }
   else if (nodeToDelete->pLeft == nullptr || nodeToDelete->pRight == nullptr)
   {
      // Case 2: Node to delete has one child
      BNode* child = (nodeToDelete->pLeft != nullptr) ? nodeToDelete->pLeft : nodeToDelete->pRight;
      if (nodeToDelete->pParent != nullptr)
      {
         if (nodeToDelete->pParent->pLeft == nodeToDelete)
            nodeToDelete->pParent->pLeft = child;
         else
            nodeToDelete->pParent->pRight = child;
      }
      child->pParent = nodeToDelete->pParent;
      delete nodeToDelete;
   }
   else
   {
      // Case 3: Node to delete has two children
      // Find the in-order successor (smallest node in the right subtree)
      BNode* successor = nodeToDelete->pRight;
      while (successor->pLeft != nullptr)
         successor = successor->pLeft;

      // If the successor is not the direct right child
      if (successor != nodeToDelete->pRight)
      {
         // Update the successor's parent's left or right pointer
         if (successor->pRight != nullptr)
            successor->pRight->pParent = successor->pParent;
         if (successor->pParent->pLeft == successor)
            successor->pParent->pLeft = successor->pRight;
         else
            successor->pParent->pRight = successor->pRight;
         // Move the successor's right child to the node to delete's right child
         successor->pRight = nodeToDelete->pRight;
         if (nodeToDelete->pRight != nullptr)
            nodeToDelete->pRight->pParent = successor;
      }

      // Update the parent of the node to delete to point to the successor
      if (nodeToDelete->pParent != nullptr)
      {
         if (nodeToDelete->pParent->pLeft == nodeToDelete)
            nodeToDelete->pParent->pLeft = successor;
         else
            nodeToDelete->pParent->pRight = successor;
      }
      // If the node to delete is the root, update the root pointer
      else
         root = successor;

      // Update the successor's parent pointer and left child pointer
      successor->pParent = nodeToDelete->pParent;
      successor->pLeft = nodeToDelete->pLeft;
      if (nodeToDelete->pLeft != nullptr)
         nodeToDelete->pLeft->pParent = successor;

      delete nodeToDelete;
   }

   --numElements; // Decrement the number of elements
   return returnValue;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   if (root != nullptr)
   {
      root->clear(root);
      root = nullptr;
   }
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (empty())
      return end();
   BNode* p = root; // help 
   while (p->pLeft)
      p = p->pLeft;
   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode* p = root;
   while (p)
   {
      if (p->data == t)
         return iterator(p);
      else if (t < p->data)
         p = p->pLeft;
      else
         p = p->pRight;
   }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   this->pLeft = pNode;
   pNode->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   this->pRight = pNode;
   pNode->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   pLeft = new BNode * (t);
   pLeft->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   pLeft = new BNode * (std::move(t));
   pLeft->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   pRight = new BNode * (t);
   pRight->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   pRight = new BNode * (std::move(t));
   pRight->pParent = this;
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight == nullptr) //(pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   if (!pParent->isRed)
      return;

   // Case 3: if the aunt is red, then just recolor

   BNode* pGranny = pParent->pParent;
   if (pGranny == nullptr)
      return;

   BNode* pAunt = (pGranny->pLeft == pParent) ? pGranny->pRight : pGranny->pLeft;

   if (pAunt != nullptr && pAunt->isRed)
   {
      pParent->isRed = false;
      pAunt->isRed = false;
      pGranny->isRed = true;
      pGranny->balance();
      return;
   }

   // Case 4: if the aunt is black or non-existent, then we need to rotate
   BNode* pMom = this->pParent;
   BNode* pNew = this;
   BNode* pSibling = (pParent->pRight == pNew) ? pParent->pLeft : pParent->pRight;
   BNode* pHead = pGranny->pParent;

   // Case 4a: We are mom's left and mom is granny's left
   // Rotate Right
   if (pParent->pLeft == this && pGranny->pLeft == pParent)
   {
      // Perform the rotation
      if (pSibling != nullptr)
         pGranny->addLeft(pParent->pRight);
      else
         pGranny->pLeft = nullptr;

      pParent->addRight(pGranny);
      pParent->pParent = pHead;

      // Fix Parent pointers
      if (pHead != nullptr)
      {
         if (pHead->pLeft == pGranny)
            pHead->addLeft(pParent);
         else
            pHead->addRight(pParent);
      }

      // Recolor
      pGranny->isRed = true;
      pParent->isRed = false;
   }
   // Case 4b: We are mom's right and mom is granny's right
   // Left Rotation
   else if (pParent->pRight == this && pGranny->pRight == pParent)
   {
      // Perform the rotation
      if (pSibling != nullptr)
         pGranny->addRight(pParent->pLeft);
      else
         pGranny->pRight = nullptr;

      pParent->addLeft(pGranny);
      pParent->pParent = pHead;

      // Fix parent pointers
      if (pHead != nullptr)
      {
         if (pHead->pLeft == pGranny)
            pHead->addLeft(pParent);
         else
            pHead->addRight(pParent);
      }
      // Recolor
      pGranny->isRed = true;
      pParent->isRed = false;
   }
   // Case 4c: We are mom's right and mom is granny's left
   // Double Right Rotation
   else if (pParent->pRight == this && pGranny->pLeft == pParent)
   {
      // Perform the rotations
      pGranny->addLeft(pNew->pRight);
      pParent->addRight(pNew->pLeft);

      pNew->addLeft(pParent);
      pNew->addRight(pGranny);
      pNew->pParent = pHead;

      if (pHead != nullptr)
      {
         if (pHead->pLeft == pGranny)
            pHead->addLeft(pNew);
         else
            pHead->addRight(pNew);
      }

      // Recolor
      pGranny->isRed = true;
      pNew->isRed = false;
   }
   // Case 4d: we are mom's left and mom is granny's right
   // Double Left Rotation
   else if (pParent->pLeft == this && pGranny->pRight == pParent)
   {
      // Perform the rotations

      pGranny->addRight(pNew->pLeft);
      pParent->addLeft(pNew->pRight);

      pNew->addRight(pParent);
      pNew->addLeft(pGranny);

      pNew->pParent = pHead;

      if (pHead != nullptr)
      {
         if (pHead->pLeft == pGranny)
            pHead->addLeft(pNew);
         else
            pHead->addRight(pNew);
      }

      // Recolor
      pGranny->isRed = true;
      pNew->isRed = false;
   }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (!pNode)
      return *this;
   if (pNode->pRight)
   {
      pNode = pNode->pRight;
      while (pNode->pLeft)
         pNode = pNode->pLeft;
      return *this;
   }
   if (pNode->pRight == nullptr && pNode->pParent->pLeft == pNode)
   {
      pNode = pNode->pParent;
      return *this;
   }
   if (pNode->pRight == nullptr && pNode->pParent->pRight == pNode)
   {
      while (pNode->pParent && pNode->pParent->pRight == pNode)
         pNode = pNode->pParent;
      pNode = pNode->pParent;
      return *this;
   }
   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (!pNode)
      return *this;
   if (pNode->pLeft)
   {
      pNode = pNode->pLeft;
      while (pNode->pRight)
         pNode = pNode->pRight;
      return *this;
   }
   if (pNode->pLeft == nullptr && pNode->pParent->pRight == pNode)
   {
      pNode = pNode->pParent;
      return *this;
   }
   if (pNode->pLeft == nullptr && pNode->pParent->pLeft == pNode)
   {
      while (pNode->pParent && pNode->pParent->pLeft == pNode)
         pNode = pNode->pParent;
      pNode = pNode->pParent;
      return *this;
   }
   return *this;
}


} // namespace custom



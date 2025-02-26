/***********************************************************************
* Header:
*    Set
* Summary:
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Daniel Carr, Jarom Anderson, Arlo Jolly
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

//   class TestSet;

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() : bst() {}
   set(const set& rhs) : bst(rhs.bst) {}
   set(set&& rhs) : bst(std::move(rhs.bst)) {}
   set(const std::initializer_list <T>& il) : bst(il) {}
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
      // Still needs done
   }
  ~set() {}

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
      if (this != &rhs)
      {
         //clear();
         bst = rhs.bst;
      }
      return *this;
   }
   set & operator = (set && rhs)
   {
      if (this != &rhs) {
         clear();
         bst.swap(rhs.bst);
      }
      return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
      return *this;
   }
   void swap(set& rhs) noexcept
   {
      bst.swap(rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      return iterator(bst.begin()); 
   }
   iterator end() const noexcept 
   { 
      return iterator(bst.end()); 
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      return iterator(bst.find(t)); 
   }

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }
   std::pair<iterator, bool> insert(T&& t)
   {
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }
   void insert(const std::initializer_list <T>& il)
   {
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
   }


   //
   // Remove
   //
   void clear() noexcept 
   { 
      bst.clear();
   }
   iterator erase(iterator &it)
   { 
      return iterator(); 
   }
   size_t erase(const T & t) 
   {
      return 99;
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      return iterator();
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;
public:
   // constructors, destructors, and assignment operator
   iterator() : it() {}
   iterator(const typename custom::BST<T>::iterator& itRHS)  
   {  
   }
   iterator(const iterator & rhs) 
   { 
   }
   iterator & operator = (const iterator & rhs)
   {
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return true; 
   }
   bool operator == (const iterator & rhs) const 
   { 
      return true; 
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *(new T); 
   }

   // prefix increment
   iterator & operator ++ ()
   {
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
      return *this;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
      return *this;
   }
   
private:

   typename custom::BST<T>::iterator it;
};



}; // namespace custom




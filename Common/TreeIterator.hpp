//***********************************************************************************************
//
//    Copyright (c) 2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  TreeIterator.hpp
// PURPOSE: Contains templated iterator class for traversal of binary trees.
// AUTHOR:  BHI Nov 2000
// NOTES:   The node class passed to the template *must* implement the functions:
//          T *Left() & T *Right().
//
#ifndef INC_TREEITERATOR_HPP
#define INC_TREEITERATOR_HPP

#pragma once

// warning C4786: identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786)
#include <deque>

template <class Node>
class LeftToRightTreeIterator
{
private:
   Node *m_pCurrent;           // the current node in the traversal
   std::deque<Node *> m_Stack; // maintains traversal state information

   void Next(Node *pNode=NULL) // sets the current element to the next in the traversal
   {
      if (!pNode)
      {
         if (m_pCurrent)
            pNode = m_pCurrent->Right();
         else // the traversal has started, so get the current element and continue from there
            return;
      }

      // push all left children on the stack
      while (pNode)
      {  
         m_Stack.push_back(pNode);
         pNode = pNode->Left();
      }

      // if the traversal stack is not empty, visit the top item
      if (m_Stack.size()!=0)
      {  
         m_pCurrent = m_Stack.back();
         m_Stack.pop_back();
         return;
      }
      m_pCurrent = NULL; // the traversal is done
   }

public:
   // Constructor: initialize the iterator for the given tree
   LeftToRightTreeIterator(Node *pTree=NULL)
   {
      m_pCurrent = NULL;
      Next(pTree);
   }

   // Assignment operator: initialize the iterator for the given tree
   LeftToRightTreeIterator &operator=(Node *pTree) 
   {
      m_pCurrent = NULL;
      m_Stack.clear();
      Next(pTree); // will set m_pCurrent
      return *this;
   }

   // pre-increment operator: steps onto the next item.
   LeftToRightTreeIterator &operator++()
	{
      Next();
		return *this; 
   }

	Node &operator*() const
	{
      return (*m_pCurrent); 
   }
	Node *operator->() const
   {
      return (&**this); 
   }
   operator Node *() const
   {
      return m_pCurrent;
   }
};

template <class Node>
class TopDownTreeIterator
{
private:
   Node *m_pCurrent;           // the current node in the traversal
   std::deque<Node *> m_Stack; // maintains traversal state information

   void Next(Node *pNode=NULL) // sets the current element to the next in the traversal
   {
      if (pNode)
         m_pCurrent = pNode;
      else if (m_Stack.size()==0)
         m_pCurrent = NULL;
      else
      {
         m_pCurrent = m_Stack.front();
         m_Stack.pop_front();
      }

      if (m_pCurrent)
      {
         if (m_pCurrent->Left())
            m_Stack.push_back(m_pCurrent->Left());
         if (m_pCurrent->Right())
            m_Stack.push_back(m_pCurrent->Right());
      }
   }

public:
   TopDownTreeIterator(Node *pTree=NULL) // Initialize the iterator for the given tree
   {
      m_pCurrent = NULL;
      Next(pTree);
   }

   // Assignment operator: initialize the iterator for the given tree
   TopDownTreeIterator &operator=(Node *pTree) 
   {
      m_pCurrent = NULL;
      m_Stack.clear();
      Next(pTree); // will set m_pCurrent
      return *this;
   }

   // pre-increment operator: steps onto the next item.
   TopDownTreeIterator &operator++()
	{
      Next();
		return *this; 
   }

	Node &operator*() const
	{
      return (*m_pCurrent); 
   }
	Node *operator->() const
   {
      return (&**this); 
   }
   operator Node *() const
   {
      return m_pCurrent;
   }
};

template <class Node>
class BottomUpTreeIterator
{
private:
   // the Next() for postorder requires a state stack of this form
   struct TreeInfo
   {  
      Node *pNode;
      int   nAction; // denotes the action to take when this node is popped

      TreeInfo(Node *pN, int nA)
      {
         pNode   = pN;
         nAction = nA;
      }
   };
   Node *m_pCurrent;             // the current node in the traversal
   std::deque<TreeInfo> m_Stack; // maintains traversal state information
   
   void Next(Node *pTree=NULL)   // sets the current element to the next in the traversal
   {
      if (pTree)
      {
         ASSERT(m_Stack.size()==0);
         m_Stack.push_back(TreeInfo(pTree, 1));
      }

      while (m_Stack.size()!=0)
      {
         TreeInfo p = m_Stack.back();
         m_Stack.pop_back();
         Node *pNode = p.pNode;

         // choose the appropriate action to take
         switch (p.nAction)
         {
            case 1: // push node and its left subtree
               m_Stack.push_back(TreeInfo(pNode, 2));
               if (pNode->Left())
                  m_Stack.push_back(TreeInfo(pNode->Left(), 1));
               break;
            case 2: // push node and its right subtree
               m_Stack.push_back(TreeInfo(pNode, 3));
               if (pNode->Right())
                  m_Stack.push_back(TreeInfo(pNode->Right(), 1));
               break;
            case 3: // visit this node
               m_pCurrent = pNode;
               return;
         }
      }
      m_pCurrent = NULL;
   }

public:
   BottomUpTreeIterator(Node *pTree=NULL) // Initialize the iterator for the given tree
   {
      m_pCurrent = NULL;
      Next(pTree);
   }

   // Assignment operator: initialize the iterator for the given tree
   BottomUpTreeIterator &operator=(Node *pTree) 
   {
      m_pCurrent = NULL;
      m_Stack.clear();
      Next(pTree); // will set m_pCurrent
      return *this;
   }

   // pre-increment operator: steps onto the next item.
   BottomUpTreeIterator &operator++()
	{
      Next();
		return *this; 
   }

	Node &operator*() const
	{
      return (*m_pCurrent); 
   }
	Node *operator->() const
   {
      return (&**this); 
   }
   operator Node *() const
   {
      return m_pCurrent;
   }
};

#endif   // INC_TREEITERATOR_HPP

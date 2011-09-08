/*
 * tree.h
 * 为系统提供树形结构的抽象
 *  Created on: 2011-5-4
 *      Author: 代码基于 TinyXML项目 重构 原作者Lee Thomason
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <assert.h>
#include "../../typedef.h"

namespace YF{

/* Nodes have siblings, a parent, and children. A node can be in a document, or stand on its own.
 * The type of a TreeNode can be queried, and it can be cast to its more defined type. */
template<typename NODE_T, typename VALUE_T>
class TreeNode {
	public:
		virtual ~TreeNode();

		/* Delete all the children of this node. Does not affect 'this'. */
		void Clear();

		/* One step up the DOM. */
		NODE_T* Parent(){
			return parent;
		}

		const NODE_T* Parent() const {
			return parent;
		}

		/* The first child of this node. Will be null if there are no children. */
		const NODE_T* FirstChild() const {
			return firstChild;
		}

		NODE_T* FirstChild(){
			return firstChild;
		}

		/* The first child of this node with the matching 'value'. Will be null if none found. */
		const NODE_T* FirstChild(const VALUE_T* value) const;

		NODE_T* FirstChild(const VALUE_T* _value){
			return const_cast<NODE_T*>((const_cast<const NODE_T*>(this))->FirstChild(_value));
		}

		/* The last child of this node. Will be null if there are no children. */
		const NODE_T* LastChild() const{
			return lastChild;
		}

		NODE_T* LastChild(){
			return lastChild;
		}

		/* The last child of this node matching 'value'. Will be null if there are no children. */
		const NODE_T* LastChild(const VALUE_T* value) const;

		NODE_T* LastChild(const VALUE_T* _value){
			return const_cast<NODE_T*>((const_cast<const NODE_T*>(this))->LastChild(_value));
		}

		/* An alternate way to walk the children of a node.	One way to iterate over nodes is:
		     for( child = parent->FirstChild(); child; child = child->NextSibling() )
		        IterateChildren
		  does the same thing with the syntax:
				 child = 0;
				 while( child = parent->IterateChildren( child ) )
		 *	IterateChildren takes the previous child as input and finds	the next one.
		 *	If the previous child is null, it returns the	first.
		 *	IterateChildren will return null when done.*/
		const NODE_T* IterateChildren(const NODE_T* previous) const;

		NODE_T* IterateChildren(const NODE_T* previous){
			return const_cast<NODE_T*>((const_cast<const NODE_T*>(this))->IterateChildren(previous));
		}

		/* This flavor of IterateChildren searches for children with a particular 'value' */
		const NODE_T* IterateChildren(const VALUE_T* value, const NODE_T* previous) const;

		NODE_T* IterateChildren(const VALUE_T* _value, const NODE_T* previous){
			return const_cast<NODE_T*>((const_cast<const NODE_T*>(this))->IterateChildren(_value, previous));
		}

		/* Add a new node related to this. Adds a child past the LastChild.
		 * Returns a pointer to the new object or NULL if an error occured.*/
		NODE_T* InsertEndChild(const NODE_T& addThis);

		/* Add a new node related to this. Adds a child past the LastChild.
		 * [NOTE] the node to be added is passed by pointer, and will be
		 * henceforth owned (and deleted) by tinyXml. This method is efficient
		 * and avoids an extra copy, but should be used with care as it
		 * uses a different memory model than the other insert functions.
		 * @sa InsertEndChild */
		NODE_T* LinkEndChild(NODE_T* addThis);

		/* Add a new node related to this. Adds a child before the specified child.
		 * Returns a pointer to the new object or NULL if an error occured. */
		NODE_T* InsertBeforeChild(NODE_T* beforeThis, const NODE_T& addThis);

		/* Add a new node related to this. Adds a child after the specified child.
		 * Returns a pointer to the new object or NULL if an error occured. */
		NODE_T* InsertAfterChild(NODE_T* afterThis, const NODE_T& addThis);

		/* Replace a child of this node.
		 * Returns a pointer to the new object or NULL if an error occured. */
		NODE_T* ReplaceChild(NODE_T* replaceThis, const NODE_T& withThis);

		/* Delete a child of this node. */
		bool RemoveChild(NODE_T* removeThis);

		/* Navigate to a sibling node. */
		const NODE_T* PreviousSibling() const{
			return prev;
		}
		NODE_T* PreviousSibling(){
			return prev;
		}

		/* Navigate to a sibling node. */
		const NODE_T* PreviousSibling(const VALUE_T*) const;
		NODE_T* PreviousSibling(const VALUE_T*_prev){
			return const_cast<NODE_T*>((const_cast<const NODE_T*>(this))->PreviousSibling(_prev));
		}

		/* Navigate to a sibling node. */
		const NODE_T* NextSibling() const{
			return next;
		}
		NODE_T* NextSibling(){
			return next;
		}

		/* Navigate to a sibling node with the given 'value'. */
		const NODE_T* NextSibling(const VALUE_T*) const;
		NODE_T* NextSibling(const VALUE_T* _next){
			return const_cast<NODE_T*>((const_cast<const NODE_T*>(this))->NextSibling(_next));
		}

		/* Returns true if this node has no children. */
		bool NoChildren() const{
			return !firstChild;
		}

		/* Create an exact duplicate of this node and return it.
		 * The memory must be deleted by the caller. */
	  virtual NODE_T* Clone() const = 0;

	protected:
		TreeNode(NODE_T* parent_);

		NODE_T* parent;
		NODE_T* firstChild;
		NODE_T* lastChild;
		NODE_T* prev;
		NODE_T* next;
	private:
		NODE_T* initNode;
		TreeNode(const NODE_T&); // not implemented.
		void operator=(const NODE_T& base); // not allowed.
};


template<typename NODE_T, typename VALUE_T>
TreeNode<NODE_T, VALUE_T>::TreeNode(NODE_T* init_node){
	parent = NULL;
	firstChild = NULL;
	lastChild = NULL;
	prev = NULL;
	next = NULL;
	initNode = init_node;
}

template<typename NODE_T, typename VALUE_T>
TreeNode<NODE_T, VALUE_T>::~TreeNode(){
	NODE_T* node = firstChild;
	NODE_T* temp = NULL;

	while (node){
		temp = node;
		node = node->next;
		delete temp;
	}
}

template<typename NODE_T, typename VALUE_T>
void TreeNode<NODE_T, VALUE_T>::Clear(){
	NODE_T* node = firstChild;
	NODE_T* temp = NULL;
	while (node){
		temp = node;
		node = node->next;
		delete temp;
	}
	firstChild = NULL;
	lastChild = NULL;
}

template<typename NODE_T, typename VALUE_T>
NODE_T* TreeNode<NODE_T, VALUE_T>::LinkEndChild(NODE_T* node){
	assert(node->parent == NULL || node->parent == initNode);

	node->parent = initNode;
	node->prev = lastChild;
	node->next = NULL;
	if (lastChild)
		lastChild->next = node;
	else // it was an empty list.
		firstChild = node;

	lastChild = node;
	return node;
}

template<typename NODE_T, typename VALUE_T>
NODE_T* TreeNode<NODE_T, VALUE_T>::InsertEndChild(const NODE_T& addThis){
	NODE_T* node = addThis.Clone( );
	if ( !node)
		return NULL;
	return LinkEndChild(node);
}

template<typename NODE_T, typename VALUE_T>
NODE_T* TreeNode<NODE_T, VALUE_T>::InsertBeforeChild(NODE_T* beforeThis, const NODE_T& addThis){
	if ( !beforeThis || beforeThis->parent != this)
		return NULL;

	NODE_T* node = addThis.Clone( );
	if ( !node) return NULL;
	node->parent = this;
	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev){
		beforeThis->prev->next = node;
	}else{
		assert(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}

template<typename NODE_T, typename VALUE_T>
NODE_T* TreeNode<NODE_T, VALUE_T>::InsertAfterChild(NODE_T* afterThis, const NODE_T& addThis){
	if ( !afterThis || afterThis->parent != this)
		return NULL;

	NODE_T* node = addThis.Clone( );
	if (!node)
		return NULL;
	node->parent = this;
	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next){
		afterThis->next->prev = node;
	}else{
		assert(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}

template<typename NODE_T, typename VALUE_T>
NODE_T* TreeNode<NODE_T, VALUE_T>::ReplaceChild(NODE_T* replaceThis, const NODE_T& withThis){
	if( ( !replaceThis) || (replaceThis->parent != this) )
		return NULL;

	NODE_T* node = withThis.Clone( );
	if ( !node)
		return NULL;
	node->next = replaceThis->next;
	node->prev = replaceThis->prev;
	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;
	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;
	delete replaceThis;
	node->parent = this;
	return node;
}

template<typename NODE_T, typename VALUE_T>
bool TreeNode<NODE_T, VALUE_T>::RemoveChild(NODE_T* removeThis){
	if( ( !removeThis) || (removeThis->parent != this)){
		assert(0);
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;
	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

template<typename NODE_T, typename VALUE_T>
const NODE_T* TreeNode<NODE_T, VALUE_T>::FirstChild(const VALUE_T* _value) const{
	const NODE_T* node;
	for (node = firstChild; node; node = node->next){
		if( ( node->Value( ) == _value) == 0)
			return node;
	}
	return NULL;
}

template<typename NODE_T, typename VALUE_T>
const NODE_T* TreeNode<NODE_T, VALUE_T>::LastChild(const VALUE_T* _value) const{
	const NODE_T* node;
	for (node = lastChild; node; node = node->prev){
		if ((node->Value( )== _value) == 0)
			return node;
	}
	return NULL;
}

template<typename NODE_T, typename VALUE_T>
const NODE_T* TreeNode<NODE_T, VALUE_T>::IterateChildren(const NODE_T* previous) const{
	if ( !previous){
		return FirstChild( );
	}else{
		assert(previous->parent == this);
		return previous->NextSibling( );
	}
}

template<typename NODE_T, typename VALUE_T>
const NODE_T* TreeNode<NODE_T, VALUE_T>::IterateChildren(const VALUE_T* val, const NODE_T* previous) const{
	if ( !previous){
		return FirstChild(val);
	}else{
		assert(previous->parent == this);
		return previous->NextSibling(val);
	}
}

template<typename NODE_T, typename VALUE_T>
const NODE_T* TreeNode<NODE_T, VALUE_T>::NextSibling(const VALUE_T* _value) const {
	const NODE_T* node;
	for (node = next; node; node = node->next){
		if ((node->Value( )== _value) == 0)
			return node;
	}
	return NULL;
}

template<typename NODE_T, typename VALUE_T>
const NODE_T* TreeNode<NODE_T, VALUE_T>::PreviousSibling(const VALUE_T* _value) const{
	const NODE_T* node;
	for (node = prev; node; node = node->prev){
		if ((node->Value( )== _value) == 0)
			return node;
	}
	return NULL;
}

#if 0
/* A TreeNodeHandle is a class that wraps a node pointer with null checks */
template<typename VALUE_T>
class TreeNodeHandle {
	public:
		/* Create a handle from any node (at any depth of the tree.) This can be a null pointer.*/
		TreeNodeHandle(TreeNode<VALUE_T>* _node){
			this->node = _node;
		}

		TreeNodeHandle(const TreeNodeHandle& ref){
			this->node = ref.node;
		}

		TreeNodeHandle operator=(const TreeNodeHandle& ref){
			this->node = ref.node;
			return *this;
		}

		/* Return a handle to the first child node. */
		TreeNodeHandle FirstChild() const;

		/* Return a handle to the first child node with the given name. */
		TreeNodeHandle FirstChild(const VALUE_T* value) const;

		/* Return a handle to the "index" child with the given name.
		 * The first child is 0, the second 1, etc.*/
		TreeNodeHandle Child(const VALUE_T* value, int index) const;

		/* Return a handle to the "index" child.
		 * The first child is 0, the second 1, etc.*/
		TreeNodeHandle Child(int index) const;

		/* Return the handle as a TreeNode. This may return null.	*/
		TreeNode<VALUE_T>* ToTreeNode() const{
			return node;
		}

	private:
		TreeNode<VALUE_T>* node;
};
#endif


#if 0
/////////////////////  calss TreeNodeHandle   //////////////////////////////////
template<typename NODE_T>
TreeNodeHandle<NODE_T> TreeNodeHandle<NODE_T>::FirstChild() const{
	if (node){
		NODE_T* child = node->FirstChild( );
		if (child)
			return TreeNodeHandle<NODE_T>(child);
	}
	return TreeNodeHandle<NODE_T>(NULL);
}

template<typename NODE_T>
TreeNodeHandle<NODE_T> TreeNodeHandle<NODE_T>::FirstChild(const NODE_T* value) const{
	if (node){
		NODE_T* child = node->FirstChild(value);
		if (child)
			return TreeNodeHandle<NODE_T>(child);
	}
	return TreeNodeHandle<NODE_T>(NULL);
}

template<typename NODE_T>
TreeNodeHandle<NODE_T> TreeNodeHandle<NODE_T>::Child(int count) const{
	if (node){
		NODE_T* child = node->FirstChild( );
		for (int i = 0; child && i < count; child = child->NextSibling( ), ++i){
			// nothing
		}
		if (child)
			return TreeNodeHandle<NODE_T>(child);
	}
	return TreeNodeHandle<NODE_T>(NULL);
}

template<typename NODE_T>
TreeNodeHandle<NODE_T> TreeNodeHandle<NODE_T>::Child(const NODE_T* value, int count) const{
	if (node){
		NODE_T* child = node->FirstChild(value);
		for (int i = 0; child && i < count; child = child->NextSibling(value), ++i){
			// nothing
		}
		if (child)
			return TreeNodeHandle<NODE_T>(child);
	}
	return TreeNodeHandle<NODE_T>(NULL);
}
#endif

} /// namespace YF
#endif /* TREE_HPP_ */

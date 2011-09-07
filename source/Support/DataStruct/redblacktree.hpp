/*
 * redblacktree.hpp
 *
 *  Created on: 2011-4-20
 *      Author: fanopi
 */
#ifndef __YF_INCLUDE_SUPPORT_REDBLACKTREE_H__
#define __YF_INCLUDE_SUPPORT_REDBLACKTREE_H__

#include "../classsupport.hpp"
#include "../../os/threads/threads.hpp"

namespace YF{

template <typename T>
class RBTree{
 public:
	enum NodeColor{
		RED,
		BLACK
	};

	typedef struct Node {
		NodeColor	color;
		T	 key;
		union {
				struct Node *left;   // For nodes in the tree, the node's left child.
				struct Node *next;   // For nodes in the free list, the next node on the free list.
		};
		struct Node     *parent;
		struct Node     *right;
	} Node;

	// Reserve capacity for the tree is allocated in large chunks with room for many nodes.
	typedef struct Chunk{
		struct Chunk *next;  // Pointer to the next node in the chunk list.
		Node         *nodes; // Pointer to an array (of variable size) where nodes are stored.
	}Chunk;

	RBTree(){
		freelist_   = NULL;
		YFInitializeCriticalSection(&lock_);
		nil_.color  = BLACK;
		nil_.key    = T();
		nil_.left   = &nil_;
		nil_.parent = &nil_;
		nil_.right  = &nil_;
		reserve_    = TREE_DEFAULT_RESERVE;
		root_       = &nil_;
		store_      = NULL;
		storetail_  = NULL;
	}

	~RBTree(){
		Chunk *cur;
		Chunk *temp;
		YFEnterCriticalSection(&lock_);
		cur = store_;
		while (cur != NULL) {
				temp = cur;
				cur = cur->next;
				delete [] temp->nodes;
				delete temp;
		}
		YFLeaveCriticalSection(&lock_);
		YFDeleteCriticalSection(&lock_);
	}

	typename
	RBTree::Node* Begin()const{
		Node *cur;
		YFEnterCriticalSection(&lock_);
		if (root_ == &nil_){
				YFLeaveCriticalSection(&lock_);
				return NULL;
		}
		cur = root_;
		while (cur->left != &nil_){
				cur = cur->left;
		}
		YFLeaveCriticalSection(&lock_);

		return cur;
  }

	void Erase(typename RBTree::Node *node){
		Node *child;
		Node *cur;
		Node *erasure;
		Node *sibling;

		YFEnterCriticalSection(&lock_);
		if ((node->left == &nil_) || (node->right == &nil_)) {
				erasure = node;
		}
		else {
				erasure = node->right;
				while (erasure->left != &nil_) {
						erasure = erasure->left;
				}
		}

		if (erasure->left != &nil_) {
				child = erasure->left;
		}
		else {
				child = erasure->right;
		}

		child->parent = erasure->parent;
		if (child->parent == &nil_) {
				root_ = child;
		}
		else {
				if (erasure == erasure->parent->left) {
						erasure->parent->left = child;
				}
				else {
						erasure->parent->right = child;
				}
		}

		if (erasure != node) {
				node->key  = erasure->key;
		}

		if (erasure->color == BLACK) {
				cur = child;
				while ((cur != root_) && (cur->color == BLACK)) {
						if (cur == cur->parent->left) {
								// Current node is a left child.
								sibling = cur->parent->right;
								if (sibling->color == RED) {
										sibling->color = BLACK;
										cur->parent->color = RED;
										Rotateleft(cur->parent);
										sibling = cur->parent->right;
								}
								if ((sibling->left->color == BLACK) && (sibling->right->color == BLACK)) {
										sibling->color = RED;
										cur = cur->parent;
								}
								else {
										if (sibling->right->color == BLACK) {
												sibling->left->color = BLACK;
												sibling->color = RED;
												Rotateright(sibling);
												sibling = cur->parent->right;
										}
										sibling->color = cur->parent->color;
										cur->parent->color = BLACK;
										sibling->right->color = BLACK;
										Rotateleft(cur->parent);
										cur = root_;
								}
						}
						else {
								sibling = cur->parent->left;
								if (sibling->color == RED) {
										sibling->color = BLACK;
										cur->parent->color = RED;
										Rotateright(cur->parent);
										sibling = cur->parent->left;
								}
								if ((sibling->left->color == BLACK) && (sibling->right->color == BLACK)) {
										sibling->color = RED;
										cur = cur->parent;
								}
								else {
										if (sibling->left->color == BLACK) {
												sibling->right->color = BLACK;
												sibling->color = RED;
												Rotateleft(sibling);
												sibling = cur->parent->left;
										}
										sibling->color = cur->parent->color;
										cur->parent->color = BLACK;
										sibling->left->color = BLACK;
										Rotateright(cur->parent);
										cur = root_;
								}
						}
				}
				cur->color = BLACK;
		}

		erasure->next = freelist_;
		freelist_ = erasure;

		YFLeaveCriticalSection(&lock_);
	}

	void Erase(const T &key){
		Node *node;

		// Find the node to erase.
		YFEnterCriticalSection(&lock_);
		node = root_;
		while (node != &nil_) {
				if (node->key < key) {
						// Go right.
						node = node->right;
				}
				else if (key < node->key) {
						// Go left.
						node = node->left;
				}
				else {
						// Found it.
						Erase(node);
						YFLeaveCriticalSection(&lock_);
						return;
				}
		}
		YFLeaveCriticalSection(&lock_);

		return; // 'key' is not in the tree.
	}

	typename
	RBTree::Node* Find(const T &key) const{
		Node *cur;

		YFEnterCriticalSection(&lock_);
		cur = root_;
		while (cur != &nil_) {
				if (cur->key < key) {
						cur = cur->right;
				}
				else if (key < cur->key) {
						cur = cur->left;
				}
				else {
						YFLeaveCriticalSection(&lock_);
						return cur;
				}
		}
		YFLeaveCriticalSection(&lock_);

		return NULL;// 'key' is not in the tree.
	}

	typename
	RBTree::Node* Insert(const T &key){
		Node  *cur;
		Node  *node;
		Node  *parent;
		Node  *uncle;

		YFEnterCriticalSection(&lock_);
		cur = root_;
		parent = &nil_;
		while (cur != &nil_) {
				parent = cur;
				if (cur->key < key) {
						cur = cur->right;
				}
				else if (key < cur->key) {
						cur = cur->left;
				}
				else {
						YFLeaveCriticalSection(&lock_);
						return NULL;
				}
		}

		if (freelist_ == NULL) {
				Reserve(reserve_);
		}
		node = freelist_;
		freelist_ = freelist_->next;

		node->color  = RED;
		node->key    = key;
		node->left   = &nil_;
		node->parent = parent;
		node->right  = &nil_;
		if (parent == &nil_) {
				root_ = node;
		}
		else {
				if (parent->key < key) {
						parent->right = node;
				}
				else {
						parent->left = node;
				}
		}

		// Rebalance and/or adjust the tree, if necessary.
		cur = node;
		while (cur->parent->color == RED) {
				if (cur->parent == cur->parent->parent->left){// Parent is the left child. Uncle is the right child.
						uncle = cur->parent->parent->right;
						if (uncle->color == RED) {
								cur->parent->parent->color = RED; // Uncle is RED. Recolor.
								cur->parent->color = BLACK;
								uncle->color = BLACK;
								cur = cur->parent->parent;
						}
						else {// Uncle is BLACK. Restructure.
								if (cur == cur->parent->right) {
										cur = cur->parent;
										Rotateleft(cur);
								}
								cur->parent->color = BLACK;
								cur->parent->parent->color = RED;
								Rotateright(cur->parent->parent);
						}
				}
				else {// Parent is the right child. Uncle is the left child.
						uncle = cur->parent->parent->left;
						if (uncle->color == RED){// Uncle is RED. Recolor.
								cur->parent->parent->color = RED;
								cur->parent->color = BLACK;
								uncle->color = BLACK;
								cur = cur->parent->parent;
						}
						else {// Uncle is BLACK. Restructure.
								if (cur == cur->parent->left) {
										cur = cur->parent;
										Rotateright(cur);
								}
								cur->parent->color = BLACK;
								cur->parent->parent->color = RED;
								Rotateleft(cur->parent->parent);
						}
				}
		}

		root_->color = BLACK;// The root node is always colored BLACK.
		YFLeaveCriticalSection(&lock_);

		return node;
	}

	typename
	RBTree::Node* Next(typename RBTree::Node *node) const{
		Node* cur;

		if (node == NULL) {
				return NULL;
		}

		YFEnterCriticalSection(&lock_);
		if (node->right != &nil_) {
				// 'node' has a right child. Successor is the far left node in
				// the right subtree.
				cur = node->right;
				while (cur->left != &nil_) {
						cur = cur->left;
				}
				YFLeaveCriticalSection(&lock_);
				return cur;
		}
		else if (node->parent != &nil_) {
				// 'node' has no right child, but does have a parent.
				if (node == node->parent->left) {
						// 'node' is a left child; node's parent is successor.
						YFLeaveCriticalSection(&lock_);
						return node->parent;
				}
				else {
						// 'node' is a right child.
						cur = node;
						// Go up the tree until we find a parent to the right.
						while (cur->parent != &nil_) {
								if (cur == cur->parent->right) {
										cur = cur->parent;
										continue;
								}
								else {
										YFLeaveCriticalSection(&lock_);
										return cur->parent;
								}
						}

						// There is no parent greater than 'node'. 'node' is the
						// maximum node.
						YFLeaveCriticalSection(&lock_);
						return NULL;
				}
		}
		else {
				// 'node' is root and root is the maximum node.
				YFLeaveCriticalSection(&lock_);
				return NULL;
		}
	}

	typename
	RBTree::Node* Prev(typename RBTree::Node *node) const{
		Node* cur;

		if (node == NULL) {
				return NULL;
		}

		YFEnterCriticalSection(&lock_);
		if (node->left != &nil_) {
				// 'node' has left child. Predecessor is the far right node in the
				// left subtree.
				cur = node->left;
				while (cur->right != &nil_) {
						cur = cur->right;
				}
				YFLeaveCriticalSection(&lock_);
				return cur;
		}
		else if (node->parent != & nil_) {
				// 'node' has no left child, but does have a parent.
				if (node == node->parent->right) {
						// 'node' is a right child; node's parent is predecessor.
						YFLeaveCriticalSection(&lock_);
						return node->parent;
				}
				else {
						// 'node is a left child.
						cur = node;
						// Go up the tree until we find a parent to the left.
						while (cur->parent != &nil_) {
								if (cur == cur->parent->left) {
										cur = cur->parent;
										continue;
								}
								else {
										YFLeaveCriticalSection(&lock_);
										return cur->parent;
								}
						}

						// There is no parent less than 'node'. 'node' is the minimum
						// node.
						YFLeaveCriticalSection(&lock_);
						return NULL;
				}
		}
		else {
				// 'node' is root and root is the minimum node.
				YFLeaveCriticalSection(&lock_);
				return NULL;
		}
	}

	int Reserve (int count){
		Chunk *chunk;
		int   index;
		int   oldreserve = reserve_;

		if (count != reserve_) {
				if (count < 1) {
						reserve_ = 1; // Minimum reserve size is 1.
				}
				else {
						reserve_ = count;
				}
		}

		YFEnterCriticalSection(&lock_);
		if (freelist_ == NULL) {
				// Allocate additional storage.
				// Link a new chunk into the chunk list.
				chunk = new RBTree::Chunk;
				chunk->nodes = new RBTree::Node[reserve_];
				chunk->next = NULL;
				if (store_ == NULL) {
						store_ = chunk;
				}
				else {
						storetail_->next = chunk;
				}
				storetail_ = chunk;

				// Link the individual nodes together to form a new free list.
				for (index = 0; index < reserve_ - 1; index++) {
						chunk->nodes[index].next = &chunk->nodes[index + 1];
				}
				chunk->nodes[index].next = NULL;
				freelist_ = chunk->nodes;
		}
		YFLeaveCriticalSection(&lock_);

		return oldreserve;
	}

 private:
	const static int TREE_DEFAULT_RESERVE = 32;


	void Rotateleft(typename RBTree::Node *parent){
			Node *child = parent->right;

			// Reassign the child's left subtree to the parent.
			parent->right = child->left;
			if (child->left != &nil_) {
					child->left->parent = parent;
			}

			// Reassign the child/parent relationship.
			child->parent = parent->parent;
			if (parent->parent == &nil_) {
					// The child becomes the new root node.
					root_ = child;
			}
			else {
					// Point the grandparent at the child.
					if (parent == parent->parent->left) {
							parent->parent->left = child;
					}
					else {
							parent->parent->right = child;
					}
			}
			child->left = parent;
			parent->parent = child;
	}

	void Rotateright(typename RBTree::Node *parent){
			Node *child = parent->left;

			// Reassign the child's right subtree to the parent.
			parent->left = child->right;
			if (child->right != &nil_) {
					child->right->parent = parent;
			}

			// Reassign the child/parent relationship.
			child->parent = parent->parent;
			if (parent->parent == &nil_) {
					// The child becomes the new root node.
					root_ = child;
			}
			else {
					// Point the grandparent at the child.
					if (parent == parent->parent->left) {
							parent->parent->left = child;
					}
					else {
							parent->parent->right = child;
					}
			}
			child->right = parent;
			parent->parent = child;
	}

	// Private data members.
	mutable YFCRITICAL_SECTION  lock_;      // Protects the tree's integrity against concurrent accesses.
	Node	*freelist_;  // Pointer to the list of free nodes (reserve storage).
	Node	nil_;       // The tree's nil node. All leaf nodes point to this.
	int		reserve_;   // The size (in nodes) of the chunks of reserve storage.
	Node	*root_;      // Pointer to the tree's root node.
	Chunk	*store_;     // Pointer to the start of the chunk list.
	Chunk	*storetail_; // Pointer to the end of the chunk list.

	DISALLOW_COPY_ADN_ASSIGN(RBTree<T>);
};

} /// namespace YF ///
#endif /// __YF_INCLUDE_SUPPORT_REDBLACKTREE_H__

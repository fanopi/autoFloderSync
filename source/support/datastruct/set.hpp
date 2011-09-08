/*
 * set.hpp
 *
 *  Created on: 2011-4-20
 *      Author: fanopi
 */
#ifndef __YF_INCLUDE_SUPPORT_REDBLACKTREE_H__
#define __YF_INCLUDE_SUPPORT_REDBLACKTREE_H__

#include "redblacktree.hpp"

namespace YF{

template <typename Tk>
class Set{
 public:
  class Iterator {
    friend class Set<Tk>;
   public:
    Iterator(){
			node_ = NULL;
			tree_ = NULL;
    }

		bool operator !=(const Iterator &other) const{
			return ((tree_ != other.tree_) || (node_ != other.node_));
		}

		const Tk& operator *() const{
			return node_->key;
		}

		Iterator& operator ++(int){
			node_ = tree_->Next(node_);
			return *this;
		}

		Iterator operator ++ (){
			typename RBTree<Tk>::Node *cur = node_;
			node_ = tree_->Next(node_);
			return Iterator(tree_, cur);
		}

		Iterator operator - (size_t num) const{
			size_t                     count;
			typename RBTree<Tk>::Node *cur = node_;
			cur = tree_->Prev(node_);
			for (count = 0; count < num; count++){
					cur = tree_->Prev(node_);
					if (cur == NULL){
							return Iterator(tree_, NULL);
					}
			}
			return Iterator(tree_, cur);
		}

		bool operator == (const Iterator &other) const{
			return ((tree_ == other.tree_) && (node_ == other.node_));
		}

  private:
		Iterator (const RBTree<Tk> *tree, typename RBTree<Tk>::Node *node){
				tree_ = tree;
				node_ = node;
		}

		typename RBTree<Tk>::Node  *node_; // Pointer to the node referenced by the Set Iterator.
		const    RBTree<Tk>        *tree_; // Pointer to the tree containing the referenced node.
    }; /// class Iterator ///

    Iterator Begin() const{
      return Iterator(&tree_, tree_.Begin());
    }

    Iterator End() const{
      return Iterator(&tree_, NULL);
    }

    void Erase(Iterator& it){
      tree_.Erase(it.node_);
    }

    void Erase(const Tk &key){
      tree_.Erase(key);
    }

    Iterator Find(const Tk &key) const{
      return Iterator(&tree_, tree_.Find(key));
    }

    Iterator Insert(const Tk &key){
      return Iterator(&tree_, tree_.Insert(key));
    }

    size_t Reserve(size_t count){
      return tree_.Reserve(count);
    }

private:
    RBTree<Tk> tree_;
}; /// class Set ///

} /// namespace YF
#endif /// __YF_INCLUDE_SUPPORT_REDBLACKTREE_H__

/*
 * map.hpp
 *
 *  Created on: 2011-4-20
 *      Author: fanopi
 */
#ifndef __YF_INCLUDE_SUPPORT_MAP_H__
#define __YF_INCLUDE_SUPPORT_MAP_H__

#include "redblacktree.hpp"

namespace YF{

template <typename Tf, typename Ts>
class Pair {
 public:
	Pair(){
		first = Tf();
		second = Ts();
	}

	Pair(const Tf &f, const Ts &s){
		first = f;
		second = s;
	}

	bool operator < (const Pair &other) const{
		return (first < other.first);
	}

	Tf first;
	Ts second;
};

template <typename Tk, typename Tv>
class Map{
 public:
    class Iterator {
     public:
			Iterator (){
				node_ = NULL;
				tree_ = NULL;
			}

			bool operator != (const Iterator &other) const{
				return ((tree_ != other.tree_) || (node_ != other.node_));
			}

			const Pair<Tk, Tv>& operator * () const{
				return node_->key;
			}

			Iterator& operator ++ (int){
				node_ = tree_->Next(node_);
				return *this;
			}

			Iterator operator ++ (){
				typename RBTree<Pair<Tk, Tv> >::Node *cur = node_;

				node_ = tree_->Next(node_);
				return Iterator(tree_, cur);
			}

			Iterator operator - (size_t num) const{
				size_t  count;
				typename RBTree<Pair<Tk, Tv> >::Node *cur = node_;

				cur = tree_->Prev(node_);
				for (count = 0; count < num; count++)  {
						cur = tree_->Prev(node_);
						if (cur == NULL) {
								return Iterator(tree_, NULL);
						}
				}
				return Iterator(tree_, cur);
			}

			bool operator == (const Iterator &other) const{
				return ((tree_ == other.tree_) && (node_ == other.node_));
			}

   private:
			Iterator(const RBTree<Pair<Tk, Tv> > *tree, typename RBTree<Pair<Tk, Tv> >::Node *node){
				node_ = node;
				tree_ = tree;
			}

			typename RBTree<Pair<Tk, Tv> >::Node* node_;
			const RBTree<Pair<Tk, Tv> >* tree_;

			friend class Map<Tk, Tv>;
    };

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
      tree_.Erase(Pair<Tk, Tv>(key, Tv()));
    }

    Iterator Find(const Tk &key) const{
      return Iterator(&tree_, tree_.Find(Pair<Tk, Tv>(key, Tv())));
    }

    Iterator Insert(const Tk &key, const Tv &data){
      return Iterator(&tree_, tree_.Insert(Pair<Tk, Tv>(key, data)));
    }

    size_t Reserve(size_t count){
      return tree_.Reserve(count);
    }

private:
    RBTree<Pair<Tk, Tv> > tree_;
};

} /// namespace YF ///
#endif /// __YF_INCLUDE_SUPPORT_MAP_H__

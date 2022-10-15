// xtree internal header

#ifndef XTREE
#define XTREE

const int RED = 0;
const int Red = 0;

const int BLACK = 1;
const int Black = 1;

template<typename Ty>
struct TreeNode
{
	using NodePtr = TreeNode*;
	using value_type = Ty;

	NodePtr left;   // left subtree, or smallest element if head
	NodePtr parent; // parent, or root of tree if head
	NodePtr right;  // right subtree, or largest element if head
	char color;  // 0: Red, 1: Black; Black if head
	char isNil;  // 1 only if head (also nil) node
	value_type myval;  // the stored value, unused if head
};

// CLASS TEMPLATE TreeVal
template<typename Ty>
class TreeVal
{
public:
	using NodePtr = TreeNode<Ty>*;

	using value_type = Ty;
	using size_type = size_t;

	TreeVal()
		: myHead(new TreeNode<value_type>),
		mySize(0)
	{
		myHead->left = myHead;
		myHead->parent = myHead;
		myHead->right = myHead;
		myHead->color = 1;
		myHead->isNil = 1;
	}

	~TreeVal()
	{
		clear(myHead->parent);
		delete myHead;
	}

	// Removes all elements from the set object (which are destroyed)
	void clear(TreeNode<value_type>* node)
	{
		if (!node->isNil) // node is not an external node
		{
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	// rebalance for insertion; u points to the inserted node
	// done 
	void reBalance(TreeNode<value_type>* u)
	{  // u->parent cannot be the root
		TreeNode<value_type>* pu = u->parent;
		TreeNode<value_type>* gu = pu->parent;
		TreeNode<value_type>* uncle = Sibling(pu); // uncle of u

		//just change the color
		if (uncle->color == RED)
		{
			if (gu == myHead->parent)
			{
				pu->color = BLACK;
				uncle->color = BLACK;
			}
			else
			{
				gu->color = RED;
				pu->color = BLACK;
				uncle->color = BLACK;
				if (gu->parent->color == RED)
					reBalance(gu);
			}

		}
		else // change node && 
		{
			if (pu->parent->left == pu) // parent equals L
			{
				if (u->parent->left == u) // LLB
				{
					LLRotation(pu);
					gu->color = RED;
					pu->color = BLACK;

				}
				else // LRB
				{
					LRRotation(u);
					u->color = BLACK;
					pu->color = RED;
					gu->color = RED;

				}
			}
			else if (pu->parent->right == pu)   // RXb
			{
				if (u->parent->left == u) // RLB
				{
					RLRotation(u);
					u->color = BLACK;
					pu->color = RED;
					gu->color = RED;

				}
				else // RRB
				{
					RRRotation(pu);
					pu->color = BLACK;
					gu->color = RED;

				}
			}
		}
	}

	TreeNode< value_type >* Sibling(TreeNode< value_type >* node, TreeNode< value_type >* par = nullptr)
	{
		if (node->isNil == 0)
			par = node->parent;
		if (par->left == node)
			return par->right;
		else if (par->right == node)
			return par->left;
		else
			return myHead;//orphan

	}

	// rotate right at gu, where pu = gu->left and u = pu->left
	// done 
	void LLRotation(TreeNode<value_type>* pu)
	{

		TreeNode<value_type>* gu = pu->parent;
		TreeNode<value_type>* guPar = gu->parent;
		LinkStart(gu, pu->right, 'L');
		LinkStart(pu, gu, 'R');

		if (guPar->isNil == 1) // gu is root
		{
			guPar->parent = pu;
			pu->parent = guPar;
		}
		else // gu is not root
		{
			if (guPar->left == gu)
				LinkStart(guPar, pu, 'L');
			else if (guPar->right == gu)
				LinkStart(guPar, pu, 'R');
		}

	}

	// LR rotation; pu = gu->left and u = pu->right
	void LRRotation(TreeNode<value_type>* u)
	{
		RRRotation(u);
		LLRotation(u);
	}

	// RL rotation; pu = gu->right and u = pu->left
	void RLRotation(TreeNode<value_type>* u)
	{
		LLRotation(u);
		RRRotation(u);
	}

	// rotate left at gu, where pu = gu->right and u = pu->right
	void RRRotation(TreeNode<value_type>* pu)
	{
		TreeNode<value_type>* gu = pu->parent;
		TreeNode<value_type>* guPar = gu->parent;
		LinkStart(gu, pu->left, 'R');
		LinkStart(pu, gu, 'L');

		if (guPar->isNil == 1) // gu is root
		{
			guPar->parent = pu;
			pu->parent = guPar;
		}
		else // gu is not root
		{
			if (guPar->left == gu)
				LinkStart(guPar, pu, 'L');
			else if (guPar->right == gu)
				LinkStart(guPar, pu, 'R');
		}
	}

	void LinkStart(TreeNode<value_type>* NewPar, TreeNode<value_type>* Child, char flag)
	{
		if (flag == 'L')
			NewPar->left = Child;
		if (flag == 'R')
			NewPar->right = Child;
		if (Child->isNil == 0)
			Child->parent = NewPar;
	}

	// erase p provided that the degree of p is at most one
	// p points to the node to be deleted
	// p == M in "Ch 10 Sec 3.pptx"
	void eraseDegreeOne(TreeNode<value_type>* p)
	{
		TreeNode<value_type>* child;
		if (p->left != myHead)
			child = p->left;
		else if (p->right != myHead)
			child = p->right;
		else
			child = myHead;

		if (p == myHead->parent) // p points to the root; Case 3 in "Ch 10 Sec 3.pptx"
		{
			// set child to be the new root
			child->color = BLACK;
			myHead->parent = child;
			child->parent = myHead;

			if (myHead->left == p)
				myHead->left = child;

			if (myHead->right == p)
				myHead->right = child;

		}
		else
		{
			TreeNode<value_type>* pp = p->parent;

			// Case 1
			if (pp->left == p)
				LinkStart(pp, child, 'L');
			else
				LinkStart(pp, child, 'R');

			if (p->color == 1)
				if (child->color == 0) // Case 2 in "Ch 10 Sec 3.pptx"
					child->color = 1;
				else                   // Case 4 in "Ch 10 Sec 3.pptx"
					fixUp(child, pp);
		}

		delete p;
		mySize--;
	}


	// rebalance for deletion; Case 4 in "Ch 10 Sec 3.pptx"
	void fixUp(TreeNode<value_type>* N, TreeNode<value_type>* P)
	{
		TreeNode< value_type >* S = Sibling(N, P);


		if (S->color == BLACK && S->right->color == BLACK && S->left->color == BLACK && P->color == RED) // Case 4.4
		{
			ChangeColor(S, P);
		}
		else if (S->color == BLACK && S->right->color == RED && N == P->left) // Case 4.2.1 
		{
			ChangeColor(P, S);
			S->right->color = BLACK;
			RRRotation(S);
		}
		else if (S->color == BLACK && S->left->color == RED && N == P->right) // Case 4.2.2
		{
			ChangeColor(P, S);
			S->left->color = BLACK;
			LLRotation(S);
		}
		else if (S->color == BLACK && S->right->color == BLACK && S->left->color == RED && N == P->left) // Case 4.3.1
		{
			ChangeColor(S, S->left);
			LLRotation(S->left);
			fixUp(N, P);
		}
		else if (S->color == BLACK && S->left->color == BLACK && S->right->color == RED && N == P->right) //Case 4.3.2
		{
			ChangeColor(S, S->right);
			RRRotation(S->right);
			fixUp(N, P);
		}
		else if (S->color == RED && N == P->left) // Case 4.1.1
		{
			ChangeColor(P, S);
			RRRotation(S);
			fixUp(N, P);
		}
		else if (S->color == RED && N == P->right)
		{
			ChangeColor(P, S);
			LLRotation(S);
			fixUp(N, P);
		}
		else if (S->color == BLACK && S->right->color == BLACK && S->left->color == BLACK && P->color == BLACK)
		{
			S->color = 0;
			// error
			if (P != myHead)
				fixUp(P, P->parent);
		}
	}

	void ChangeColor(TreeNode<value_type>* right, TreeNode<value_type>* left)
	{
		char temp = right->color;
		right->color = left->color;
		left->color = temp;
	}

	void SwapColor(TreeNode<value_type>* right, TreeNode<value_type>* left)
	{
		char temp = right->color;
		right->color = left->color;
		left->color = temp;
	}

	NodePtr myHead;   // pointer to head node
	size_type mySize; // number of elements
};

// CLASS TEMPLATE Tree
template<typename Traits>
class Tree // ordered red-black tree for map/multimap/set/multiset
{
public:
	using value_type = typename Traits::value_type;

protected:
	using ScaryVal = TreeVal<value_type>;

public:
	using key_type = typename Traits::key_type;
	using key_compare = typename Traits::key_compare;

	using size_type = size_t;

	Tree(const key_compare& parg)
		: keyCompare(parg),
		scaryVal()
	{
	}

	~Tree()
	{
	}

	/// <summary>
	/// Extends the container by inserting a new element,
	/// effectively increasing the container size by one.
	/// </summary>
	/// <param name="val"> 要插入的值</param>
	void insert(const value_type& val)
	{
		if (scaryVal.mySize == 0)
		{
			TreeNode<value_type>* root = new TreeNode<value_type>;
			root->myval = val;
			root->left = scaryVal.myHead;
			root->right = scaryVal.myHead;
			root->isNil = 0;
			root->parent = scaryVal.myHead;
			root->color = 1;
			scaryVal.myHead->left = root;
			scaryVal.myHead->parent = root;
			scaryVal.myHead->right = root;
			scaryVal.mySize = 1;
		}
		else
		{                           // scaryVal.myHead->parent points to the root
			TreeNode<value_type>* p = scaryVal.myHead->parent; // p points to the root
			TreeNode<value_type>* pp = nullptr;
			while (p != scaryVal.myHead)
			{
				pp = p;
				//              keyCompare.operator()( val, p->myval )
				if (keyCompare(val, p->myval)) // if( val < p->myval )
					p = p->left;
				else if (keyCompare(p->myval, val)) // if( val > p->myval )
					p = p->right;
				else
					return;
			}

			TreeNode<value_type>* newNode = new TreeNode<value_type>;
			newNode->myval = val;
			newNode->left = scaryVal.myHead;
			newNode->right = scaryVal.myHead;
			newNode->isNil = 0;
			newNode->parent = pp;
			newNode->color = RED;

			//scaryVal.reBalance(newNode);

			if (keyCompare(val, pp->myval))
				pp->left = newNode;
			else
				pp->right = newNode;

			if (pp->color == RED)
				scaryVal.reBalance(newNode);

			if (keyCompare(val, scaryVal.myHead->left->myval))
				scaryVal.myHead->left = newNode;

			if (keyCompare(scaryVal.myHead->right->myval, val))
				scaryVal.myHead->right = newNode;

			scaryVal.mySize++;

		}
	}


	/// <summary>
	/// Removes from the set container a single element whose value is val
	/// This effectively reduces the container size by one, which are destroyed.
	/// Returns the number of elements erased.
	/// </summary>
	/// <param name="val"></param>
	/// <returns>1 if erase </returns>
	size_type erase(const key_type& val)
	{
		TreeNode<key_type>* p = scaryVal.myHead->parent; // p points to the root
		while (p != scaryVal.myHead && val != p->myval)
		{
			//           keyCompare.operator()( val, p->myval )
			if (keyCompare(val, p->myval)) // if( val < p->myval )
				p = p->left;
			else
				p = p->right;
		}

		if (p == scaryVal.myHead) // not found
			return 0;
		else // found
		{  // deletes the node pointed by p
/**/

			if (p->left == scaryVal.myHead || p->right == scaryVal.myHead)// 分一個child 沒CHILD
				scaryVal.eraseDegreeOne(p);
			else // 兩個child(找柚樹最小的
			{
				TreeNode<key_type>* iter = p->right;

				while (iter->left != scaryVal.myHead)
				{
					iter = iter->left;
				}

				p->myval = iter->myval;
				scaryVal.eraseDegreeOne(iter);

			}

			// connection max min
			TreeNode< key_type >* leftMin = scaryVal.myHead->parent;
			TreeNode< key_type >* rightMax = scaryVal.myHead->parent;

			while (leftMin->left != scaryVal.myHead)
				leftMin = leftMin->left;

			while (rightMax->right != scaryVal.myHead)
				rightMax = rightMax->right;

			scaryVal.myHead->left = leftMin;
			scaryVal.myHead->right = rightMax;
			return 1;

		}
	}

private:
	key_compare keyCompare;
	ScaryVal scaryVal;
};

#endif // XTREE
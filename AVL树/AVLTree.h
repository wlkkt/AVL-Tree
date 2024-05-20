#pragma once

#include <iostream>
using namespace std;


template<class K, class V>
struct  AVLTreeNode
{
	//三叉链结构便于向上寻找自己的父亲、爷爷等长辈结点并修改它们的平衡因子
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	pair<K, V> _kv;

	int _bf; //平衡因子


	AVLTreeNode(const pair<K,V>& kv)
		:_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_kv(kv)
		,_bf(0)
	{}
};


template<class K, class V>
class  AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;

		//遍历寻找
		while (cur)
		{
			if (cur->_kv.first < kv.first)//当前结点的值小于插入结点的值，就进入右子树判断
			{
				parent = cur;//走之前保留之前的信息，便于找到时向前一个结点尾插
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)//当前结点的值大于插入结点的值，就进入左子树判断
			{
				parent = cur;
				cur = cur->left;
			}
			else//相同时
			{
				return false;//不允许冗余
			}
		}
		//插入新节点
		cur = new Node(kv);

		if (cur->_kv.first < kv.first)
		{
			parent->_right = cur;
		}
		else
		{
			parent->_left = cur;
		}

		cur->_parent = parent;//插入每个新结点时，都将该结点和它的父亲结点连接

		return true;//成功插入返回true
	}

	//循环更新平衡因子
	while ()
	{
		if (cur == parent->_left)//若新插入的结点是父结点的左结点，则父结点的平衡因子++
		{
			parent->_bf--;
		}
		else//若新插入的结点是父结点的左结点，则父结点的平衡因子++
		{
			parent->_bf++;
		}

		if (parent->_bf == 0)//若插入并更新父结点的平衡因子后，父结点的平衡因子的值变为0，那么就不需要继续向上更新平衡因子，退出循环
		{
			break;
		}
		else if(parent->_bf == 1 || parent->_bf == -1)
		{
			//继续向上更新
			cur = parent;
			parent = parent->_parent;
		}
		else if (parent->_bf == 2 || parent->_bf == -2)
		{
			//进行旋转
			break;//防止一直更新所以在旋转后就直接退出
		}
		else//一般情况下不会出错，但为了防止特殊情况导致的出错
		{
			assert(false);
		}
	}





	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->key < key)
			{
				cur = cur->right;
			}
			else if (cur->key > key)
			{
				cur = cur->left;
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	}


	//右旋
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}

			subL->_parent = ppNode;
		}

		parent->_bf = subL->_bf = 0;
	}



private:
		Node* _root = nullptr;
};
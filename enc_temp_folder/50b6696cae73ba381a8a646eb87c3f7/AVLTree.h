#pragma once

#include <iostream>
#include <assert.h>
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

		//主要负责确定要插入结点的父结点位置，同时保证不会出现冗余
		//cur负责为parent探路，最后cur一定指向空（相当于没用后被置空），parent指向的结点就是插入结点的父结点
		while (cur)
		{
			if (cur->_kv.first < kv.first)
			{
				parent = cur;//走之前保留之前的信息，便于找到时向前一个结点尾插
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else//相同时
			{
				return false;//不允许冗余
			}
		}

		//插入新节点
		cur = new Node(kv);//重新令cur指向新申请的结点

		//新结点的k小于parent指向结点的k
		if (kv.first < parent->_kv.first)
		{
			parent->_left = cur;//将新结点插入到父结点的左子树上
		}
		else
		{
			parent->_right = cur;//将新结点插入父结点的右子树上
		}

		cur->_parent = parent;//新结点插入后，新结点有了父亲结点，它的父亲结点就是parent指向的结点，所以将cur的_parent更新为parent



		//更新平衡因子
		while (parent)
		{
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}


			if (parent->_bf == 0)
			{
				//更新结束
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				//继续往上更新
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{

				if (parent->_bf == 2 && cur->_bf == 1)
				{
					RotateL(parent);//2 + 1 == LL右旋
				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					RotateLR(parent);//2 + -1 == LR左右旋
				}
				else if (parent->_bf == -2 && cur->_bf == -1)
				{
					RotateR(parent);//-2 + -1  == RR左旋
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateRL(parent);//-2 + 1 == RL右左旋
				}

				break;
			}
			else
			{
				// 理论而言不可能出现这个情况
				assert(false);
			}
		}
	}

	//查找函数
	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first < key)
			{
				cur = cur->_right;
			}
			else if (cur->_kv.first > key)
			{
				cur = cur->_left;
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	}

	//LL右旋
	//右旋完全版（parent指向的结点可能不是初始根节点）
	void RotateR(Node* parent)//传入的参数是平衡因子不为1、0、-1的结点
	{
		Node* subL = parent->_left;//subL指向parent结点的左结点
		Node* subLR = subL->_right;//subLR指向subL指向的结点的右结点

		parent->_left = subLR;//parent的左结点更新为subLR指向的结点

		//subLR可能为空，只有它不为空的时候才去更新它的父亲信息
		if (subLR)
		{
			subLR->_parent = parent;//更新subLR的父结点信息
		}

		subL->_right = parent;//subL的右结点变为了parent指向的结点

		Node* ppNode = parent->_parent;//先保存自己原先得父亲结点信息

		parent->_parent = subL;//后将parent指向的结点中存放的父结点信息由原来的某个结点变为subL

		//如果parent指向的结点的确是初始根结点
		if (parent == _root)//_root指向原始根节点,在建树的时候就已经确定
		{
			_root = subL;//右旋后应该是subL作为初始根节点，令_root也指向subL指向的结点
			_root->_parent = nullptr;//初始根节点没有父结点，将存放父结点信息的指针置空
		}
		else//如果parent指向的结点不是初始根结点
		{
			if (ppNode->_left == parent)//如果parent是原父结点的左结点
			{
				ppNode->_left = subL;//将原父结点中存放的左结点信息变为subL
			}
			else//如果parent是原父结点的右结点
			{
				ppNode->_right = subL;//将原父结点中存放的右结点信息变为subL
			}

			subL->_parent = ppNode;//同时将subL中存放的父结点的信息更新为parent之前的父结点
		}

		parent->_bf = subL->_bf = 0;//更新平衡因子
	}

	//RR左旋
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		subR->_left = parent;
		Node* ppNode = parent->_parent;

		parent->_parent = subR;

		if (parent == _root)
		{
			_root = subR;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_right == parent)
			{
				ppNode->_right = subR;
			}
			else
			{
				ppNode->_left = subR;
			}
			subR->_parent = ppNode;
		}

		parent->_bf = subR->_bf = 0;
	}

	void RotateLR(Node* parent)
	{
		//这里的subL和subLR的仅用于修改父亲和爷爷结点的bf
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
 		int bf = subLR->_bf;

		RotateL(subL);
		RotateR(parent);

		subLR->_bf = 0;//旋转完后subLR指向的结点的bf为0

		if (bf == 1)
		{
			parent->_bf = -1;
			subL->_bf = 0;
		}
		else if (bf == -1)
		{
			parent->_bf = 0;
			subL->_bf = 1;
		}
		{
			parent->_bf = 0;
			subL->_bf = 0;
		}
	}
	
	
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(subR);
		RotateL(parent);

		subRL->_bf = 0;//旋转完后subRL指向的结点的bf为0

		if (bf == 1)
		{
			parent->_bf = 0;
			subR->_bf = -1;
		}
		else if (bf == -1)
		{
			parent->_bf = 1;
			subR->_bf = 0;
		}
		else
		{
			parent->_bf = 0;
			subR->_bf = 0;
		}
	}


	//中序遍历函数
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//判断是否平衡
	bool IsBalance()
	{
		return _IsBalance(_root);
	}

	//判断高度
	int Height()
	{
		return _Height(_root);
	}

	//判断结点个数
	int Size()
	{
		return _Size(_root);
	}

	//避免接口暴露，让处理函数的权限为private
private:

	int _Size(Node* root)
	{
		return root == nullptr ? 0 : _Size(root->_left) + _Size(root->_right) + 1;
	}

	int _Height(Node* root)
	{
		if (root == nullptr)
			return 0;

		return max(_Height(root->_left), _Height(root->_right)) + 1;
	}

	bool _IsBalance(Node* root)
	{
		if (root == nullptr)
			return true;

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		// 不平衡
		if (abs(leftHeight - rightHeight) >= 2)
		{
			cout << root->_kv.first << endl;
			return false;
		}

		// 顺便检查一下平衡因子是否正确
		if (rightHeight - leftHeight != root->_bf)
		{
			cout << root->_kv.first << endl;
			return false;
		}

		return _IsBalance(root->_left)
			&& _IsBalance(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

private:
		Node* _root = nullptr;
};


void TestAVLTree1()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t1;
	for (auto e : a)
	{
		t1.Insert({ e,e });
	}

	/*t1.InOrder();
	cout << t1.IsBalance() << endl;*/
}
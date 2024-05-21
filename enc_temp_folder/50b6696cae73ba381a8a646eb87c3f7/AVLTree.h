#pragma once

#include <iostream>
#include <assert.h>
using namespace std;


template<class K, class V>
struct  AVLTreeNode
{
	//�������ṹ��������Ѱ���Լ��ĸ��ס�үү�ȳ�����㲢�޸����ǵ�ƽ������
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	pair<K, V> _kv;

	int _bf; //ƽ������


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

		//��Ҫ����ȷ��Ҫ������ĸ����λ�ã�ͬʱ��֤�����������
		//cur����Ϊparent̽·�����curһ��ָ��գ��൱��û�ú��ÿգ���parentָ��Ľ����ǲ�����ĸ����
		while (cur)
		{
			if (cur->_kv.first < kv.first)
			{
				parent = cur;//��֮ǰ����֮ǰ����Ϣ�������ҵ�ʱ��ǰһ�����β��
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else//��ͬʱ
			{
				return false;//����������
			}
		}

		//�����½ڵ�
		cur = new Node(kv);//������curָ��������Ľ��

		//�½���kС��parentָ�����k
		if (kv.first < parent->_kv.first)
		{
			parent->_left = cur;//���½����뵽��������������
		}
		else
		{
			parent->_right = cur;//���½����븸������������
		}

		cur->_parent = parent;//�½�������½�����˸��׽�㣬���ĸ��׽�����parentָ��Ľ�㣬���Խ�cur��_parent����Ϊparent



		//����ƽ������
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
				//���½���
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				//�������ϸ���
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{

				if (parent->_bf == 2 && cur->_bf == 1)
				{
					RotateL(parent);//2 + 1 == LL����
				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					RotateLR(parent);//2 + -1 == LR������
				}
				else if (parent->_bf == -2 && cur->_bf == -1)
				{
					RotateR(parent);//-2 + -1  == RR����
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateRL(parent);//-2 + 1 == RL������
				}

				break;
			}
			else
			{
				// ���۶��Բ����ܳ���������
				assert(false);
			}
		}
	}

	//���Һ���
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

	//LL����
	//������ȫ�棨parentָ��Ľ����ܲ��ǳ�ʼ���ڵ㣩
	void RotateR(Node* parent)//����Ĳ�����ƽ�����Ӳ�Ϊ1��0��-1�Ľ��
	{
		Node* subL = parent->_left;//subLָ��parent��������
		Node* subLR = subL->_right;//subLRָ��subLָ��Ľ����ҽ��

		parent->_left = subLR;//parent���������ΪsubLRָ��Ľ��

		//subLR����Ϊ�գ�ֻ������Ϊ�յ�ʱ���ȥ�������ĸ�����Ϣ
		if (subLR)
		{
			subLR->_parent = parent;//����subLR�ĸ������Ϣ
		}

		subL->_right = parent;//subL���ҽ���Ϊ��parentָ��Ľ��

		Node* ppNode = parent->_parent;//�ȱ����Լ�ԭ�ȵø��׽����Ϣ

		parent->_parent = subL;//��parentָ��Ľ���д�ŵĸ������Ϣ��ԭ����ĳ������ΪsubL

		//���parentָ��Ľ���ȷ�ǳ�ʼ�����
		if (parent == _root)//_rootָ��ԭʼ���ڵ�,�ڽ�����ʱ����Ѿ�ȷ��
		{
			_root = subL;//������Ӧ����subL��Ϊ��ʼ���ڵ㣬��_rootҲָ��subLָ��Ľ��
			_root->_parent = nullptr;//��ʼ���ڵ�û�и���㣬����Ÿ������Ϣ��ָ���ÿ�
		}
		else//���parentָ��Ľ�㲻�ǳ�ʼ�����
		{
			if (ppNode->_left == parent)//���parent��ԭ����������
			{
				ppNode->_left = subL;//��ԭ������д�ŵ�������Ϣ��ΪsubL
			}
			else//���parent��ԭ�������ҽ��
			{
				ppNode->_right = subL;//��ԭ������д�ŵ��ҽ����Ϣ��ΪsubL
			}

			subL->_parent = ppNode;//ͬʱ��subL�д�ŵĸ�������Ϣ����Ϊparent֮ǰ�ĸ����
		}

		parent->_bf = subL->_bf = 0;//����ƽ������
	}

	//RR����
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
		//�����subL��subLR�Ľ������޸ĸ��׺�үү����bf
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
 		int bf = subLR->_bf;

		RotateL(subL);
		RotateR(parent);

		subLR->_bf = 0;//��ת���subLRָ��Ľ���bfΪ0

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

		subRL->_bf = 0;//��ת���subRLָ��Ľ���bfΪ0

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


	//�����������
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//�ж��Ƿ�ƽ��
	bool IsBalance()
	{
		return _IsBalance(_root);
	}

	//�жϸ߶�
	int Height()
	{
		return _Height(_root);
	}

	//�жϽ�����
	int Size()
	{
		return _Size(_root);
	}

	//����ӿڱ�¶���ô�������Ȩ��Ϊprivate
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
		// ��ƽ��
		if (abs(leftHeight - rightHeight) >= 2)
		{
			cout << root->_kv.first << endl;
			return false;
		}

		// ˳����һ��ƽ�������Ƿ���ȷ
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
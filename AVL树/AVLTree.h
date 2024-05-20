#pragma once

#include <iostream>
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

		//����Ѱ��
		while (cur)
		{
			if (cur->_kv.first < kv.first)//��ǰ����ֵС�ڲ������ֵ���ͽ����������ж�
			{
				parent = cur;//��֮ǰ����֮ǰ����Ϣ�������ҵ�ʱ��ǰһ�����β��
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)//��ǰ����ֵ���ڲ������ֵ���ͽ����������ж�
			{
				parent = cur;
				cur = cur->left;
			}
			else//��ͬʱ
			{
				return false;//����������
			}
		}
		//�����½ڵ�
		cur = new Node(kv);

		if (cur->_kv.first < kv.first)
		{
			parent->_right = cur;
		}
		else
		{
			parent->_left = cur;
		}

		cur->_parent = parent;//����ÿ���½��ʱ�������ý������ĸ��׽������

		return true;//�ɹ����뷵��true
	}

	//ѭ������ƽ������
	while ()
	{
		if (cur == parent->_left)//���²���Ľ���Ǹ��������㣬�򸸽���ƽ������++
		{
			parent->_bf--;
		}
		else//���²���Ľ���Ǹ��������㣬�򸸽���ƽ������++
		{
			parent->_bf++;
		}

		if (parent->_bf == 0)//�����벢���¸�����ƽ�����Ӻ󣬸�����ƽ�����ӵ�ֵ��Ϊ0����ô�Ͳ���Ҫ�������ϸ���ƽ�����ӣ��˳�ѭ��
		{
			break;
		}
		else if(parent->_bf == 1 || parent->_bf == -1)
		{
			//�������ϸ���
			cur = parent;
			parent = parent->_parent;
		}
		else if (parent->_bf == 2 || parent->_bf == -2)
		{
			//������ת
			break;//��ֹһֱ������������ת���ֱ���˳�
		}
		else//һ������²��������Ϊ�˷�ֹ����������µĳ���
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


	//����
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
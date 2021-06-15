#pragma once
#include <cstring>
#define MAX 100				// set MAX

using namespace std;
class treeNode
{
	friend class Tree;		// friend class with Tree

private:
	treeNode() {
		Parent_Node = nullptr;
		for (int i = 0; i < MAX; i++) {	// make all child node to null pointer
			Child_Node[i] = nullptr;
		}
		data = new char[256];
		data[0] = '\0';		// set data
	}
	~treeNode() {

	}

private:
	char* data;		// node's data
	treeNode* Parent_Node;	// parent node
	treeNode* Child_Node[MAX];	// child node
};

class Tree
{
public:
	Tree() {
		m_Head = new NODE;
	}
	~Tree() {

	}

private:		/* type define */
	typedef treeNode NODE;
	typedef treeNode* PNODE;

private:
	PNODE m_Head;	// head

public:
	void push_head(char* dat) {	// push head data
		PNODE pNode = new NODE;	// make pNode
		pNode->data = dat;		// input data in head

		// link header
		m_Head->Child_Node[0] = pNode;
		pNode->Parent_Node = m_Head;
	}

	void push_lv2(char* dat) {	// push lv2 data
		PNODE pNode = new NODE;		// make pNode
		PNODE Head = m_Head->Child_Node[0];	// Head = head node

		pNode->data = dat;	// set data

		PNODE child;
		int i = 0;
		while (1) {		// get empty child node
			child = Head->Child_Node[i];
			if (child == nullptr)	// found empty child node
				break;
			i++;
		}

		child = pNode;

		// link header and lv2
		Head->Child_Node[i] = child;
		child->Parent_Node = Head;
	}

	void push_lv3(char* cmp, char* dat) {	// push lv3 data
		PNODE Head = m_Head->Child_Node[0];	// Head : head node
		PNODE pNode = new NODE;				// make pNode
		pNode->data = dat;	// set data

		PNODE Check;
		int i = 0;
		for (;; i++) {		// check all node
			Check = Head->Child_Node[i];
			if (strcmp(cmp, Check->data) == 0)	// check if data is same
				break;	// set i
		}

		PNODE child;
		int j = 0;
		while (1) {
			child = Check->Child_Node[j];
			if (child == nullptr)	// go to end child node
				break;
			j++;	// set j
		}

		child = pNode;

		// link lv2 node and lv3
		Check->Child_Node[j] = child;
		child->Parent_Node = Check;
	}

	char* get_lv2(int idx) {					// get lv2's data
		PNODE Head = m_Head->Child_Node[0];		// set head node
		return Head->Child_Node[idx]->data;		// return lv2's data
	}

	char* get_lv3(char* dat, int idx) {			// get lv3's data
		PNODE Head = m_Head->Child_Node[0];		// set head
		
		for (int i = 0;; i++) {					// check all node
			PNODE lv2 = Head->Child_Node[i];	// set lv2 node
			if (strcmp(lv2->data, dat) == 0) {	// check if data is same
				return lv2->Child_Node[idx]->data;	// return the data
			}
		}
	}

	int get_child_num(char* dat) {		// get child's number (lv3)
		PNODE Head = m_Head->Child_Node[0];	// set header node

		int num = 0;	// return value
		for (int i = 0;; i++) {
			PNODE lv2 = Head->Child_Node[i];	// check all lv2 node
			if (lv2 == nullptr)	// check all
				break;
			if (strcmp(lv2->data, dat) == 0) {	// found lv2 node which name is 'dat'
				int j = 0;
				while (1) {
					if (lv2->Child_Node[j] == nullptr) {	// check all lv3 node
						return num;	// return the number of child
					}
					num++;	// count up
					j++;
				}
			}
		}

		return -1;	// not found
	}
};


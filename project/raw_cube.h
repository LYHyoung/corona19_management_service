#pragma once
#include <cstring>
class raw_cube_Node
{
	friend class raw_cube;

private:
	raw_cube_Node() {
		product = new char[256];
		product[0] = '\0';
		location = new char[256];
		location[0] = '\0';
		time = new char[256];
		time[0] = '\0';

		cnt = 0;

		nxt_product = nullptr;
		prv_product = nullptr;
		nxt_location = nullptr;
		prv_location = nullptr;
		nxt_time = nullptr;
		prv_time = nullptr;
	}
	~raw_cube_Node() {

	}

private:
	char* product;					// product data
	char* location;					// location data
	char* time;						// time data
	int cnt;						// sales data
	raw_cube_Node* nxt_product;		// next product node
	raw_cube_Node* prv_product;		// previous product node
	raw_cube_Node* nxt_location;	// next location node
	raw_cube_Node* prv_location;	// previous location node
	raw_cube_Node* nxt_time;		// next time node
	raw_cube_Node* prv_time;		// previous time node
};

class raw_cube
{
public:
	raw_cube() {
		head_product = new NODE;
		head_location = new NODE;
		head_time = new NODE;
	}
	~raw_cube() {

	}

private:		/* type define */
	typedef raw_cube_Node NODE;
	typedef raw_cube_Node* PNODE;

private:
	PNODE head_product;		// head of product
	PNODE head_location;	// head of location
	PNODE head_time;		// head of time

public:
	void set(char* prod, char* loc, char* tim) {		// set raw cube
		// first input
		if (head_product->nxt_product == nullptr && head_location->nxt_location == nullptr && head_time->nxt_time == nullptr) {
			PNODE newHead = new NODE;		// set newHead
			newHead->product = prod;
			newHead->location = loc;
			newHead->time = tim;

			// link with header and new Head node
			head_product->nxt_product = newHead;
			head_location->nxt_location = newHead;
			head_time->nxt_time = newHead;
			newHead->prv_product = head_product;
			newHead->prv_location = head_location;
			newHead->prv_time = head_time;

			return;	// end
		}

		PNODE pNode = new NODE;			// make new node
		pNode->product = prod;			// set pNode(new node)
		pNode->location = loc;
		pNode->time = tim;

		PNODE Check = head_product->nxt_product;	// checking node
		PNODE pNext = head_product->nxt_product;	// next node

		// push pNode
		while (1) {					// check product
			if (pNext == nullptr || strcmp(pNext->product, "") == 0) {	// new product node
				Check->nxt_product = pNode;
				pNode->prv_product = Check;		// link pNode and Check node
				break;
			}
			Check = pNext;
			
			if (strcmp(Check->product, prod) == 0) {
				while (1) {			// check location
					if (strcmp(Check->location, loc) == 0) {	// already exist location
						while (1) {
							if (Check->nxt_time == nullptr || strcmp(Check->nxt_time->time, "") == 0) {	// empty next time node
								Check->nxt_time = pNode;	// link pNode and Check node
								pNode->prv_time = Check;
								break;
							}
							Check = Check->nxt_time;	// next time
						}
						break;
					}
					if (Check->nxt_location == nullptr || strcmp(Check->nxt_time->location, "") == 0) {	// new location node
						Check->nxt_location = pNode;	// link pNode and Check node
						pNode->prv_location = Check;
						break;
					}
					Check = Check->nxt_location;		// next location
				}
				break;
			}
			pNext = Check->nxt_product;		// next product
		}
	}

	void push(char* prod, char* loc, char* tim, int cnt) {
		PNODE pNode = head_product->nxt_product;		// set pNode
		while (1) {
			if (strcmp(pNode->product, prod) == 0) {	// same product
				while (1) {
					if (strcmp(pNode->location, loc) == 0) {		// same location
						while (1) {
							if (strcmp(pNode->time, tim) == 0) {	// same time
								pNode->cnt = cnt;	// set cnt(sales)
								
								break;
							}
							pNode = pNode->nxt_time;	// go to next time
						}

						break;
					}
					pNode = pNode->nxt_location;		// go to next location
				}

				break;
			}
			pNode = pNode->nxt_product;					// go to product
		}
	}

	int get_count(int s_p, int mv_p, int s_l, int mv_l, int s_t, int mv_t) {	// get sum of cnt
		int sum = 0;	// return value
		PNODE pNode = head_product->nxt_product;	// set pNode into head node
		for (int i = 0; i < s_p; i++)				// move to start position of product
			pNode = pNode->nxt_product;

		for (int i = 0; i < mv_p; i++) {			// move position of prodcut
			PNODE lNode = pNode;
			for (int j = 0; j < s_l; j++)			// move to start position of location
				lNode = lNode->nxt_location;

			for (int j = 0; j < mv_l; j++) {		// move position of location
				PNODE tNode = lNode;
				for (int k = 0; k < s_t; k++)		// move to start position of time
					tNode = tNode->nxt_time;

				for (int k = 0; k < mv_t; k++) {	// move position of time
					sum += tNode->cnt;				// set sum
					tNode = tNode->nxt_time;
				}
				lNode = lNode->nxt_location;
			}

			pNode = pNode->nxt_product;
		}

		return sum;	// return the sum
	}

	int get_num(char* prod, char* loc, char* time) {		// get the number
		PNODE pProd = head_product->nxt_product;	// set pProd
		while (pProd != nullptr) {		// check all node(product)
			PNODE pLoc = pProd;			// set pLoc
			
			if (strcmp(pProd->product, prod) == 0) {	// same product found
				while (pLoc != nullptr) {	// check all node(location)
					PNODE pTime = pLoc;		// seet pTime

					if (strcmp(pLoc->location, loc) == 0) {	// same location found
						while (pTime != nullptr) {		// check all node(time)
							if (strcmp(pTime->time, time) == 0) {	// same time found
								return pTime->cnt;		// return time's sales data
							}
							pTime = pTime->nxt_time;	// next time
						}
					}

					pLoc = pLoc->nxt_location;	// next location
				}
			}

			pProd = pProd->nxt_product;	// next product
		}

		return -1;	// error return
	}
};


#pragma once
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "raw_cube.h"
#include "Tree.h"
using namespace std;

class view_cube_Node
{
	friend class view_cube;

private:
	view_cube_Node() {
		product = new char[256];
		product[0] = '\0';
		location = new char[256];
		location[0] = '\0';
		time = new char[256];
		time[0] = '\0';

		sum = 0;

		nxt_product = nullptr;
		prv_product = nullptr;
		nxt_location = nullptr;
		prv_location = nullptr;
		nxt_time = nullptr;
		prv_time = nullptr;
	}
	~view_cube_Node() {

	}

private:
	char* product;
	char* location;
	char* time;
	int sum;
	view_cube_Node* nxt_product;
	view_cube_Node* prv_product;
	view_cube_Node* nxt_location;
	view_cube_Node* prv_location;
	view_cube_Node* nxt_time;
	view_cube_Node* prv_time;
};

class view_cube
{
public:
	view_cube() {
		head_product = new NODE;
		head_location = new NODE;
		head_time = new NODE;
		front_name = new char[256];
	}
	~view_cube() {

	}

private:
	typedef view_cube_Node NODE;
	typedef view_cube_Node* PNODE;

private:
	PNODE head_product;
	PNODE head_location;
	PNODE head_time;
	char* front_name;

public:
	// set the view cube
	void set(char* prod, char* loc, char* time) {
		// first input
		if (head_product->nxt_product == nullptr && head_location->nxt_location == nullptr && head_time->nxt_time == nullptr) {
			PNODE newHead = new NODE;		// make new Head node
			newHead->product = prod;		// set new Head
			newHead->location = loc;
			newHead->time = time;

			// link with header and new Head node
			head_product->nxt_product = newHead;
			head_location->nxt_location = newHead;
			head_time->nxt_time = newHead;

			newHead->prv_product = head_product;
			newHead->prv_location = head_location;
			newHead->prv_time = head_time;

			return;		// exit
		}

		PNODE pNode = new NODE;		// make new node (pNode)
		pNode->product = prod;		// set pNode
		pNode->location = loc;
		pNode->time = time;

		PNODE Check = head_product->nxt_product;	// Check node
		PNODE pNext = head_product->nxt_product;	// pNext node

		// push pNode
		while (1) {					// check product
			if (pNext == nullptr || strcmp(pNext->product, "") == 0) {	// new product node
				Check->nxt_product = pNode;		// linke Check and pNode
				pNode->prv_product = Check;
				break;
			}
			Check = pNext;	// set Check to pNext

			if (strcmp(Check->product, prod) == 0) {
				while (1) {			// check location
					if (strcmp(Check->location, loc) == 0) {	// already exist location
						while (1) {
							if (Check->nxt_time == nullptr || strcmp(Check->nxt_time->time, "") == 0) {	// empty next time node
								Check->nxt_time = pNode;	// linke Check and pNode
								pNode->prv_time = Check;
								break;
							}
							Check = Check->nxt_time;	// next time
						}
						break;
					}
					if (Check->nxt_location == nullptr || strcmp(Check->nxt_time->location, "") == 0) {	// empty next location node
						Check->nxt_location = pNode;	// linke Check and pNode
						pNode->prv_location = Check;
						break;
					}
					Check = Check->nxt_location;		// next location
				}
				break;
			}
			pNext = Check->nxt_product;			// next product
		}

		front_name = head_time->nxt_time->time;
	}

	// push to view cube
	void push(int sum, char* prod, char* loc, char* tim) {
		PNODE pNode = head_product->nxt_product;	// set pNode to head node
		while (1) {
			if (strcmp(pNode->product, prod) == 0) {	// same product
				while (1) {
					if (strcmp(pNode->location, loc) == 0) {	// same location
						while (1) {
							if (strcmp(pNode->time, tim) == 0) {	// same time
								pNode->sum = sum;	// set pNode's sum

								break;
							}
							pNode = pNode->nxt_time;	// move to next time node
						}

						break;
					}
					pNode = pNode->nxt_location;		// move to next location node
				}

				break;
			}
			pNode = pNode->nxt_product;					// move to next product node
		}
	}

	// used in VIEW
	void clear() {			// clear the view cube
		PNODE pProd = head_product->nxt_product;	// set pProd to head node

		// product
		while (1) {
			if (pProd == nullptr) {		// if product pointer is null
				break;
			}
			PNODE pLoc = pProd;			// set pLoc
			PNODE pNext_prod = pProd->nxt_product;	// set pNext_prod

			// location
			while (1) {
				if (pLoc == nullptr) {	// check till end
					break;
				}
				PNODE pTime = pLoc;		// set pTime
				PNODE pNext_loc = pLoc->nxt_location;	// set pNext_loc

				// time
				while (1) {
					if (pTime == nullptr) {	// check till end
						break;
					}
					PNODE pNext_time = pTime->nxt_time;	// set next time node

					delete pTime;			// delete time node

					pTime = pNext_time;		// goto next time node
				}

				
				pLoc = pNext_loc;
			}

			pProd = pNext_prod;
		}

		// set new head
		head_product = new NODE;
		head_location = new NODE;
		head_time = new NODE;
	}

	// print the cube (front)
	void cube_print(ofstream& logfile) {
		PNODE pNode = head_product->nxt_product;	// set pNode to head node
		logfile.setf(ios::left);					// print in left
		logfile << setw(20) <<  pNode->time;

		// print column name
		PNODE pName = pNode;						// set pName to pNode
		while (pName != nullptr) {					// check all node
			logfile.setf(ios::left);
			logfile << '\t' << pName->location;		// print all location
			pName = pName->nxt_location;
		}
		logfile << '\n';

		while (pNode != nullptr) {					// check all node
			logfile.setf(ios::left);				// print in left most

			logfile << setw(18) << pNode->product;

			PNODE pSum = pNode;
			while (pSum != nullptr) {				// check all node
				logfile.setf(ios::left);
				logfile << /*setw(12)*/ '\t' << pSum->sum;
				pSum = pSum->nxt_location;
			}
			logfile << '\n'; 

			pNode = pNode->nxt_product;
		}
	}

	// SLICE time
	bool find_time(char* tim) {
		PNODE pNode = head_location->nxt_location;	// pNode : head node
		PNODE newHead = new NODE;		// make new node

		int check = 0;		// check flag
		while (pNode != nullptr) {		// check all node
			if (strcmp(pNode->time, tim) == 0) {	// same time found
				check = 1;		// flag on
				break;
			}
			pNode = pNode->nxt_time;	// set to next time node
		}
		if (check == 0)		// not found
			return 0;

		// set new Head node
		newHead->product = pNode->product;
		newHead->location = pNode->location;
		newHead->time = pNode->time;
		newHead->sum = pNode->sum;

		PNODE pCur = newHead;	// set pCur
		PNODE pNext = newHead;	// set pNext

		PNODE pProd = head_product->nxt_product;	// set pProd
		
		int nxt = 100;		// used as flas

		// product
		while (1) {
			if (pProd == nullptr)	// check all node (product)
				break;
			PNODE pLoc = pProd;	// set pLoc
			
			// location
			while (1) {
				if (pLoc == nullptr)		// check all node (location)
					break;
				PNODE pTime = pLoc;	// set pTime

				// time
				while (1) {
					if (strcmp(pTime->time, tim) == 0) {	// check if time is same
						if (nxt == 100)		// first time
							break;
						if (nxt == 0) {		// get from location
							PNODE newNode = new NODE;	// make new node

							// set new node
							newNode->product = pTime->product;
							newNode->location = pTime->location;
							newNode->time = pTime->time;
							newNode->sum = pTime->sum;

							// link pNext and new node
							pNext->nxt_product = newNode;
							newNode->prv_product = pNext;

							// set next node and current node
							pNext = newNode;
							pCur = newNode;

							break;
						}

						PNODE newNode = new NODE;		// make new node
						
						// set new node
						newNode->product = pTime->product;
						newNode->location = pTime->location;
						newNode->time = pTime->time;
						newNode->sum = pTime->sum;

						// linke with new node
						pCur->nxt_location = newNode;
						newNode->prv_location = pCur;

						// set current node
						pCur = newNode;

						break;
					}
					pTime = pTime->nxt_time;		// next time
					
				}
				nxt++;		// plus the flag
				pLoc = pLoc->nxt_location;		// next location
			}
			pProd = pProd->nxt_product;		// next product
			nxt = 0;		// 0 to flag
		}

		clear();		// clear the cube

		// link header with new Head
		head_product->nxt_product = newHead;
		newHead->prv_product = head_product;
		head_location->nxt_location = newHead;
		newHead->prv_location = head_location;
		head_time->nxt_time = newHead;
		newHead->prv_time = head_time;

		return 1;
	}

	// SLICE location
	bool find_loc(char* loc) {
		PNODE pNode = head_location->nxt_location;		// set pNode to head node
		PNODE newHead = new NODE;		// make new head node

		int check = 0;		// check flag
		while (pNode != nullptr) {		// check all node (location)
			if (strcmp(pNode->location, loc) == 0) {	// same location found
				check = 1;		// flag on
				break;
			}
			pNode = pNode->nxt_location;	// go to next location
		}
		if (check == 0)		// not found
			return 0;

		// set new Head node
		newHead->product = pNode->product;
		newHead->location = pNode->location;
		newHead->time = pNode->time;
		newHead->sum = pNode->sum;

		PNODE pCur = newHead;		// set current node
		PNODE pNext = newHead;		// set next node

		PNODE pProd = head_product->nxt_product;		// pProd

		int nxt = 100;		// used as flag

		// product
		while (1) {
			if (pProd == nullptr)	// check all node (product)
				break;
			PNODE pLoc = pProd;		// set pLoc

			// location
			while (1) {
				PNODE pTime = pLoc;	// set pTime
				if (strcmp(pLoc->location, loc) == 0) {	// same location found
					while (1) {
						if (nxt == 100) {		// first input
							nxt = 1;			// set nxt flag to 1
							pTime = pTime->nxt_time;	// set pTime to next time node
							continue;
						}

						if (nxt == 0) {			// product
							PNODE newNode = new NODE;	// make newnode

							// set new node
							newNode->product = pTime->product;
							newNode->location = pTime->location;
							newNode->time = pTime->time;
							newNode->sum = pTime->sum;

							// link pNext with new node
							pNext->nxt_product = newNode;
							newNode->prv_product = pNext;

							// set pNext and pCur to newnode;
							pNext = newNode;
							pCur = newNode;

							nxt = 1;	// set nxt flag to 1

							continue;
						}

						if (pTime == nullptr) {	// check all node (time)
							nxt = 0;	// set flag to 0
							break;
						}

						PNODE newNode = new NODE;		// make new node

						// set new node
						newNode->product = pTime->product;
						newNode->location = pTime->location;
						newNode->time = pTime->time;
						newNode->sum = pTime->sum;

						// link pCur with new node
						pCur->nxt_time = newNode;
						newNode->prv_time = pCur;

						// set current node to new node
						pCur = newNode;

						pTime = pTime->nxt_time;	// next time
					}
					break;
				}
				pLoc = pLoc->nxt_location;		// next location

			}
			pProd = pProd->nxt_product;		// next product
			nxt = 0;		// set nxt flag to 0
		}

		clear();		// clear the cube

		// link header with new head
		head_product->nxt_product = newHead;
		newHead->prv_product = head_product;
		head_location->nxt_location = newHead;
		newHead->prv_location = head_location;
		head_time->nxt_time = newHead;
		newHead->prv_time = head_time;

		return 1;
	}

	// SLICE product
	bool find_prod(char* prod) {
		PNODE pNode = head_location->nxt_location;		// set pNode with head node
		PNODE newHead = new NODE;		// make new head

		int check = 0;		// check flag
		while (pNode != nullptr) {		// check all node
			if (strcmp(pNode->product, prod) == 0) {	// same product found
				check = 1;		// flag on
				break;
			}
			pNode = pNode->nxt_product;	// next product
		}
		if (check == 0)		// not found
			return 0;
		
		// set new Head node
		newHead->product = pNode->product;
		newHead->location = pNode->location;
		newHead->time = pNode->time;
		newHead->sum = pNode->sum;

		PNODE pCur = newHead;	// set current node
		PNODE pNext = newHead;	// set next node

		PNODE pProd = head_product->nxt_product;	// set pProd

		int nxt = 100;		// set nxt flag

		// product
		while (1) {
			if (strcmp(pProd->product, prod) == 0) {	// same product found
				PNODE pLoc = pProd;		// set pLoc

				// location
				while (1) {
					if (pLoc == nullptr)		// check all location node
						break;
					PNODE pTime = pLoc;		// set pTime
					
					// time
					while (1) {
						if (nxt == 100) {		// first time
							nxt = 1;			// set nxt flag to 1
							pTime = pTime->nxt_time;	// next time
							continue;
						}

						if (nxt == 0) {			// location
							PNODE newNode = new NODE;		// make new node

							// set new node
							newNode->product = pTime->product;
							newNode->location = pTime->location;
							newNode->time = pTime->time;
							newNode->sum = pTime->sum;

							// link new node and pNext
							pNext->nxt_location = newNode;
							newNode->prv_location = pNext;

							// seet pNext and pCur to new node
							pNext = newNode;
							pCur = newNode;

							nxt = 1;		// nxt flag = 1

							continue;
						}

						if (pTime == nullptr) {	// check all time
							nxt = 0;
							break;
						}

						PNODE newNode = new NODE;		// make new node

						// set new node
						newNode->product = pTime->product;
						newNode->location = pTime->location;
						newNode->time = pTime->time;
						newNode->sum = pTime->sum;

						// link with current node and new node
						pCur->nxt_time = newNode;
						newNode->prv_time = pCur;

						// set current node to new node
						pCur = newNode;

						pTime = pTime->nxt_time;		// go to next time
					}

					pLoc = pLoc->nxt_location;		// go to next location
				}

				break;
			}

			pProd = pProd->nxt_product;			// next product
		}

		clear();		// clear the cube

		// link head with new head
		head_product->nxt_product = newHead;
		newHead->prv_product = head_product;
		head_location->nxt_location = newHead;
		newHead->prv_location = head_location;
		head_time->nxt_time = newHead;
		newHead->prv_time = head_time;

		return 1;
	}

	// DRILLDOWN time
	bool DD_time(char* tim, raw_cube* rc, Tree* tr, int n, int mv_p, int mv_l, int mv_t) {
		PNODE pNode = head_product->nxt_product;	// set pNode
		int check = 0;	// check flag

		while (pNode != nullptr) {
			if (strcmp(pNode->time, tim) == 0) {	// if there's same time
				check = 1;	// flag on
				break;
			}
			pNode = pNode->nxt_time;	// check next time
		}
		if (check == 0)		// if there's no 'tim' in cube
			return 0;

		int p = 0, l = 0, t = 0;	// position variable

		PNODE pProd = head_product->nxt_product;	// set pProd
		while (pProd != nullptr) {		// check all production
			PNODE pLoc = pProd;			// set pLoc

			while (pLoc != nullptr) {	// check all Location
				PNODE pTime = pLoc;

				while (pTime != nullptr) {	// check all time
					if (strcmp(pTime->time, tim) == 0) {	// same time found
						PNODE pPrev = pTime->prv_time;		// set previous node
						PNODE pNext = pTime->nxt_time;		// set next node

						for (int i = 0; i < n; i++) {		// get lv3 node to view cube
							int c = rc->get_count(p, mv_p, l, mv_l, t, 1);	// get count
							char* nam = new char[100];	// get time name
							nam = tr->get_lv3(pTime->time, i);	// get lv3's data in 'nam'
							
							PNODE newNode = new NODE;		// make new node
							newNode->time = nam;			// set new node
							newNode->location = pTime->location;
							newNode->product = pTime->product;
							newNode->sum = c;

							pPrev->nxt_time = newNode;		// link new node with pPrev
							newNode->prv_time = pPrev;

							pPrev = newNode;				// set pPrev

							t++;
						}

						pPrev->nxt_time = pNext;		// link pPrev and pNext
						pNext->prv_time = pPrev;
						break;
					}

					pTime = pTime->nxt_time;	// go to next time
					t += mv_t;	// plus the movement
				}
				t = 0;	// reset position
				pLoc = pLoc->nxt_location;	// go to next location
				l += mv_l;	// plus the movement
			}
			l = 0;	// reset position
			t = 0;
			pProd = pProd->nxt_product;	// go to next product
			p += mv_p;	// plus the movement
		}
	}

	// DRILLDOWN location
	bool DD_location(char* loc, raw_cube* rc, Tree* tr, int n, int mv_p, int mv_l, int mv_t) {
		PNODE pNode = head_product->nxt_product;	// set pNode to header node
		int check = 0;		// check flag

		while (pNode != nullptr) {	// check all node
			if (strcmp(pNode->location, loc) == 0) {	// same location founc
				check = 1;	// flag on
				break;
			}
			pNode = pNode->nxt_location;	// goto next location
		}
		if (check == 0)		// if there's no 'tim' in cube
			return 0;

		int p = 0, l = 0, t = 0;	// position variable

		bool f = 0;		// first 

		PNODE pProd = head_product->nxt_product;		// set pProd variable
		while (pProd != nullptr) {		// check all product
			PNODE pLoc = pProd;			// set pLoc

			while (pLoc != nullptr) {	// check all location
				if (strcmp(pLoc->location, loc) == 0) {	// if same location found
					PNODE pPrev = pLoc->prv_location;	// set pPrev
					if (pPrev == nullptr) {				// if pPrev is null
						f = 1;
					}
					PNODE pCur = new NODE;				// current node
					PNODE pNext = pLoc->nxt_location;	// next node

					for (int i = 0; i < n; i++) {		// make all lv3 node
						PNODE pTime = pLoc;				// set pTime node
						int pos = 0;					// posiion variable
						while (pTime != nullptr) {		// check all time
							int c = rc->get_count(p, mv_p, l, 1, t, mv_t);	// get the count
							char* nam = new char[100];						// get the data
							nam = tr->get_lv3(pLoc->location, i);
							
							PNODE newNode = new NODE;						// set new node
							newNode->time = pTime->time;
							newNode->location = nam;
							newNode->product = pTime->product;

							if (pos == 0) {
								if (f == 1) {	// first input
									// linke newnode and header
									head_location->nxt_location = newNode;
									head_product->nxt_product = newNode;
									head_time->nxt_time = newNode;
									pPrev = newNode;
									pCur = newNode;
									pos++;	// position set
								}
								else {
									// linke with newnode and pPrev
									pPrev->nxt_location = newNode;
									newNode->prv_location = pPrev;
									pPrev = newNode;
									pCur = newNode;
									pos++;
								}
							}
							else {
								// link new node and pcur
								pCur->nxt_time = newNode;
								newNode->prv_time = pCur;
								pCur = newNode;
							}

							pTime = pTime->nxt_time;	// goto next time
							t += mv_t;	// set position
						}
						pos = 0;		// reset
						t = 0;			// reset
						l++;			// move location
					}
					pPrev->nxt_location = pNext;	// linke pPrev and pNext
					pNext->prv_location = pPrev;
				}
				t = 0;					// reset
				pLoc = pLoc->nxt_location;	// next location
				l += mv_l;				// move
				f = 0;					// reset
			}
			l = 0;	// reset
			t = 0;	// reset
			pProd = pProd->nxt_product;	// get next product
			p += mv_p;					// move
		}
		return 1;
	}

	// DRILLDOWN product
	bool DD_product(char* prod, raw_cube* rc, Tree* tr, int n, int mv_p, int mv_l, int mv_t) {
		PNODE pNode = head_product->nxt_product;	// pNode to head node
		int check = 0;		// check flag

		while (pNode != nullptr) {			// check all node
			if (strcmp(pNode->product, prod) == 0) {	// smae product found
				check = 1;		// flag on
				break;
			}
			pNode = pNode->nxt_product;	// goto next product
		}
		if (check == 0)		// if there's no 'tim' in cube
			return 0;

		int p = 0, l = 0, t = 0;	// set position
		int f = 0;	// first time
		PNODE pProd = head_product->nxt_product;	// set pProd
		while (pProd != nullptr) {	// check all product
			if (strcmp(pProd->product, prod) == 0) {	// same product found
				PNODE pPrev = pProd->prv_product;		// set pPrev
				PNODE pNext = pProd->nxt_product;		// set pNext
				PNODE pCur = new NODE;					// make current node
				for (int i = 0; i < n; i++) {			// make product's child node
					PNODE pLoc = pProd;					// set pLoc
					int pos = 0;						// make 'pos' variable to check
					while (pLoc != nullptr) {			// check all node
						PNODE pTime = pLoc;				// set pTIme

						while (pTime != nullptr) {		// check all node
							int c = rc->get_count(p, 1, l, mv_l, t, mv_t);	// get count
							char* nam = new char[100];		// get name of lv3
							nam = tr->get_lv3(pProd->product, i);

							PNODE newNode = new NODE;		// make new node and set
							newNode->time = pTime->time;
							newNode->location = pTime->location;
							newNode->product = nam;
							newNode->sum = c;


							pTime = pTime->nxt_time;	// goto next time
							t += mv_t;		// move time
						}

						t = 0;				// reset
						pLoc = pLoc->nxt_location;		// goto next location
						l += mv_l;			// move location
						f = 0;				// reset
					}
					l = 0;					// reset
					p++;					// move position
				}
			}
			
			l = 0;		// reset
			t = 0;		// reset
			pProd = pProd->nxt_product;	// goto next product
			p += mv_p;	// move position
		}
		return 1;
	}
};


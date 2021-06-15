#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include "Tree.h"
#include "raw_cube.h"
#include "view_cube.h"
#define MAX 100
using namespace std;

int product_sum[MAX];
int location_sum[MAX];
int time_sum[MAX];

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	
	Tree* product_tree = new Tree;			// product Tree
	Tree* location_tree = new Tree;			// location Tree
	Tree* time_tree = new Tree;				// time Tree

	raw_cube* RC = new raw_cube;			// RAW CUBE

	view_cube* VC = new view_cube;			// VIEW CUBE

	ifstream file;
	file.open("command.txt", ios::in);		// read command file
	
	ofstream log;
	log.open("log.txt", ios::out, ios::trunc);

	/* use these array when making raw_cube */
	// save products in array (level 3)
	char* product_arr[MAX]; int product_idx[MAX]; int product_num = 0;
	for (int i = 0; i < MAX; i++) product_arr[i] = new char[MAX];
	int product_size = 0;

	// save locations in array (level 3)
	char* location_arr[MAX]; int location_idx[MAX]; int location_num = 0;
	for (int i = 0; i < MAX; i++) location_arr[i] = new char[MAX];
	int location_size = 0;

	// save times in array (level 3)
	char* time_arr[MAX]; int time_idx[MAX]; int time_num = 0;
	for (int i = 0; i < MAX; i++) time_arr[i] = new char[MAX];
	int time_size = 0;

	// save level2
	char* product_lv2[MAX]; for (int i = 0; i < MAX; i++) product_lv2[i] = new char[MAX];
	char* location_lv2[MAX]; for (int i = 0; i < MAX; i++) location_lv2[i] = new char[MAX];
	char* time_lv2[MAX]; for (int i = 0; i < MAX; i++) time_lv2[i] = new char[MAX];


	if (file.is_open()) {			// read command text file
		while (!file.eof()) {
			// get file's line
			char* line = new char[256];
			file.getline(line, 256);

			// get command from 'line'
			char* cmd = new char[256];
			cmd = strtok(line, " ");
			//cout << cmd << '\n';

			if (strcmp(cmd, "EXIT") != 0)
				log << "[" << cmd << "]" << '\n';

			// LOAD
			if (strcmp(cmd, "LOAD") == 0) {
				// make product tree
				ifstream product;
				product.open("product.txt", ios::in);		// read product.txt

				if (product.is_open()) {
					char* p_line = new char[256];
					product.getline(p_line, 256);			// get one line

					char* tok1 = strtok(p_line, " ");		// get data from line
					product_tree->push_head(tok1);	// get head (lv1)
					while (tok1 != NULL) {			// get lv2
						tok1 = strtok(NULL, " ");	// get next data from the line
						if (tok1 == NULL)
							continue;
						product_tree->push_lv2(tok1);	// push to lv2
					}

					while (!product.eof()) {		// get lv3
						char* p_line = new char[256];
						product.getline(p_line, 256);		// get next line
						char* tok1 = strtok(p_line, " ");	// get word from line(lv2)
						char* cmp = new char[256];			// set compare word(lv2)
						cmp = tok1;

						int product_count = 0;

						while (tok1 != NULL) {				// get next word
							tok1 = strtok(NULL, " ");

							if (tok1 == NULL)
								continue;

							product_tree->push_lv3(cmp, tok1);	// push to lv3
							product_arr[product_size] = tok1;	// push to array
							product_size++;

							product_count++;
						}
						product_idx[product_num] = product_count;	// push count up to idx array
						product_num++;
					}
				}
				product.close();
				
				// make location tree
				ifstream location;
				location.open("location.txt", ios::in);		// read location.txt

				if (location.is_open()) {
					char* l_line = new char[256];	
					location.getline(l_line, 256);			// get line from text file

					char* tok2 = strtok(l_line, " ");		// get word from line(head)
					location_tree->push_head(tok2);			// push head
					while (tok2 != NULL) {					// get next word (lv2)
						tok2 = strtok(NULL, " ");
						if (tok2 == NULL)
							continue;
						location_tree->push_lv2(tok2);		// push to lv2
					}

					while (!location.eof()) {
						char* l_line = new char[256];
						location.getline(l_line, 256);		// get next line
						char* tok2 = strtok(l_line, " ");	// get word from line(lv2)
						char* cmp = new char[256];			// cmp = lv2
						cmp = tok2;

						int location_count = 0;

						while (tok2 != NULL) {
							tok2 = strtok(NULL, " ");		// get next word

							if (tok2 == NULL)
								continue;

							location_tree->push_lv3(cmp, tok2);		// push to lv3
							location_arr[location_size] = tok2;		// set array
							location_size++;

							location_count++;
						}
						location_idx[location_num] = location_count;	// set idx array
						location_num++;
					}
				}
				location.close();

				// make time tree
				ifstream time;
				time.open("time.txt", ios::in);		// read time.txt

				if (time.is_open()) {
					char* t_line = new char[256];
					time.getline(t_line, 256);		// get next line

					char* tok3 = strtok(t_line, " ");	// get word from line(head)
					time_tree->push_head(tok3);			// set head of tree
					while (tok3 != NULL) {
						tok3 = strtok(NULL, " ");		// get next word(lv2)
						if (tok3 == NULL)
							continue;
						time_tree->push_lv2(tok3);		// set lv2 of tree
					}

					while (!time.eof()) {
						char* t_line = new char[256];
						time.getline(t_line, 256);			// get line
						char* tok3 = strtok(t_line, " ");	// get word from line
						char* cmp = new char[256];			// cmp = lv2
						cmp = tok3;

						int time_count = 0;

						while (tok3 != NULL) {				// get next word
							tok3 = strtok(NULL, " ");

							if (tok3 == NULL)
								continue;

							time_tree->push_lv3(cmp, tok3);	// push to lv3 in tree
							time_arr[time_size] = tok3;		// set array
							time_size++;

							time_count++;					// count up
						}
						time_idx[time_num] = time_count;	// set idx array
						time_num++;
					}
				}
				time.close();
				
				// make raw_cube
				for (int i = 0; i < product_size; i++)
					for (int j = 0; j < location_size; j++)
						for (int k = 0; k < time_size; k++)
							RC->set(product_arr[i], location_arr[j], time_arr[k]);
				
				ifstream sale;
				sale.open("sales.txt", ios::in);		// get sales.txt file
				if (sale.is_open()) {
					while (!sale.eof()) {				// check all line in sales.txt
						char* s_line = new char[256];
						sale.getline(s_line, 256);		// get line

						char* tok = strtok(s_line, " ");// get product from sales.txt
						char* sale_product = tok;

						tok = strtok(NULL, " ");		// get location from sales.txt
						char* sale_location = tok;
						
						tok = strtok(NULL, " ");		// get time from sales.txt
						char* sale_time = tok;

						tok = strtok(NULL, " ");		// get count from sales.txt
						int cnt_t;
						if (tok == NULL)
							cnt_t = 0;
						else cnt_t = atoi(tok);

						//cout << sale_product << " " << sale_location << " " << sale_time << " " << cnt_t << '\n';
						RC->push(sale_product, sale_location, sale_time, cnt_t);	// push count in raw cube
					}
				}
				sale.close();

				/* get from tree and push it to array for easy use */
				// make view cube
				for (int i = 0; i < product_num; i++) {			// set lv2 array in product
					product_lv2[i] = product_tree->get_lv2(i);
				}
				for (int i = 0; i < location_num; i++) {		// set lv2 array in location
					location_lv2[i] = location_tree->get_lv2(i);
				}
				for (int i = 0; i < time_num; i++) {			// set lv2 array in time
					time_lv2[i] = time_tree->get_lv2(i);
				}

				// set view cube
				for (int i = 0; i < product_num; i++)
					for (int j = 0; j < location_num; j++)
						for (int k = 0; k < time_num; k++)
							VC->set(product_lv2[i], location_lv2[j], time_lv2[k]);

				int mv_p = product_idx[0];		// number of product(lv3)
				int mv_l = location_idx[0];		// number of location(lv3)
				int mv_t = time_idx[0];			// number of time(lv3)
				int p = 0, l = 0, t = 0;		// position of product, location, time
				int sum = 0;					// sum of sales

				for (int i = 0; i < product_num; i++) {
					for (int j = 0; j < location_num; j++) {
						for (int k = 0; k < time_num; k++) {
							// get the sum
							sum += RC->get_count(p, mv_p, l, mv_l, t, mv_t);				// get sum and push it to view cube
							VC->push(sum, product_lv2[i], location_lv2[j], time_lv2[k]);	// push to view cube
							t += mv_t;		// move the time
						}
						sum = 0;			// reset the sum
						l += mv_l;			// move the location
						t = 0;				// reset the time position
					}
					l = 0;					// reset the position
					p += mv_p;				// move the product
				}

				VC->cube_print(log);		// print cube in log.txt file
			}	// LOAD End

			// VIEW
			else if (strcmp(cmd, "VIEW") == 0) {
				VC->clear();		// clear the view cube

				// set view cube again
				for (int i = 0; i < product_num; i++)
					for (int j = 0; j < location_num; j++)
						for (int k = 0; k < time_num; k++)
							VC->set(product_lv2[i], location_lv2[j], time_lv2[k]);
				
				int mv_p = product_idx[0];		// number of product(lv3)
				int mv_l = location_idx[0];		// number of location(lv3)
				int mv_t = time_idx[0];			// number of time(lv3)
				int p = 0, l = 0, t = 0;		// position of product, location, time
				int sum = 0;					// sum of sales

				for (int i = 0; i < product_num; i++) {
					for (int j = 0; j < location_num; j++) {
						for (int k = 0; k < time_num; k++) {
							// get the sum
							sum += RC->get_count(p, mv_p, l, mv_l, t, mv_t);				// get sum and push it to view cube
							VC->push(sum, product_lv2[i], location_lv2[j], time_lv2[k]);	// push to view cube
							t += mv_t;		// move the time
						}
						sum = 0;			// reset the sum
						l += mv_l;			// move the location
						t = 0;				// reset the time position
					}
					l = 0;					// reset the position
					p += mv_p;				// move the product
				}

				VC->cube_print(log);		// print cube in log.txt file
			}	// VIEW End

			// ROTATE
			else if (strcmp(cmd, "ROTATE") == 0) {
				log << "ERROR" << '\n';
			}

			// ROLLUP
			else if (strcmp(cmd, "ROLLUP") == 0) {
				log << "ERROR" << '\n';
			}

			// DRILLDOWN
			else if (strcmp(cmd, "DRILLDOWN") == 0) {
				char* par = new char[MAX];
				par = strtok(NULL, " ");

				int mv_p = product_idx[0];		// product idx 
				int mv_l = location_idx[0];		// location idx
				int mv_t = time_idx[0];			// time idx
				int p = 0, l = 0, t = 0;		// position variable

				if (time_tree->get_child_num(par) != -1) {	// check if 'par' is in the tree
					int n = time_tree->get_child_num(par);		// get number of child
					VC->DD_time(par, RC, time_tree, n, mv_p, mv_l, mv_t);	// drilldown
					VC->cube_print(log);	// print in log
				}

				else if (location_tree->get_child_num(par) != -1) {	// check if 'par' is in the tree
					int n = location_tree->get_child_num(par);		// get number of child
					VC->DD_location(par, RC, location_tree, n, mv_p, mv_l, mv_t);	// drilldown
					VC->cube_print(log);	// print in log
				}

				else if (product_tree->get_child_num(par) != -1) {	// check if 'par' is in the tree
					int n = product_tree->get_child_num(par);		// get number of child
					VC->DD_product(par, RC, product_tree, n, mv_p, mv_l, mv_t);	// drilldown
					VC->cube_print(log);	// print in log
				}

				else	// not found
					log << "ERROR" << '\n';
			}

			// SLICE
			else if (strcmp(cmd, "SLICE") == 0) {
				char* par = new char[MAX];		// get parameter
				par = strtok(NULL, " ");

				if (VC->find_time(par)) {		// find in time
					VC->cube_print(log);		// print cube
				}

				else if (VC->find_loc(par)) {			// find in location
					VC->cube_print(log);		// print cube
				}

				else if (VC->find_prod(par)) {		// find in product
					VC->cube_print(log);		// print cube
				}

				else
					log << "ERROR" << '\n';
			}

			// EXIT
			else if (strcmp(cmd, "EXIT") == 0) {
				break;
			}

			log << '\n';
		}
		log.close();		// close log file
		file.close();		// close command file
	}
	else					// file not open
		cout << "file open error" << '\n';

	return 0;
}
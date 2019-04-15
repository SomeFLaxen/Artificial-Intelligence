#include<iostream>
#include<stdio.h>
#include<cmath> 
#include<fstream>
using namespace std;
ifstream infile("input.txt", ios::in);
ofstream outfile("output.txt", ios::out);

int open_cnt = 0;
int open_node_cnt;//open��ڵ���� 

struct Node {
	int a[3][3];
	int x, y;
	int f, g, h;//f=g+h
	int flag; //��һ���ƶ����� 
	Node* father;
}start, End;

struct Open_Close {
	int f;
	Node* np;
}open[10000], close[10000];

bool isable() {//�ж��Ƿ��н�:������֮����ż����ͬ���н�
	int s[9], e[9];
	int tf = 0, ef = 0;
	int k = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			s[k] = start.a[i][j];
			e[k] = End.a[i][j];
			k++;
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < i; j++) {
			if (s[i] > s[j] && s[j] != 0) tf += 1;
			if (e[i] > e[j] && e[j] != 0) ef += 1;
		}
	}
	if ((tf % 2 == 1 && ef % 2 == 1) || (tf % 2 == 0 && ef % 2 == 0)) return true;
	else return false;
}

int a_start_h(Node * node) {  //�� h
	int old_x = 0, old_y = 0, End_x = 0, End_y = 0;
	int h = 0;
	for (int k = 1; k < 9; k++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (node->a[i][j] == k) {  //��Ӧ��ʼ����±�
					old_x = i;
					old_y = j;
				}
				if (End.a[i][j] == k) {   //��ӦĿ��Ľ���±�
					End_x = i;
					End_y = j;
				}
			}
		}
		h += abs(old_x - End_x) + abs(old_y - End_y); //����h
	}
	return h;
}

void input() {               //���� 
	//cout << "=====������ʼͼ=====" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			infile >> start.a[i][j];
			if (start.a[i][j] == 0) {
				start.x = i;
				start.y = j;
			}
		}
	}
	outfile<< endl;
	//cout << "=====����Ŀ��ͼ=====" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			infile >> End.a[i][j];
			if (End.a[i][j] == 0) {
				End.x = i;
				End.y = j;
			}
		}
	}
	outfile << endl;
	start.g = 0;
	start.h = a_start_h(&start);
	start.f = start.g + start.h;
}

int show(Node * node) {               //��ʾ 
	Node* p = node;
	if (p == &start) return 1;
	else show(p->father);
	outfile << "==============\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			outfile << p->a[i][j] << " ";
		}
		outfile << endl;
	}
	outfile << "==============\n\n";
}


bool isend(Node * node) {         //�ж��Ƿ�ΪĿ��ڵ� 
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (node->a[i][j] != End.a[i][j])
				return false;
		}
	}
	return true;
}


void sort(Open_Close * open) {      //open������ 
	int min = 99999, min_flag = 0;
	Open_Close temp;
	for (int i = 0; i <= open_cnt; i++) {
		if (min > open[i].f && open[i].f > 0) {
			min = open[i].f;
			min_flag = i;
		}
	}
	temp = open[min_flag];
	open[min_flag] = open[0];
	open[0] = temp;
}



void move(int flag, Node * node) {   //���ĸ�������չ 
	int temp;
	if (flag == 1 && node->x > 0) {     //turn left
		Node* n = new Node();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x - 1][node->y];
		n->a[node->x - 1][node->y] = 0;
		n->x = node->x - 1;
		n->y = node->y;
		n->flag = 3;
		n->father = node;
		n->g = node->g + 1;             //  �� g���� 
		n->h = a_start_h(n);
		n->f = n->g + n->h;  //  �� f���� 
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //��ӵ�open��
		open[open_cnt].f = n->f;
	}

	else if (flag == 2 && node->y < 2) {     //go up
		Node* n = new Node();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x][node->y + 1];
		n->a[node->x][node->y + 1] = 0;
		n->x = node->x;
		n->y = node->y + 1;
		n->flag = 4;
		n->father = node;
		n->g = node->g + 1;             //  �� g���� 
		n->h = a_start_h(n);
		n->f = n->g + n->h;            //  �� f����
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //��ӵ�open��
		open[open_cnt].f = n->f;
	}
	else if (flag == 3 && node->x < 2) {    //turn right
		Node* n = new Node();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x + 1][node->y];
		n->a[node->x + 1][node->y] = 0;
		n->x = node->x + 1;
		n->y = node->y;
		n->flag = 1;
		n->father = node;
		n->g = node->g + 1;             //  �� g���� 
		n->h = a_start_h(n);
		n->f = n->g + n->h;//  �� f����
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //��ӵ�open��
		open[open_cnt].f = n->f;
	}
	else if (flag == 4 && node->y > 0) {    //go down
		Node* n = new Node();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x][node->y - 1];
		n->a[node->x][node->y - 1] = 0;
		n->x = node->x;
		n->y = node->y - 1;
		n->flag = 2;
		n->father = node;
		n->g = node->g + 1;             //  �� g���� 
		n->h = a_start_h(n);
		n->f = n->g + n->h;//  �� f���� 
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //��ӵ�open��
		open[open_cnt].f = n->f;
	}
}
void expand(Node * node) {    //�ڵ���չ    
	for (int i = 1; i < 5; i++) {
		if (i != node->flag) move(i, node);
	}
}


int main() {
	input();
	infile.close();
	open[0].np = &start;//start����open��			
	open_node_cnt = 1;
	if (isable()) {
		while (true) {//open��Ϊ�� 
			if (isend(open[0].np)) {
				outfile << "\n·����\n";
				show(open[0].np);
				outfile << open[0].np->g << endl;
				break;
			}
			expand(open[0].np);//��չ���Žڵ���ӽڵ� 
			open[0].np = NULL;
			open[0].f = -1;
			open_node_cnt--;
			sort(open);   //open������
		}
	}
	else outfile << "�޽�" << endl;
	outfile.close();
	system("pause");
	return(0);
}

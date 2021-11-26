#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

typedef struct Node {
	int depth;        //represent the depth of node
	int State[3][3];
	int F, G, H;
	int zeroLoc_x,zeroLoc_y;  //0's location
	int Closed;  // 1 represents for it has been put into close list,0 represents for it didn't.
	Node* father;
};

int IDnum = 0;
int deep = 0;//represents the depth of nodes

vector<Node > Open_list;
vector<Node > Closed_list;

int Target[3][3];
int Action[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };


//heuristic function values computation
void Heuristic(int Target[][3], int State[][3], Node* node) {
	int H = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (Target[i][j] == State[i][j]) {
				continue;
			}
			if (Target[i][j] == 0) {
				continue;
			}
			else {
				H++;
			}
		}
	}
	node->G = node->depth;          //G equals the depth
	node->H = H;
	node->F = node->G + node->H;
}

//give state array to the node
void GiveState(Node* node, int State[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			node->State[i][j] = State[i][j];
			if (State[i][j] == 0) {
				node->zeroLoc_x = i;
				node->zeroLoc_y = j;
			}
		}
	}
}


//Create the new node 
Node CreateNode(Node* fatherNode, int k[][3]) {
	//int k[3][3] = { {1,2,3},{8,0,5},{7,4,6} };
	Node node;
	node.father = fatherNode;
	node.depth = fatherNode->depth + 1;
	GiveState(&node, k);
	Heuristic(Target, node.State, &node);
	return node;
}


//Judge if the node get the final answer
int GetTarget(int Target[][3], int State[][3]) {
	int H = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (Target[i][j] == State[i][j]) {
				continue;
			}
			else {
				H++;
			}
		}
	}
	if (H == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

//For sorted the Open_list
bool cmp(Node nodea, Node nodeb) {
	
		return nodea.F > nodeb.F;
	
}

//Create new gragh based on action, add them
Node CreateSubNode(Node* fatherNode, int action[]) {
	//To see if this location can work
	int k[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			k[i][j] = fatherNode->State[i][j];
		}
	}
	int number = k[fatherNode->zeroLoc_x + action[0]][fatherNode->zeroLoc_y + action[1]];
	k[fatherNode->zeroLoc_x + action[0]][fatherNode->zeroLoc_y + action[1]] = 0;
	k[fatherNode->zeroLoc_x][fatherNode->zeroLoc_y] = number;
	Node subnode = CreateNode(fatherNode, k);
	return subnode;
}

//Make sure that 0's location never out of range,or has been visited
int Judgement(Node* node, int action[]) {
	if (node->zeroLoc_x + action[0] > 2 || node->zeroLoc_x + action[0] < 0) {
		return 0;
	}
	if (node->zeroLoc_y + action[1] > 2 || node->zeroLoc_y + action[1] < 0) {
		return 0;
	}
	return 1;
}


//find same state in openlist
int FindinOpenlist (int Target[][3]) {
	for (int k = 0; k < Open_list.size(); k++) {
		int H = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (Target[i][j] == Open_list[k].State[i][j]) {
					continue;
				}
				else {
					H++;
				}
			}
		}
		if (H == 0) {
			return k;
		}
	}
	return -1;
}


void Astar() {
	int i = 0;
	while (Open_list.size() != 0)
	{	

		Node current = Open_list[Open_list.size() - 1];
		Open_list.pop_back();
		current.Closed = 1;
		Closed_list.push_back(current);

		if (i != 0) {
			cout << "Step " << i <<  endl;
			cout << "----------" << endl;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << current.State[i][j] << " ";
				}
				cout << endl;
			}
			cout << "----------" << endl;
		}
		if (GetTarget(Target, current.State) == 1) {
			std::cout << "Find the Solution! Need " <<i<<" steps." << endl;
			//Visualize_Solution(&current);
			break;
		}

		for (int i = 0; i < 4; i++) {
			int action[2];
			for (int j = 0; j < 2; j++) {
				action[j] = Action[i][j];
			}
			if (Judgement(&current, action) == 0) {
				continue;
			}
			if (Judgement(&current, action) == 1) {
				Node subNode = CreateSubNode(&current, action);
				if (FindinOpenlist(subNode.State) != -1) {
					int loc = FindinOpenlist(subNode.State);
					if (subNode.G  < Open_list[loc].G) {
						Open_list[loc].father = &current;
						Open_list[loc].G = subNode.G;
						Open_list[loc].F = subNode.G + Open_list[loc].H;
					}
				}
				
				else {
					subNode.father = &current;
					Open_list.push_back(subNode);
				}
			}
		}
		
		sort(Open_list.begin(), Open_list.end(), cmp);
		i++;
		
	}

}


int main() {
	Node startNode;
	std::cout << "input start matrix£º"<<endl;
	//input the initial data
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> startNode.State[i][j];
			if (startNode.State[i][j] == 0) {
				startNode.zeroLoc_x = i;
				startNode.zeroLoc_y = j;
			}
		}
	}
	std::cout << endl;
	std::cout << "input target matrix:" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> Target[i][j];

		}
	}
	std::cout << endl;
	//initialize startnode
	startNode.depth = deep;
	startNode.G = 0;
	startNode.father = NULL;
	Heuristic(Target, startNode.State, &startNode);
	Open_list.push_back(startNode);
	Astar();
	return 0;
}


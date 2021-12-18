#include "game_solver.h"
#include "draw.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void read_file(int& mm, int& nn, string& temp){
	mm=0;
	nn=0;
	temp.clear();
	ifstream file_read;
	file_read.open("box.txt",ios::in);

	if (!file_read) {
		cout << "box.txt dose not exist!" << endl;
#ifdef _WIN32
		system("pause");
#endif
		exit(100);
	}

	string x;
	string tempr;
	while (getline(file_read,x)) {
		tempr += x;
		mm += 1;
	}
	for(int i=0;i<tempr.size();i++){
		char tc=tempr[i];
		if(tc!='\r'&&tc!='\n'){
			temp.push_back(tc);
		}
	}
	
	nn = temp.size() / mm;
	file_read.close();
}

int main() {
	int mm;
	int nn;
	string temp;
	read_file(mm,nn,temp);
	
	game_solver ga(temp,mm,nn);
	
	auto ss=ga.Astar_solver();

	cout << "press Enter to show solves"<<endl;

	cin.get();
	draw_picture d;
	d.draw(ss);
	
	
#ifdef _WIN32
		system("pause");
#endif
}


//7507ms
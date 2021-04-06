#include "game_solver.h"
#include "draw.h"
#include<fstream>
#include<string>


int main() {

	ifstream file_read;
	int mm=0,nn=0;
	file_read.open("box.txt",ios::in);

	if (!file_read) {
		cout << "box.txt dose not exist!" << endl;
		system("pause");
		return 100;
	}

	string x;
	string temp;
	while (getline(file_read,x)) {
		temp += x;
		mm += 1;
	}
	nn = temp.size() / mm;
	file_read.close();
	
	game_solver ga(temp,mm,nn);
	
	auto ss=ga.Astar_solver();

	cout << "按回车键展示解法,w查看下一步,s查看上一步" << endl;

	cin.get();
	draw_picture d;
	d.draw(ss);

	system("pause");
}


//7507ms
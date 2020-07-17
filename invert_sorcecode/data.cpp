#include <fstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;
int main(){
	double t;
	ofstream ofile;
	ofile.open("data.txt");
	ofile<<1200<<" "<<1200<<endl;
	for(int i = 0;i < 1200;i++){
		for(int j = 0; j < 1200; j++){
			t = rand()%1000;
			ofile<<t<<" ";
		}	
		ofile<<endl;
	}
}

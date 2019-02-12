#include"Car.h"

Car1::Car1() {
	tier = new int[3];//this is to keep note of what tier it is at
	for (int i = 0; i < 3; i++) {
		tier[i] = 1;//use load progress function
	}
}
Car1::~Car1() {
	//saves progess in txt file
	delete[] tier;
}
void Car1::Upgrade(int number) {
	
}
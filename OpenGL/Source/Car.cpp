#include"Car.h"

Car::Car() {
	tier = new int[3];//this is to keep note of what tier it is at
	for (int i = 0; i < 3; i++) {
		tier[i] = 1;//use load progress function
	}
}
Car::~Car() {
	//saves progess in txt file
	delete[] tier;
}
Car1::Car1() {

}
Car1::~Car1() {

}
void Car1::Upgrade(int Obj) {
	tier[Obj]++;
}
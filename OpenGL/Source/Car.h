#ifndef CAR_H
#define CAR_H
class Car {
public:
	virtual void upgrade() = 0;
protected:
	//physics
	int Nitro;//Power of nitro
	float Control;//conering and handling ability
	float Acceleration;
	int topSpeed;
	float Braking;

	int* tier;
};
class Car1 : Car {//Car1 is an example
public:
	Car1();
	~Car1();
	void Upgrade(int);
private:

};
//requires a way to change teirs from shop..
#endif
//In Main
//Car* car;
//car=new car1;
//loads car info
//saves car info;
//delete car;
//car=new car2;
//loads car info
//saves car info;
//delete car;
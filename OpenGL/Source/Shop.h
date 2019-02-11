#ifndef SHOP_H
#define SHOP_H

class shop {
public:
	shop();
	~shop();
	double Buy(int);
	void Sold(double);
private:
	int itemNo;
	int teirNo[4];
};
#endif
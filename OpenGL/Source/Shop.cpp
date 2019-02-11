#include "Shop.h"
//#include "Player.h"
shop::shop() {
	teirNo[0] = 1;
	teirNo[1] = 1;
	teirNo[2] = 1;
	teirNo[3] = 1;
}
shop::~shop() {

}
double shop::Buy(int number) {//resets DeltaT so that 'sold' shows for a few seconds
	switch (number) {
	case 1:	if (/*player.getMoney() >=*/ (teirNo[0] * 20)) {
		++teirNo[0];
		itemNo = 1;
		//player.setMoney(player.getMoney() - (teirNo[0] * 20));
	}
			else {
		itemNo = 0;
	}
	case 2:if (/*player.getMoney() >=*/ (teirNo[1] * 15)) {
		++teirNo[1];
		itemNo = 2;
		//player.setMoney(player.getMoney() - (teirNo[1] * 15));
	}
		   else {
		itemNo = 0;
	}
	case 3:if (/*player.getMoney() >=*/ (teirNo[2] * 10)) {
		++teirNo[2];
		itemNo = 3;
		//player.setMoney(player.getMoney() - (teirNo[2] * 10));
	}
		   else {
		itemNo = 0;
	}
	case 4:if (/*player.getMoney() >=*/ (teirNo[3] * 30)) {
		++teirNo[3];
		itemNo = 4;
		//player.setMoney(player.getMoney() - (teirNo[3] * 30));
	}
		   else {
		itemNo = 0;
	}
	}
	return 0;
};
void shop::Sold(double DeltaT) {
	if (DeltaT < 10) {
		if (!itemNo) {
			//RenderTextOnScreen(meshList[GEO_TEXT], "Not Enough", color(0, 1, 0), 2, 2, 2);
		}
		else {
			//RenderTextOnScreen(meshList[GEO_TEXT], "Item Bought", color(0, 1, 0), 2, 2, 2);
		}
	}
}
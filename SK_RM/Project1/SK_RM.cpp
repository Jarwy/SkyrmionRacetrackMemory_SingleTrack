#include "SK_RM.h"
#include<numeric>
SK_RM::SK_RM(int x, int y) {
	bits = 0;
	volumn = x;
	NofHead = y;
	interval = volumn / NofHead;
	bufferSize = interval - 1;
	totalCapacity = volumn + bufferSize;
	RM.resize(totalCapacity);

	headTable.resize(NofHead);							//headTable用來存放讀寫頭們所在位置	
	int headIndex = bufferSize;						//預設第一服務區尾部
	for (int i = 0; i < NofHead; i++) {					//以第一支讀寫頭為準，算出剩下的讀寫頭位置並存入vector
		headTable[i] = headIndex;
		headIndex = headIndex + interval;			//讀寫頭會在每個服務區尾部
	}
	/*印出所有讀寫頭位置(偵錯用)
	cout << "Head indexes: ";
	for (int i = 0; i < y; i++) {
		cout <<headTable[i] << " "; 
	}cout<< endl;
	*/
}

void SK_RM::PrintRM() {
	cout << "SK-RM:";
	for (int i = 0; i < volumn; i++) {
		cout << RM[i] << " ";
	}
	cout << endl;
}

//void SK_RM::PrintRM() {
//	for (vector<int>::iterator it = RM.begin(); it!= RM.end(); it++) {
//		cout << *it << " ";
//	}
//	cout << endl;
//}

void SK_RM::LeftShift() {	//assume only one side has spare space 
	RM.erase(RM.begin());
	RM.resize(totalCapacity);
}

void SK_RM::RightShift() {
	RM.insert(RM.begin(), 0);
	RM.resize(totalCapacity);
}

void SK_RM::DeleteL(int index) {
	RM.erase(RM.begin() + index);
	RM.insert(RM.begin(), 0);
	//RM.assign(RM.begin()+start, RM.end());
	RM.resize(totalCapacity);
}

void SK_RM::DeleteR(int index) {
	RM.erase(RM.begin() + index);
	RM.insert(RM.end(), 0);
	//RM.resize(totalCapacity);
}

void SK_RM::InsertL(int index, int value) {
	RM.erase(RM.begin());
	RM.insert(RM.begin() + index, value);
	//RM.resize(totalCapacity);
}

void SK_RM::InsertR(int index, int value) {
	RM.insert(RM.begin() + index, value);
	RM.resize(totalCapacity);
	RM.swap(RM);
}

void SK_RM::Update(int index, int value) {
	RM[index] = value;
}

void SK_RM::Write(int value) {
	int newValue = 0;
	bits = 0;
	while (value != newValue && !(value==0 && newValue==1)) {
		bits++;		//紀錄寫入bit數
		//cout << value % 2 << " ";			//Used to check if the converter correct
		SK_RM::InsertL(headTable[headTable.size()-1], value % 2);
		newValue = value;
		value = value / 2;
	}
	//cout << endl;
	for (int i = 0; i < volumn - bits; i++) {
		LeftShift();
	}
	//將寫入的資料左移到vector底部
	//若空間為8bit，只寫入{[ ][ ][0][1][0][1][0][1]}，則將資料左移到底，不足補零(左移)
}

void SK_RM::Read() {
	int i = 0;
	int sum = 0;
	vector<int> headValue;									//Used to preserve the value of each head.
	headValue.resize(headTable.size());						//Each head has its own value.
	for (i = 0; i <bufferSize; i++) {						//用for迴圈讓讀寫頭讀完自己負責的範圍
		for (int j = headValue.size()-1; j >= 0 ; j--) {		//用for迴圈將每個讀寫頭輪過一輪
			headValue[j] = headValue[j] + pow(2, headTable[j] - i)*RM[headTable[j]];
			//讀寫頭[j]累計值 = 讀寫頭[j]累計值 + 2^(讀寫頭位置-位移量)*讀寫頭值(0或1)
			//以volumn = 8, headNumber = 2為例：
			//       3       7        <= 讀寫頭位置
			//       ↓       ↓	      <= 讀寫頭
			//[][][][][][][][]|[][][] <= 記憶體
			// 0 1 2 3 4 5 6 7		  <= 冪次(讀寫頭位置-位移量)
		}
		RightShift();										//右移
	}
	//讀寫頭在讀完服務範圍中最後一bit後無需再位移，故前面迴圈少做，而讀最後一bit額外做
	for (int j = headValue.size()-1; j >= 0 ; j--) {			//用for迴圈將每個讀寫頭輪過一輪
		headValue[j] = headValue[j] + pow(2, headTable[j] - i)*RM[headTable[j]];
		//讀寫頭[j]累計值 = 讀寫頭[j]累計值 + 2^(讀寫頭位置-位移量)*讀寫頭值(0或1)
	}

	for (i = 0; i <bufferSize; i++) {
		//剛剛右移幾次就要左移幾次回來 避免資料遺失
		LeftShift();
	}

	sum = accumulate(headValue.begin(), headValue.end(), sum);
	//將欲累加的範圍傳進accumulate函式 
	cout << sum << endl;
}

void SK_RM::BitRead(int idx) {
	int headNumber = HeadSelector(idx);
	int step = abs(headTable[headNumber] - idx);
	bool dir = FindLocation(headTable[headNumber], idx, step);
	cout << "Bit_" << idx << ":" << RM[headTable[headNumber]] << endl;
	Shift_Multi(dir, step);//復位
}

void SK_RM::Shift_Multi(bool direction, int step) {			//1=>leftshift, 0=>rightShift
	if (direction) {
		for (int i = 0; i < step; i++) {LeftShift();}
	}
	else {
		for (int i = 0; i < step; i++) {RightShift();}
	}
}

int SK_RM::HeadSelector(int idx) {
	return (idx / interval);
}

bool SK_RM::FindLocation(int headIdx, int bitIdx, int step) {	//選擇移動方向，並回傳復位方向
	if (headIdx > bitIdx) {
		Shift_Multi(false, step);
		return true;
	}
	else {
		Shift_Multi(true, step);
		return false;
	}
}
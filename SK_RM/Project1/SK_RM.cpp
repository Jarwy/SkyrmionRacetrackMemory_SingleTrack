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

	headTable.resize(NofHead);							//headTable�ΨӦs��Ū�g�Y�̩Ҧb��m	
	int headIndex = bufferSize;						//�w�]�Ĥ@�A�Ȱϧ���
	for (int i = 0; i < NofHead; i++) {					//�H�Ĥ@��Ū�g�Y���ǡA��X�ѤU��Ū�g�Y��m�æs�Jvector
		headTable[i] = headIndex;
		headIndex = headIndex + interval;			//Ū�g�Y�|�b�C�ӪA�Ȱϧ���
	}
	/*�L�X�Ҧ�Ū�g�Y��m(������)
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
		bits++;		//�����g�Jbit��
		//cout << value % 2 << " ";			//Used to check if the converter correct
		SK_RM::InsertL(headTable[headTable.size()-1], value % 2);
		newValue = value;
		value = value / 2;
	}
	//cout << endl;
	for (int i = 0; i < volumn - bits; i++) {
		LeftShift();
	}
	//�N�g�J����ƥ�����vector����
	//�Y�Ŷ���8bit�A�u�g�J{[ ][ ][0][1][0][1][0][1]}�A�h�N��ƥ����쩳�A�����ɹs(����)
}

void SK_RM::Read() {
	int i = 0;
	int sum = 0;
	vector<int> headValue;									//Used to preserve the value of each head.
	headValue.resize(headTable.size());						//Each head has its own value.
	for (i = 0; i <bufferSize; i++) {						//��for�j����Ū�g�YŪ���ۤv�t�d���d��
		for (int j = headValue.size()-1; j >= 0 ; j--) {		//��for�j��N�C��Ū�g�Y���L�@��
			headValue[j] = headValue[j] + pow(2, headTable[j] - i)*RM[headTable[j]];
			//Ū�g�Y[j]�֭p�� = Ū�g�Y[j]�֭p�� + 2^(Ū�g�Y��m-�첾�q)*Ū�g�Y��(0��1)
			//�Hvolumn = 8, headNumber = 2���ҡG
			//       3       7        <= Ū�g�Y��m
			//       ��       ��	      <= Ū�g�Y
			//[][][][][][][][]|[][][] <= �O����
			// 0 1 2 3 4 5 6 7		  <= ����(Ū�g�Y��m-�첾�q)
		}
		RightShift();										//�k��
	}
	//Ū�g�Y�bŪ���A�Ƚd�򤤳̫�@bit��L�ݦA�첾�A�G�e���j��ְ��A��Ū�̫�@bit�B�~��
	for (int j = headValue.size()-1; j >= 0 ; j--) {			//��for�j��N�C��Ū�g�Y���L�@��
		headValue[j] = headValue[j] + pow(2, headTable[j] - i)*RM[headTable[j]];
		//Ū�g�Y[j]�֭p�� = Ū�g�Y[j]�֭p�� + 2^(Ū�g�Y��m-�첾�q)*Ū�g�Y��(0��1)
	}

	for (i = 0; i <bufferSize; i++) {
		//���k���X���N�n�����X���^�� �קK��ƿ�
		LeftShift();
	}

	sum = accumulate(headValue.begin(), headValue.end(), sum);
	//�N���֥[���d��Ƕiaccumulate�禡 
	cout << sum << endl;
}

void SK_RM::BitRead(int idx) {
	int headNumber = HeadSelector(idx);
	int step = abs(headTable[headNumber] - idx);
	bool dir = FindLocation(headTable[headNumber], idx, step);
	cout << "Bit_" << idx << ":" << RM[headTable[headNumber]] << endl;
	Shift_Multi(dir, step);//�_��
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

bool SK_RM::FindLocation(int headIdx, int bitIdx, int step) {	//��ܲ��ʤ�V�A�æ^�Ǵ_���V
	if (headIdx > bitIdx) {
		Shift_Multi(false, step);
		return true;
	}
	else {
		Shift_Multi(true, step);
		return false;
	}
}
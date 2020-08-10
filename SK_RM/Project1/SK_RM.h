#include<iostream>
#include<vector>
using namespace std;


class SK_RM {
private:
	int bits;				//��Ƥj�p(bit)
	int volumn;				//�O����e�q(bit)
	int NofHead;			//Ū�g�Y�ƶq
	int bufferSize;			//�첾�e�q(bit)
	int interval;			//Ū�g�Y�u�@�j�p(bit)
	//int totalCapacity;		//�O����e�q+�첾�e�q
	vector<int> headTable;	//Ū�g�Y��m
public:
	int totalCapacity;		//�O����e�q+�첾�e�q
	vector<int> RM;		//<<error:vector���O�˪O>> SOL: �[�W"using namespace std;" 
	SK_RM(int capacity, int head);
	void PrintRM();
	void LeftShift();
	void RightShift();
	void DeleteL(int index);
	void DeleteR(int index);
	void InsertL(int index, int value);
	void InsertR(int index, int value);
	void Update(int index, int value);
	void Write(int value);
	void Read();
	void BitRead(int idx);
	void Shift_Multi(bool direction, int step);
	int HeadSelector(int idx);
	bool FindLocation(int headIdx, int bitIdx, int step);
};

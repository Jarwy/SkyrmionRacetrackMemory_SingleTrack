#include<iostream>
#include<vector>
using namespace std;


class SK_RM {
private:
	int bits;				//資料大小(bit)
	int volumn;				//記憶體容量(bit)
	int NofHead;			//讀寫頭數量
	int bufferSize;			//位移容量(bit)
	int interval;			//讀寫頭工作大小(bit)
	//int totalCapacity;		//記憶體容量+位移容量
	vector<int> headTable;	//讀寫頭位置
public:
	int totalCapacity;		//記憶體容量+位移容量
	vector<int> RM;		//<<error:vector不是樣板>> SOL: 加上"using namespace std;" 
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

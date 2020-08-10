#include "SK_RM.h"

int main() {

	SK_RM R1(8, 2);
	//cout << "RM's capacity is " << R1.RM.capacity() << endl;
	//R1.PrintRM();
	//for (int i = 0; i < 8; i++) {
	//	R1.RM[i] = i+1;
	//}
	//R1.PrintRM();
	//R1.Update(2, 100);
	//R1.PrintRM();
	//R1.Write(18);
	//R1.Read();
	//R1.InsertR(2, 100);
	//R1.PrintRM();
	//R1.Read();
	R1.Write(127);
	R1.PrintRM();
	R1.Read();
	R1.Read();
	R1.Write(11);
	R1.PrintRM();
	R1.Read();
	R1.BitRead(0);
	R1.BitRead(1);
	R1.BitRead(2);
	R1.BitRead(3);
	R1.BitRead(4);
	R1.BitRead(5);
	R1.BitRead(6);
	R1.BitRead(7);
	R1.Read();

	system("pause");
	return 0;
}
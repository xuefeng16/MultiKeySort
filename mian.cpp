#include <iostream>
#include <vector>
#include<time.h>
#include "MultikeyData.h"

using namespace std;

void Preparation(vector<unsigned>& priority, unsigned & keynum, unsigned & recnum, int type[]);

int main()
{
	unsigned keynum, recnum;
	int type[2];	//type[0]: LSD or MSD;	type[1]: Radix or Merge
	vector<unsigned> priority;
	vector<unsigned> order;
	clock_t startTime,endTime;	//count the time of sort

	Preparation(priority, keynum, recnum, type);

	MultikeyData::Database data(keynum, recnum);
	data.SetData();
	order = MultikeyData::SetOrder(priority, type[0], keynum);

	startTime = clock();
	
	if(type[0] == 1)	//	1 LSD
		data.LSD_Sort(order, type[1]);
	if(type[0] == 2)	//	2 MSD
		data.MSD_Sort(order, type[1]);

	endTime = clock();
	cout << "Totle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	data.Print(type[1]);
	system("pause");

	return 0;
}

void Preparation(vector<unsigned>& priority, unsigned & keynum, unsigned & recnum, int type[])
{
	cout <<"\t     ==MultikeySort==\t\n\n";
	cout <<"Enter the number of keys:";
	cin >> keynum;

	cout << "Enter recnum: ";
	cin >> recnum;

	cout<<"Enter the priority of every key in range [0, " << (keynum - 1) << "]:\n";

	for(unsigned i = 0; i < keynum; i++)
	{
		unsigned key;
		cout<<"Enter the priority of key[" << i <<"]:";
		cin >> key;
		priority.push_back(key);
	}

	cout << "Enter \"1\" for LSD_Sort		\"2\" for MSD_Sort: ";
	cin >> type[0];
	cout << "Enter \"1\" for Radix_Sort	\"2\" for Merge_Sort: ";
	cin >> type[1];
}
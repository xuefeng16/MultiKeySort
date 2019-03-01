#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "MultikeyData.h"

using namespace std;

namespace MultikeyData
{	//generate perData
	PerData DataGen_Manual(int keynum, int i)	//generate Perdata manually
	{
		PerData data;
		KeyType a;
		if(!i)
			data.Setnext(1);
		else
		{
			for(int j = 0; j < keynum; j++)
			{
				cout << "Enter the records["<< i <<"]["<< j <<"]";
				cin >> a;
				data.Setkey(j, a);
			}
			data.Setnext(i+1);
		}
		return data;
	}

	PerData DataGen_Random(int keynum, int i)	//generate Perdata randomly
	{
		PerData data;
		if(!i)
			data.Setnext(1);
		else
		{
			for(int j = 0; j < keynum; j++)
			{
				data.Setkey(j, rand()%101 );
			}
			data.Setnext(i+1);
		}
		return data;
	}
}
 
namespace MultikeyData
{	//Database
	void Database::SetData()
	{
		cout << "Enter \"1\": Randomly Generate data" <<endl;
		cout << "Enter \"2\": Manually Generate data: ";

		int type;
		cin >> type;
		if(type == 1)
		{
			srand( (unsigned) time(NULL) );
			for(unsigned i= 0; i <= recnum; i++)
				records.push_back(DataGen_Random(keynum, i));
		}
		else if(type == 2)
			for(unsigned i= 0; i <= recnum; i++)
				records.push_back(DataGen_Manual(keynum, i));
		records[recnum].Setnext(0);
	}

	void Database::Distribute(int i,int f[], int e[], int low, int high) //Diatribute the ith priottiry
	{
		int j;
		for(j= 0; j<RADIX; j++)	f[j] = 0;
		for(j= getNext(low); j != high; j=getNext(j))
		{
			int m = getData(j)[i];
			if(!f[m]) f[m] = j;
			else SetNext(e[m],j);	//set the recodes[e[j]].next = j
			e[m] = j;
		}
	}

	void Database::Collect(int i,int f[], int e[], int low, int high)	//Collect the ith priottiry
	{
		int j,t;
		for(j= RADIX-1; !f[j]; j--); //Find the first non-empty sublist
		SetNext(low,f[j]);
		t = e[j];
		while(j >= 0)
		{
			for(j-- ; j>=0 &&!f[j]; j--);	//find the next non-empty sublist
			if(f[j] && j>=0)
			{
				SetNext(t,f[j]);
				t = e[j];
			}
		}
		SetNext(t,high);
	}

	void Database::LSD_RadixSort(vector<unsigned> order)	//find the next priority
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		int f[RADIX], e[RADIX];
		for(int j = 0; j<keynum; j++)	//from the lowestest priority to highest priority
		{
			Distribute(order[j], f, e, 0, 0);
			Collect(order[j], f, e, 0, 0);
		}
	}

	void Database::LSD_MergeSort(vector<unsigned> order)
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		for(int j = 0; j<keynum; j++)	//from the lowestest priority to highest priority
		{
			MergeSort(order[j], 1, recnum);
		}
	}

	void Database::MSD_RadixSort(int k, int low, int high, vector<unsigned> order)
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		int f[RADIX], e[RADIX];
		int temp = low;
		if(high != getNext(low) && high != getNext(getNext(low)) )
		{
			Distribute(order[k], f, e, low, high);
			Collect(order[k], f, e, low, high);
			int head, tail = getNext(low);	//head: precursor of a number  tail: successor of a number
			for(unsigned j = RADIX-1; j>= 0 && j < RADIX; j--)
			{
				while(getNext(temp) != tail )
					temp = getNext(temp);
				for(head = tail; getData(head)[order[k]] > j; head = getNext(head))
					temp = head;
				for(tail = head;  tail != high && getData(tail)[order[k]] == j; tail = getNext(tail));
				if(getData(head)[order[k]] == j)
				{
					if(k == keynum -1) break;
					else
						MSD_RadixSort(k+1, temp, tail, order);
				}
				if(tail == high)
					break;
			}
		}
	}

	void Database::MergeSort(unsigned key, int low, int high)
    {
        if (low < high) {
            int mid = (low + high) / 2;
            MergeSort(key, low, mid);
            MergeSort(key, mid + 1, high);
			Merge(key, low, mid, high);
        }
    }

	void Database::Merge(unsigned key, int low, int mid, int high)
	{
		int keynum = getkeynum();
		MultikeyData::Database temp(keynum, high-low+1);
		int i = low, j = mid + 1;
		while (i <= mid && j <= high)
		{
			if ( getData(i)[key] < getData(j)[key])
				temp.AddData(getPerData(j++));
			else
				temp.AddData(getPerData(i++));
		}
		while(i <= mid)
			temp.AddData(getPerData(i++));
		while(j <= high)
			temp.AddData(getPerData(j++));

		for (i = 0; i != high-low +1; i++)
			SetData(low + i, temp.getPerData(i));
	}

	//MSD_MergeSort
	void Database::MSD_MergeSort(int k, int low, int high, vector<unsigned> order)
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		int head, tail = low;
		if(low != high)
		{
			MergeSort(order[k],low,high);
			for(unsigned j = RADIX-1; j>= 0 && j < RADIX; j--) //from the highest priority to lowest priority
			{
				for(head = tail; getData(head)[order[k]] > (int)j; head++);
				for(tail = head;  tail <= high && getData(tail)[order[k]] == j; tail++);
				if(getData(head)[order[k]] == j)
				{
					if(k == keynum -1) break;
					else
						MSD_MergeSort(k+1, head, tail-1, order);
				}
				if((tail - 1) == high)
					break;
			}
		}
	}


	//type = 1 for Raxid sort;	type = 2 for Merge sort
	void Database::LSD_Sort(vector<unsigned> order, int type) 
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		if(type == 1)
			LSD_RadixSort(order);
		else if(type == 2)
			LSD_MergeSort(order);
	}

	void Database::MSD_Sort(vector<unsigned> order, int type)
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		if(type == 1)
			MSD_RadixSort(order[0], 0, 0, order);
		else if(type == 2)
			MSD_MergeSort(order[0], 1, recnum, order);
	}

	//print
	void Database::Print(int type)
	{
		if(type == 1)
			PrintDatabase_RadixSort();
		else if(type == 2)
			PrintDatabase_MergeSort();
	}

	void Database::PrintDatabase_RadixSort()
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		cout << "Keys:    ";
		for(int i= 0; i<keynum; i++)
		{
			cout.width(5);
			cout << char('A'+ i);
		}
		cout << endl;
		for(int i= getNext(0); i <= recnum  && i>0; i= getNext(i))
		{
			cout << "r["<<i<<"]";
			if(i < 10) cout << "     ";
			else if(i < 100) cout << "    ";
			else if(i < 1000) cout << "   ";
			else if(i < 10000) cout << "  ";
			else cout << " ";
			vector<KeyType> temp = getData(i);
			for(int i = 0; i< keynum; i++){
				cout.width(5);
				cout << temp[i];
			}
			cout << endl;
		}
	}

	void Database::PrintDatabase_MergeSort()
	{
		int keynum = getkeynum();
		int recnum = getrecnum();
		cout << "Keys:   ";
		for(int i= 0; i<keynum; i++)
		{
			cout.width(5);
			cout << char('A'+ i);
		}
		cout << endl;
		for(int i= 1; i <= recnum  && i>0; i++)
		{
			cout << "r["<<i<<"]";
			if(i < 10) cout << "    ";
			else if(i < 100) cout << "   ";
			else if(i < 1000) cout << "  ";
			else if(i < 10000) cout << " ";
			vector<KeyType> temp = getData(i);
			for(int i = 0; i< keynum; i++)
			{
				cout.width(5);
				cout << temp[i];
			}
			cout << endl;
		}
	}
}

namespace MultikeyData
{
	unsigned LSD_getNextKey(vector<unsigned> priority, unsigned i, unsigned keynum)	//(LSD) find the next priority
	{
		for(unsigned j=0; j<keynum; j++)
		{
			if( priority[j] == i)
				return j;
		}
		return -1;
	}

	unsigned MSD_getNextKey(vector<unsigned> priority, unsigned i, unsigned keynum)	//(MSD) find the next priority
	{
		for(unsigned j= keynum-1; j>=0; j--)
		{
			if( priority[j] == i)
				return j;
		}
		return -1;
	}

	vector<unsigned> SetOrder(vector<unsigned> priority, int type, unsigned keynum)
	{
		vector<unsigned> order;
		if(type == 1)
		{
			for(unsigned i = 0; i<= keynum-1; i++)
				order.push_back(LSD_getNextKey(priority, i, keynum));
		}
		else if(type == 2)
		{
			for(unsigned i = keynum-1; i < keynum; i--)
				order.push_back(MSD_getNextKey(priority, i, keynum));
		}
		return order;
	}

}
#include <iostream>
#include <vector>

typedef unsigned int KeyType;
#define MAX_NUM_OF_KEY 5
#define RADIX 101

using namespace std;

namespace MultikeyData
{
    class PerData
    {
    private:
        vector<KeyType> keys;
        int next;
    public:
		PerData() {}
		PerData DataGen_Manual(int keynum,int i);
		PerData DataGen_Random(int keynum,int i);
		void Setkey(unsigned n, KeyType d) { if(n<keys.size()) keys[n] = d; else keys.push_back(d); }
		void Setnext(unsigned d) { next = d;}
		vector<KeyType> getData() const { return keys;}
		int getNext() const { return next;}
		
    };
}
 
namespace MultikeyData
{
    class Database
    {
	private:
		vector<PerData> records;	// records
        unsigned keynum;			// number of keys
        unsigned recnum;			// number of records

    public:
		Database(int keynum, int recnum): keynum(keynum), recnum(recnum) { }

		void SetData();
		void SetData(int n, PerData data) { records[n] = data; }
		void SetNext(int m, int n) { records[m].Setnext(n);} //set records[m].next = n
		void PrintDatabase_RadixSort();
		void PrintDatabase_MergeSort();
		void Print(int type);
		unsigned getkeynum() const { return keynum; }
		unsigned getrecnum() const { return recnum; }
		int getNext (int n) const { return records[n].getNext();}
		vector<KeyType> getData(int n) const { return records[n].getData();}
		PerData getPerData(int n) const { return records[n];}
		void AddData(PerData keys) { records.push_back(keys); recnum++; }
		//RadixSort
		void LSD_RadixSort(vector<unsigned> order);
		void MSD_RadixSort(int k, int low, int high, vector<unsigned> order);
		void Distribute(int i,int f[], int e[], int low, int high);
		void Collect(int i,int f[], int e[], int low, int high);
		//MergeSoft
		void LSD_MergeSort(vector<unsigned> order);
		void MSD_MergeSort(int k, int low, int high, vector<unsigned> order);
		void MergeSort(unsigned key, int low, int high);
		void Merge(unsigned key, int low, int mid, int high);

		void LSD_Sort(vector<unsigned> priority, int type);
		void MSD_Sort(vector<unsigned> priority, int type);

	};
}
namespace MultikeyData
{
	unsigned LSD_getNextKey(vector<unsigned> priority, unsigned i, unsigned keynum);
	unsigned MSD_getNextKey(vector<unsigned> priority, unsigned i, unsigned keynum);
	vector<unsigned> SetOrder(vector<unsigned> priority, int type, unsigned keynum);
}
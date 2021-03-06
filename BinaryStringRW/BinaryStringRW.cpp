#include <iostream>
#include <fstream>
#include <string>
#include <malloc.h>  //for _msize() function
/*
@zh-CN
本算法用来测试将二进制以字符串的形式读出与将字符串（二进制形式）写入原文件，保存数据不变
by Kanch @ 2015 10-4
http://akakanch.com
@en-us
this algorithm is used to read file's binary out as string then write those binary(string) into a new file to reduct the raw file
by Kanch @2015-10-4
mailto://kanch@akakanch.com
*/


using namespace std;


//test infomation print function
template<class T>
inline void TEST(const T dta)
{
	cout << dta << endl;
}


//essential function definations
const string readBinary(const string filepath);                              //该函数实现将二进制以字符串的形式读出

const string writeBinary(const string filepath,const string & binData);      //该函数实现将字符串形式二进制写入文件

void determinBitData(const int sbitlen);                                    //该函数用来决定位数组大小

//essential specific binary bit varibles note:short should no smaller than char,so, minium is 16 bit,2 bytes;
//array below with 32 elements,like 32 bits,4 bytes.
short SV[] = { 0x1,0x2,0x4,0x8,
			   0x10,0x20,0x40,0x80,
			   0x100,0x200,0x400,0x800,
			   0x1000,0x2000,0x4000,0x8000,
			   0x10000,0x20000,0x40000,0x80000,
			   0x100000,0x200000,0x400000,0x800000,
			   0x1000000,0x2000000,0x4000000,0x8000000,
			   0x10000000,0x20000000,0x40000000,0x80000000,
			 };

//for this algorithm can run successfully on another systems,we have to know the length of type short in specfic systems.
int slen = sizeof(short);   //slen is the len of type short
int sbitlen = slen * 8;     //sbitlen is the bits's sum of short io specfic systems.
short * bitdta;   //used to save specfic length of short's  bit data


//some essential test varibles
const char* testfilepath = "C:\\binalgtest\\rawfile.txt";      //the raw file path
const char* binarydatapath = "C:\\binalgtest\\binstring.txt";    //binary string path
const char* targetfilepath = "C:\\binalgtest\\reductionfile.txt";    //reduction file via a known binary string(above 1)

int main(int argc, char**argv)
{

	cout << "slen= "; TEST(slen);
	cout << "sbitlen= "; TEST(sbitlen);
	cout << "(sizeof(SV) / sizeof(SV[0]))= "; TEST((sizeof(SV) / sizeof(SV[0])));

	TEST("call determinBitData");
	determinBitData(sbitlen);
	cout << "(_msize(bitdta) / sizeof(bitdta[0]))= "; TEST((sizeof(bitdta) / sizeof(bitdta[0])));
	cout << "data of bitdta[]:" << endl;

	TEST("[S1-----now start test readBinary function(no reverse)-------");	
	cout << "readBinary()= " << readBinary(testfilepath) << endl;
	TEST("[S1-----readBinary function test completed,please check result------\n\n\n\n");

	TEST("[S2-----now start test writeBinary function------");
	writeBinary(targetfilepath, readBinary(testfilepath));
	TEST("[S2-----writeBinary function test completed,please check result------");
	TEST("[S3-----we will use readBinary function to read out the bits of targetfile------");
	TEST("[S3-call readBinary");
	TEST("[S3--result:");
	cout << readBinary(targetfilepath) << endl;
	TEST("[S3-----plesae check the output,to see if writeBinary function works properly------");
	TEST("[\n\n\n\nS4-----BinaryStringRW algorithm all test completed------\n\n\n");
	

	delete[] bitdta;
	return 0;
}


//specfic functions definations
//
void determinBitData(const int s)                          
{
	if (s <= (sizeof(SV)/sizeof(SV[0])))
	{
		bitdta = new short[s];
		for (int i = 0; i < s; i++)
		{
			bitdta[i] = SV[i];
		}
	}
	else
	{
		cout << "out of range" << endl;
	}
}

const string readBinary(const string filepath)
{
	ifstream read(filepath,ios_base::binary|ios_base::in|ios_base::ate);  //open specfic file,then move pointer to the end of file
	string binData = "", binBuf = "";
	int fileylen = read.tellg();   //tell  the len of the file (bytes)
	read.seekg(0, ios_base::beg);  //reduct the pointer to the begin of the file in case of steps behind read nothing out :(
	int fileblen = fileylen * 8;
	short buf = 0, bbuf = 0;
	int looprounds = 0;  
	int lastshortbitscover = 0;  //#V1
	///*/
	//determine how much loops need to read out all the bits;
	if ( (fileylen%sizeof(short)) )  //#1
	{
		looprounds = (fileylen / sizeof(short)) + 1;
		lastshortbitscover = (fileylen%sizeof(short))*8; //rember,we need bits instead of byte :(
	}
	else                            //#2
	{
		looprounds = (fileylen / sizeof(short));
	}
	//start reading,
	//notice:if #1 was excuted,we need to use a varible to flag how much bits in the last short are really in raw file
	//so,this varible is #V1
	//pocess below doesn't reverse the bits read;if we reverse it,changes should also be make in writeBinary function
	for (int i = 0; i < looprounds; i++)
	{
		read.read((char*)&buf, sizeof(short));
		if (i < (looprounds - 1))
		{
			for (int m = 0; m < sbitlen; m++)
			{
				if ( ( buf & bitdta[m] ) == bitdta[m] )
				{
					binBuf += "1";
				}
				else
				{
					binBuf += "0";
				}
			}
		}
		else
		{
			for (int m = 0; m < lastshortbitscover; m++)
			{
				if ( ( buf & bitdta[m] ) == bitdta[m] )
				{
					binBuf += "1";
				}
				else
				{
					binBuf += "0";
				}
			}
		}
		//I'm thinking wether should I reverse the bits read to a acceptable format,at least,for we huamn
		//what's a acceptable format?look here:https://docs.google.com/drawings/d/1odXlKXx6MggQvDeObna4wYsEd_kVF23HoaM_Si7z32M/edit?usp=sharing
		//if necessary to reverse,add code here(above)
		binData += binBuf;
		binBuf = "";
	}
///*/

	//TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST  START
	//test code ,to test this if this function works properly
	cout << "file length(from tellg())= " << fileylen << " bytes" << endl;
	cout << "file length(from tellg())= " << fileblen << " bits" << endl;
	read.seekg(0, ios_base::beg);
	read.read((char*)&buf, sizeof(short));
	read.read((char*)&bbuf, sizeof(short));
	cout << "after read from file,short buf = " << buf << endl << "bbuf = " << bbuf << endl;
	cout << "looprounds= " << looprounds << endl;
	cout << "lastshortbitscover= " << lastshortbitscover << endl;
	//TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST  END



	read.close();
	return binData;
}

/*/
important notice:because I haven't reverse the bits read out in readBinary function,
so when you make changes in writeBinary,take care of bits' sequence :)
in order to write bits completely,I may use more than one types to write.
/*/
const string writeBinary(const string filepath,const string & binData)
{
	ofstream write(targetfilepath,ios_base::out|ios_base::binary);
	string a = binData, sbuf = "";
	short buf = 0;
	int looprounds = 0, lastshortbitscover = 0;
	//determines how many loops do we need,and the last type to use to write
	if ( (binData.length()%sbitlen) == 0 )
	{
		looprounds = binData.length()/sbitlen;
	}
	else
	{
		looprounds = binData.length() / sbitlen + 1;
		lastshortbitscover = binData.length() % sbitlen;     //this is very essencial to ensure the complatence of data,this derterines last type to write
	}
	if (lastshortbitscover != 0 && lastshortbitscover%8 == 0)         //seems there are some bits left,so we need to decide which type to use to write the last varible
	{
		for (int i = 0; i < looprounds; i++)
		{
			if (i < (looprounds - 1)) //#9
			{
				sbuf = a.substr(sbitlen*i,sbitlen);  //read sizeof(short) 's char 0 or 1
				for (int m = 0; m < sbitlen; m++)
				{
					if (sbuf[m] == '1')   //now set the bit of short
					{
						buf = (buf | bitdta[m]);
					}

				}
				write.write((char*)&buf, sizeof(short));
				buf = 0;        //set all bits to 0
				sbuf = "";

			}
			else     //this is used to write the last varible,I don't know use which type,so we need to determine by length
			{
				sbuf = a.substr(sbitlen*i, a.length() - sbitlen*i); //the last bits of data
				//if there really have some bits left,it should be 8 bits,so,we use char to write the last bits
				char lastbits = 0;
				for (int m = 0; m < sbuf.length(); m++)
				{
					if (sbuf[m] == '1')
					{
						lastbits = (lastbits | bitdta[m]);
					}
				}
				write.write((char*)&lastbits, sizeof(char));

			}
		}
	}
	else if (lastshortbitscover == 0)     //no bits left for short?so,we just need to write
	{
		for (int i = 0; i < looprounds; i++)   //code copied from #9
		{
			sbuf = a.substr(sbitlen*i, sbitlen);  //read sizeof(short) 's char 0 or 1
			for (int m = 0; m < sbitlen; m++)
			{
				if (sbuf[m] == '1')   //now set the bit of short
				{
					buf = (buf | bitdta[m]);
				}

			}
			write.write((char*)&buf, sizeof(short));
			buf = 0;        //set all bits to 0
			sbuf = "";
		}
	}
	
	write.close();
	return a;
}
#include <iostream>
#include <fstream>
#include <string>
#include <intrin.h>
using namespace std;

#pragma pack(push,1)
struct BIOSParameterBlock {
	unsigned long long oemId;
	unsigned short bytesPerSector;
	unsigned char sectorPerCluster;
	unsigned short reservedSectorCount;
	unsigned char tableCount[3];
	unsigned short rootEntryCount;
	unsigned char mediaDescriptor;
	unsigned short tableSize16;
	unsigned short sectorsPerTrack;
	unsigned short headSideCount;
	unsigned int hiddenSectorCount;
	unsigned char unused[8];
	unsigned long long totalSectorCount;
	unsigned long long mftLogicalClusterNumber;
	unsigned long long mftmirrLogicalClusterNumber;
	int clusterPerMftRecord;
	char clusterPerIndexBlockRecord;
	unsigned char reseved1[3];
	unsigned long long volumeSerialNumber;
	unsigned int checksum;
};

struct NTFSBootSector {
	unsigned char jumpInstructions[3];
	//unsigned char dummy[];
	struct BIOSParameterBlock bpb;
	unsigned char bootCode[426];
	unsigned short endMarker;
};

struct AttributeBegin {
	long attrCode;
	long attrSize;
	char formCode;
	char nameLen;
	short nameOffset;
	//unsigned char flags[2];
	short flags;
	short attrId;
};

struct TimePoints {
	char timeCreated[8];
	char timeAltered[8];
	char timeMFTChanged[8];
	char timeRead[8];
};

#pragma pack(pop)

int main()
{
	ifstream fin;
	fin.open("D:/Учеба/6 семестр/КК/ntfs-img-kw-1.dd", ifstream::in | ifstream::binary);
	struct NTFSBootSector bs;
	if (!fin.fail()) {
		fin.read((char*)&bs, sizeof(bs));

		cout << "oemId: " << bs.bpb.oemId << "\n";
		cout << "sectorPerCluster: " << int(bs.bpb.sectorPerCluster) << "\n";
		cout << "bytesPerSector: " << bs.bpb.bytesPerSector << "\n";
		cout << "reservedSectorCount: " << bs.bpb.reservedSectorCount << "\n";
		cout << "rootEntryCount: " << bs.bpb.rootEntryCount << "\n";
		cout << "tableSize16: " << bs.bpb.tableSize16 << "\n";
		cout << "sectorsPerTrack: " << bs.bpb.sectorsPerTrack << "\n";
		cout << "headSideCount: " << bs.bpb.headSideCount << "\n";
		cout << "hiddenSectorCount: " << bs.bpb.hiddenSectorCount << "\n";
		cout << "totalSectorCount: " << bs.bpb.totalSectorCount << "\n";
		cout << "mftLogicalClusterNumber: " << bs.bpb.mftLogicalClusterNumber << "\n";
		cout << "mftmirrLogicalClusterNumber: " << bs.bpb.mftmirrLogicalClusterNumber << "\n";
		cout << "clusterPerMftRecord: " << bs.bpb.clusterPerMftRecord << "\n";
		cout << "volumeSerialNumber: " << bs.bpb.volumeSerialNumber << "\n";
		cout << "checksum: " << bs.bpb.checksum << "\n\n";

		long long mftOffset = bs.bpb.bytesPerSector * int(bs.bpb.sectorPerCluster) * bs.bpb.mftLogicalClusterNumber;
		long long mftRecordSize = bs.bpb.bytesPerSector * int(bs.bpb.sectorPerCluster) * bs.bpb.clusterPerMftRecord;
		cout << "MFT offset: " << mftOffset << "\n";
		cout << "MFT record size: " << mftRecordSize << "\n";

		char* fileStrTemplate = new char[5]{'F','I','L','E','\0'};
		char* fileStr = new char[5];

		fin.seekg(mftOffset, ios::beg);
		fin.read(fileStr, 4);
		fileStr[4] = '\0';

		int i = 0;

		while (strcmp(fileStrTemplate, fileStr) == 0)
		{
			long long curRecOffset = mftOffset + i * mftRecordSize;
			long long nextRecOffset = mftOffset + (i + 1) * mftRecordSize;
			fin.seekg(16, ios::cur);
			//long firstAttrOffset;
			char* firstAttrOffsetChar = new char[2];
			fin.read(firstAttrOffsetChar, 2);
			cout << "File " << i << ": " << "\n";
			char* flagsChar = new char[2];
			fin.read(flagsChar, 2);
			int flag = int(flagsChar[1] * 256 + flagsChar[0]);
			cout << "Flag: " /*<< flag */<< "\n";
			switch (flag)
			{
			case 255:
				cout << "Compressed" << "\n";
				break;
			case 32768:
				cout << "Sparse" << "\n";
				break;
			case 16384:
				cout << "Encrypted" << "\n";
				break;
			default:
				cout << "Default" << "\n";
				break;
			}
			int firstAttrOffset = int(firstAttrOffsetChar[1]) * 256 + int(firstAttrOffsetChar[0]);
			cout << "First Attribute Offset: " << firstAttrOffset << "\n";

			fin.seekg(curRecOffset, ios::beg);
			fin.seekg(firstAttrOffset, ios::cur);

			AttributeBegin attr;
			TimePoints times;
			//char* len = new char[2];

			fin.read((char*)&attr, sizeof(attr));

			bool validAttrCode = true;
			while (validAttrCode) {
				string attrName;
				//int attrCode = int(attrCodeChar[0]);//тут не берем все 4 байта, ибо нам важен только первый
				switch (attr.attrCode) {
				case 16:
					attrName = "Standard Information";
					//todo временные метки
					fin.seekg(8, ios::cur);//16
					fin.read((char*)&times, sizeof(times));
					fin.seekg(-8 - ((int)sizeof(times)), ios::cur);//16
					break;
				case 32:
					attrName = "Attribute List";
					break;
				case 48:
				{
					attrName = "File Name";
					//todo имя
					fin.seekg(8, ios::cur);//прошли хедер атрибута//16
					fin.seekg(64, ios::cur);//прыгнули до длины имени
					char* len = new char[3];
					fin.read(len, sizeof(len) - 1);
					int fnlen = /*int(len[1]) * 256 + */int(len[0]);
					//len[1] = '\0';
					delete[](len);
					//fin.seekg(1, ios::cur);//прыгнули до имени
					////string fname;
					//tmp = fname;

					char* fname = new char[fnlen * 2];
					fin.read(fname, fnlen * 2);

					cout << "Attribute: " << attrName << "\n";
					cout << "	Name lenth: " << fnlen << "\n";
					cout << "	Name: " /*<< string(fname)*/ << "\n";
					for (int i = 1; i < fnlen * 2 - 1; i+=2) {
						cout << "		" << i/2 << ": " << fname[i] << "\n";
					}
					/*cout << "		1: " << fname[0] << "\n";
					cout << "		2: " << fname[1] << "\n";
					cout << "		3: " << fname[2] << "\n";
					cout << "		4: " << fname[3] << "\n";*/

					delete[](fname);

					fin.seekg(-8 - 64 - 2 - fnlen * 2, ios::cur);//16
					break;
				}
				case 64:
					attrName = "Object ID";
					break;
				case 80:
					attrName = "Security Descriptor";
					break;
				case 96:
					attrName = "Volume Name";
					break;
				case 112:
					attrName = "Volume Information";
					break;
				case 128:
					attrName = "Data";
					break;
				case 144:
					attrName = "Index Root";
					break;
				case 160:
					attrName = "Index Allocation";
					break;
				case 176:
					attrName = "Bitmap";
					break;
				case 192:
					attrName = "Reparse Point";
					break;
				case 208:
					attrName = "Logged Tool Stream";
					break;
				default:
					validAttrCode = false;
				}
				if (validAttrCode) {
					if (attr.attrCode != 48) {
						cout << "Attribute: " << attrName << "\n";
					}
					if (attr.attrCode == 16) {
						cout << "Times: " << "\n";
						cout << "	Created: " << times.timeCreated << "\n";
						cout << "	Altered: " << times.timeCreated << "\n";
						cout << "	MFTChanged: " << times.timeCreated << "\n";
						cout << "	Read: " << times.timeCreated << "\n";
					}
					//else if (attr.attrCode == 48) {
					//	cout << "	File Name: " << fname[0] << "\n";
					//	//delete[](fname);
					//}

					cout << "Flags: " << "\n";
					switch (attr.flags)
					{
					case 255:
						cout << "		Compressed" << "\n";
						break;
					case 32768:
						cout << "		Sparse" << "\n";
						break;
					case 16384:
						cout << "		Encrypted" << "\n";
						break;
					default:
						cout << "		Default" << "\n";
						break;
					}

					fin.seekg(-((int)sizeof(attr)), ios::cur);
					fin.seekg(attr.attrSize, ios::cur);
					fin.read((char*)&attr, sizeof(attr));
				}
				
			}
			

			//delete[]((char*)&attr);
			//delete[](len);
			fin.seekg(nextRecOffset, ios_base::beg);//go to next mft record
			//fin.seekg(mftRecordSize - 4 - 16 - 2, ios::cur);//go to next mft record
			fin.read(fileStr, 4);
			fileStr[4] = '\0';
			i++;
			delete[](firstAttrOffsetChar, flagsChar);
		}

		cout << "Number of files: " << i << "\n";


		//delete(record);
		delete[](fileStr, fileStrTemplate);
		return 0;
	}
	else {
		cout << "Error, motherf*cker!\n";
		return 1;
	}
	return 0;
}

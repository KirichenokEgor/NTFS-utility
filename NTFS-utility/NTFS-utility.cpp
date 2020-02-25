#include <iostream>
#include <fstream>
#include <string>
#include <intrin.h>
using namespace std;

#pragma pack(push,1)
struct BIOSParameterBlock {
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
	unsigned long long oemId;
	//unsigned char dummy[];
	struct BIOSParameterBlock bpb;
	unsigned char bootCode[426];
	unsigned short endMarker;
};
#pragma pack(pop)

int main()
{
	ifstream fin;
	fin.open("D:/Учеба/6 семестр/КК/ntfs-img-kw-1.dd", ifstream::in | ifstream::binary);
	struct NTFSBootSector bs;
	if (!fin.fail()) {
		fin.read((char*)&bs, sizeof(bs));

		cout << "oemId: " << bs.oemId << "\n";
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
		cout << "checksum: " << bs.bpb.checksum << "\n";

		return 0;
	}
	else {
		cout << "Error, motherf*cker!\n";
		return 1;
	}
	return 0;
}

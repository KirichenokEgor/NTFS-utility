#include <iostream>
#include <fstream>
using namespace std;

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
	unsigned short hiddenSectorCount;
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
	unsigned char oemId[8];
	//unsigned char dummy[];
	struct BIOSParameterBlock bpb;
	unsigned char bootCode[426];
	unsigned short endMarker;
};

int main()
{
	ifstream fin;
	fin.open("D:/Учеба/6 семестр/КК/ntfs-img-kw-1.dd", ifstream::in | ifstream::binary);
	struct NTFSBootSector bs;
	if (!fin.fail()) {
		fin.read((char*)&bs, sizeof(bs));
		//cout << bs.jumpInstructions[0] + bs.jumpInstructions[1] + bs.jumpInstructions[2] << "\n";
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
		
		cout << "jumpInstructions: " << bs.jumpInstructions << "\n";
		return 0;
	}
	else {
		cout << "Error, motherf*cker!\n";
		return 1;
	}
    cout << "Hello World!\n";
	return 0;
}

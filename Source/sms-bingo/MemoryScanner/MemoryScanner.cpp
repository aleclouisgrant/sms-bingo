#include "MemoryScanner.h"
#include "Common/CommonTypes.h"
#include "Common/CommonUtils.h"
#include "Common/MemoryCommon.h"
#include <DolphinProcess/DolphinAccessor.h>

#include <thread>
#include <Windows.h>

MemoryScanner::MemoryScanner()
{
}


MemoryScanner::~MemoryScanner()
{
}

void MemoryScanner::Start() {
	std::thread scannerThread(&MemoryScanner::FirstScan, this);
	scannerThread.detach();
}

void MemoryScanner::FirstScan() {

	char addressbuffer[sizeof(u32)] = { 0 };
	// List of addresses
	u32 blue_coin_address = 0x80578A5F;
	u32 shine_total_address = 0x80578A5B;


	DolphinComm::DolphinAccessor::readFromRAM(Common::dolphinAddrToOffset(
		blue_coin_address), addressbuffer, sizeof(u32), false);
	blue_coin_total = (int)addressbuffer[0];

	while(true) {

		//I'm going to probably just emit whenever a pertinent value changes
		//and then handle whether a space should be selected in the BoardWindow
		//class

		if (Scan(blue_coin_address, &blue_coin_total)) {
			if (blue_coin_total == 17) {
				emit ValueChanged(0);
			}
		}

		if (Scan(shine_total_address, &shine_total)) {
			
			if (shine_total == 4) {
				emit BingoSpace_OneShine(9);
			}
		}

		Sleep(100);
	}
}

//I wish I could just pass a signal as a parameter to be called 
//here but I'm not sure how
bool MemoryScanner::Scan(u32 address, int *var) 
{
	char addressBuffer[sizeof(u32)];

	DolphinComm::DolphinAccessor::readFromRAM(Common::dolphinAddrToOffset(address), 
		addressBuffer, sizeof(u32), false);

	if (addressBuffer[0] != *var) {
		*var = (int)addressBuffer[0];
		return true;
		//would rather call emit signal(); here than check return status
	}

	return false;
}
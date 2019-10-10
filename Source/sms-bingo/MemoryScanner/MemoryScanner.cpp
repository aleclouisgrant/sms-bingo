#include "MemoryScanner.h"
#include "Common/CommonTypes.h"
#include "Common/CommonUtils.h"
#include "Common/MemoryCommon.h"
#include <DolphinProcess/DolphinAccessor.h>

#include <thread>
#include <Windows.h>

MemoryScanner::MemoryScanner(BingoBoard *bingoBoard)
{
	m_bingoBoard = bingoBoard;
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

	while (true) {
		Scan(blue_coin_address, &m_bingoBoard->getPlayer()->m_blueCoinTotal);
		Scan(shine_total_address, &m_bingoBoard->getPlayer()->m_shineTotal);

		Sleep(100);
	}
}

void MemoryScanner::Scan(u32 address, int *var) 
{
	char addressBuffer[sizeof(u32)];

	DolphinComm::DolphinAccessor::readFromRAM(Common::dolphinAddrToOffset(address), 
		addressBuffer, sizeof(u32), false);

	if (addressBuffer[0] != *var) {
		m_bingoBoard->getPlayer()->SetValue(var, addressBuffer[0]);
		m_bingoBoard->CheckBoard();
	}
}
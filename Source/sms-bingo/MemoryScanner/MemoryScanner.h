#pragma once

#include "Common/CommonTypes.h"
#include "Common/CommonUtils.h"
#include "Common/MemoryCommon.h"
#include "BingoBoard2.h"

class MemoryScanner {

public:
	MemoryScanner(BingoBoard *bingoBoard);
	~MemoryScanner();
	
	void Start();

	void ScanAll();

	int blue_coin_total = 0;
	int shine_total = 0;

private:
	void FirstScan();
	void Scan(u32 address, int *var);

	BingoBoard *m_bingoBoard;
	char** addressList;
};


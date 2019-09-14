#pragma once

#include "Common/CommonTypes.h"
#include "Common/CommonUtils.h"
#include "Common/MemoryCommon.h"
#include <qobject.h>

class MemoryScanner : public QObject
{
	Q_OBJECT

public:
	MemoryScanner();
	~MemoryScanner();
	
	void Start();

	void ScanAll();

	int blue_coin_total = 0;
	int shine_total = 0;

signals:
	void ValueChanged(int id);
	void BingoSpace_OneShine(int id);

private:
	void FirstScan();
	bool Scan(u32 address, int *var);

	char** addressList;
};


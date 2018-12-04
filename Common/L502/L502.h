#pragma once

class L502
{
	void *hnd;
public:
  static const int buffer_length = 10 * 1024;  
  int READ_TIMEOUT;
public:
	L502();
	bool Init();
	void Destroy();
	bool SetupParams();
	int Start();
	int Stop();
	bool Read(unsigned &startChennel, double *data, unsigned &count);
	bool ReadAsync(unsigned ch, int mode, int range, double &value);
	bool ReadAsyncChannels(int count, unsigned *ch, int *mode, int *range, double *value);

	bool BitOut(unsigned ch, bool value);
	bool BitIn(unsigned &value);
};

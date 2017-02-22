#pragma once

class PasteBot
{
public:
	PasteBot(): inBuffer(NULL), load_lock(false) {}
	~PasteBot() { delete[] inBuffer; }

	void SetOwner(HWND Owner) { hwndOwner = Owner; }
	void SetHWar3(HWND War3) { hWar3 = War3; }
	void Load();
	void Send(unsigned short &delay);
	void inline Clean() { delete[] inBuffer; inBuffer = NULL; }

private:
	HWND hwndOwner, hWar3;
	char * inBuffer;
	DWORD fileBytes;
	bool load_lock;
};

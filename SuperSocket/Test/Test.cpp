#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include "SuperSocketAPI.h"
using namespace std;

void Onconnect(void* userPointer)
{
	cout << "connect"<<endl;
}

void OnDisconnect(void* userPointer)
{
	cout << "disconnect" << endl;
}

void OnRecive(const DataBlock& db, void* userPointer)
{
	string msg((const char*)db.addr);
	cout << "receive:"<<msg << endl;
}

void OnSend(SSError err, void* userPointer)
{
	cout << "send " << endl;
}

int main()
{
	SS_InitSSLib();
	string msg("hello server");
	size_t len = msg.size() + 1;
	DataBlock db{ len,(char*)msg.c_str() };

	SSEventCallback callbacks{ 0 };
	callbacks.userPointer = (void*)0xce4141;
	callbacks.cbConnect = Onconnect;
	callbacks.cbDisconnect= OnDisconnect;
	callbacks.cbReceive= OnRecive;
	callbacks.cbSend= OnSend;
	int fd;
	auto ss=SS_CreateSocket(&fd, "127.1.1.1:5050", &callbacks);
	system("pause");

	while (true)
	{
		SS_SendBlock(fd, db);
		Sleep(500);
	}
	system("pause");
	SS_DestroySocket(fd);
	SS_DeinitSSLib();
	system("pause");
	return 0;
}
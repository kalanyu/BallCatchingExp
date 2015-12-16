#include "stdio.h"
#include "Triger.h"
#include "conio.h"
#include "windows.h"

void Triger::triger_on(){

	HANDLE h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(h == INVALID_HANDLE_VALUE) {
		//GIVEIOがインストールされてなかった場合の処理をここに記述
		printf("error\n");
	}

	CloseHandle(h);
	
	_outp(IOPORT_CONTROL, 0);
	_outp(IOPORT_DATA, 1);	
	printf("triger is on!\n");

}

void Triger::triger_off(){
	
	HANDLE h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(h == INVALID_HANDLE_VALUE) {
		//GIVEIOがインストールされてなかった場合の処理をここに記述
		printf("error\n");
	}

	CloseHandle(h);
	
	_outp(IOPORT_CONTROL, 0);
	_outp(IOPORT_DATA, 0);	
	printf("triger is off!\n");
	
}
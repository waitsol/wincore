
/*



	ÄÚºËcs 0x20
	0x0040EC00 ~ 0X00201230


*/
#include <windows.h>
#include <stdio.h>

typedef int (__CRTDECL* PDebugPrint)(
	_In_z_ _Printf_format_string_ char const* const _Format,
	...);
PDebugPrint DebugPrint;
char* strx;
void _declspec(naked) callr0fun() {


	_asm {
	push fs
	push 0x30;
	pop fs
	mov eax, [strx];
	push eax
	call DebugPrint
	add esp, 4

	// int 3
	pop fs
	retf
	}

}



int main() {

	printf("rfun addr = %x\n", (long)callr0fun);
	DebugPrint = (PDebugPrint)0x83e48c60;
	//4.8.04084
	strx = (char*)malloc(300);
	memset(strx, 0, 300);
	memcpy(strx, "qwewqeqweqqqqqqqqqqqqq", strlen("qwewqeqweqqqqqqqqqqqqq"));
	//C:\Program Files\Microsoft SDKs\Windows\v7.1    eq 80b98860  0040EC00`00081000
	system("pause");
	char cmd[] = { 0,0,0,0,0x60,0 };
	_asm {

		//push 0x1234  //arg1
		call fword ptr cmd

		//asm_ret :


	}
	printf("end\n");

	system("pause");

	return 0;
}
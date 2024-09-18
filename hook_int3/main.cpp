
/*



	内核cs 0x20
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
		sub esp, 8
		lea eax, hook
		mov[esp], eax
		mov[esp + 4], 0x8
		jmp fword ptr[esp]
		hook:
		add esp, 8
			push fs
			push 0x30
			pop fs
			mov eax, [strx]
			push eax
			call DebugPrint
			add esp, 4
			pop fs
			mov eax, 0x83e54fc0
			jmp eax
	}

}


int main() {
	printf("rfun addr = %x\n", (long)callr0fun);
	DebugPrint = (PDebugPrint)0x83e21c60;
	//4.8.04084
	strx = (char*)malloc(300);
	memset(strx, 0, 300);
	memcpy(strx, "qwewqeqweqqqqqqqqqqqqq", strlen("qwewqeqweqqqqqqqqqqqqq"));
	/**
		/*
	int 3 83e7cfc0
	callr0fun = 0x 40100
	offset =7C5A C040

		eq 80b98848 7ccf9b5AC040ffff
		eq 80b98018 83e5ee00`00484fc0
		 83e54fc0


		*/

		//eq 80b98848  0040EC00`00081000  //这里跳到 上面的函数

		//
	system("pause");
	_asm {
		int 3
	}
	printf("end\n");

	system("pause");

	return 0;
}
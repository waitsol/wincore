#include <Windows.h>
#include <stdio.h>
unsigned int g_value = 0;
unsigned int p1_addr_real = 0;

__declspec(naked) void test()
{

    __asm
    {
        push 0x30;
        pop fs;
        pushad;
        pushfd;

        //0��ַ��PDEһ���Ǵ��ڵģ������ֱ�ӹ�PTE
        mov eax, 0x600000;
        //��PAEģʽ�»�ȡpte���㷨����������MmIsAddressValid�ҵ���
        shr eax, 0x9;
        and eax, 0x7ffff8;

        //find pte
        mov edx, eax;
        add edx, 0xc0000000;
        mov edx, dword ptr ds : [edx] ;

        mov dword ptr ds : [0xc0000000] , edx;  //pte
        mov[p1_addr_real], edx;

        mov dword ptr ds : [0] , 0x12345678;   //����tlb


        mov eax, 0x700000;
        shr eax, 0x9;
        and eax, 0x7ffff8;

        //find pte
        mov edx, eax;
        add edx, 0xc0000000;
        mov edx, dword ptr ds : [edx] ;
        mov dword ptr ds : [0xc0000000] , edx;

        mov eax, dword ptr ds : [0] ;
        mov g_value, eax;


        popfd;
        popad;

        retf;
    }
}


int main(int agrc, char* agrv[])
{
    char buf[] = { 0,0,0,0,0x48,0 };

    void* p1 = VirtualAlloc((void*)0x600000, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    void* p2 = VirtualAlloc((void*)0x700000, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (p1 == NULL)
    {
        if (p2 != NULL) VirtualFree(p2, 0x1000, MEM_COMMIT | MEM_RESERVE);
        printf("virtual failed p1 %d\n", GetLastError());
        return 0;
    }

    if (p2 == NULL)
    {
        VirtualFree(p1, 0x1000, MEM_COMMIT | MEM_RESERVE);
        printf("virtual failed p2 %d\n", GetLastError());
        return 0;
    }
    //�������Ҫ��ֵ����Ȼ��0xC0000005��VirtualAlloc������˿ռ䣬���ǵ���ʹ�õ�ʱ��Ź�����ҳ��
    *((unsigned int*)p1) = 0x100;
    *((unsigned int*)p2) = 0x200;
    printf("test addr  = %x\n", test);
    //eq 80b98848 0040ec00`00081000
    system("pause");
    __asm
    {
        call fword ptr buf;
        push 0x3b;
        pop fs;
    };

    printf("%x\np1 real addr = %x\n", g_value, p1_addr_real);
    system("pause");
    VirtualFree(p1, 0x1000, MEM_COMMIT | MEM_RESERVE);
    VirtualFree(p2, 0x1000, MEM_COMMIT | MEM_RESERVE);

    return 0;
}
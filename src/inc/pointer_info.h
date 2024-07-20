#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>

#ifdef _DEBUG 
	#define DPRINT(fmt, ...) printf(fmt, __VA_ARGS__)
#else
	#define DPRINT(fmt, ...)

#endif


#define GetModuleInfo(module, returnd) (K32GetModuleInformation(GetCurrentProcess(), GetModuleHandle(module), &returnd, sizeof(MODULEINFO)) ? modinfo : return)




char* rebuild_string(char* string){
	


	char* newString;


	//get number of spaces in string
	int spaces = 0;
	for (int i = 0; i < strlen(string); i++){
		if (string[i] == ' '){
			spaces++;
		}
	}

	spaces++; // add space for one at the start

	newString = (char*)malloc(strlen(string) + (spaces * 2) + 1); // allocate memory for new string

	for (int i = 0; i < strlen(string); i++){
		if (string[i] == ' '){
			strcat(newString, "\\x");
		}
		else {
			strncat(newString, &string[i], 1);
		}
	}

	DPRINT("New String: %s\n", newString);
	



	return newString;
}

uintptr_t FindPattern(HMODULE moduleBase, char* pattern, char* mask){
    MODULEINFO modinfo = { 0 };

    HMODULE phandle = moduleBase;

    DPRINT("Module: %lp\n", phandle);



    
    GetModuleInformation(GetCurrentProcess(), phandle, &modinfo, sizeof(MODULEINFO));

    uintptr_t base = (uintptr_t)modinfo.lpBaseOfDll;
    uintptr_t size = (uintptr_t)modinfo.SizeOfImage;

    DPRINT("Base: %llx\n", base);
    DPRINT("Size: %llx\n", size);

    uintptr_t patternLength = (uintptr_t)strlen(mask);

    DPRINT("Pattern Length: %lld\n", patternLength);

    for (uintptr_t i = 0; i < size - patternLength; i++){
        bool found = true;
        for (uintptr_t j = 0; j < patternLength; j++){
            found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
        }
        if (found){
            return base + i;
        }
    }
    return 0xDEADC0DE;
}

struct pointerInfo {
        

    public:

		char* offsetName;
        char* module;
        char* pattern;
        char* mask;
        uintptr_t offset;

        uintptr_t runtimeAddress;
		HMODULE moduleBase;

		uintptr_t moduleOffset;
        

        pointerInfo(char* offsetName, char* module, char* pattern, char* mask, uintptr_t offset = 0){
			this->offsetName = offsetName;
            this->module = module;
			this->moduleBase = GetModuleHandle(module);
            this->pattern = pattern;
            this->mask = mask;
            this->offset = offset;
            this->runtimeAddress = FindPattern(this->moduleBase, pattern, mask) + offset;
			this->moduleOffset = this->runtimeAddress - (uintptr_t)this->moduleBase;
        }

		void printResults(){
			printf("Offset Name: %s\n", this->offsetName);
			printf("Module: %s\n", this->module);
			printf("Module Base: %llx\n", this->moduleBase);
			printf("Runtime Address: %llx\n", this->runtimeAddress);
			printf("Module Offset: %llx\n", this->moduleOffset);

		}
};
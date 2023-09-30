#include <Windows.h>


#ifdef _DEBUG
    #include <stdio.h>
#endif

void Unload(){

    #ifdef _DEBUG
        printf("=======================================================\n");
        printf("                   DLL Uninjecting\n");
        printf("=======================================================\n");
    #endif

    //Unload all

    

}


void MainThread(){

    #ifdef _DEBUG
        printf("=======================================================\n");
        printf("                     Main Thread\n");
        printf("=======================================================\n"); 
        #endif

    while (true){
        if (GetAsyncKeyState(VK_END)){
            break;
        }


        Sleep(10);

    }
    

    Unload();
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved){

    if (dwReason == DLL_PROCESS_ATTACH){
        #ifdef _DEBUG
            AllocConsole();
            freopen("CONOUT$", "w", stdout);
            printf("\n=======================================================\n");
            printf("\n                      DLL Injected\n");
            printf("\n=======================================================\n");
        #endif
        
        DisableThreadLibraryCalls(hModule);

        HANDLE hThread = CreateThread(
                                NULL, 0,
                                reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread),
                                NULL, 0, NULL
                            );

        if (!hThread){
            CloseHandle(hThread);
        }
    }
}




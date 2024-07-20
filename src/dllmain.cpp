#include <Windows.h>
#include <pointer_info.h>
#include <vector>
#include <string.h>

#include <json.hpp>
#include <fstream>

#ifdef _DEBUG
    #include <stdio.h>
#endif



std::vector<pointerInfo> pointers;




void MainThread(HMODULE hModule){

    #ifdef _DEBUG
        printf("=======================================================\n");
        printf("                     Main Thread\n");
        printf("=======================================================\n"); 
    #endif


	std::vector<pointerInfo*> pointers;

	//open config.json

	std::ifstream file("/inc/config.json");

	if (!file.is_open()){
		DPRINT("Failed to open config.json\n");
		return;
	}

	nlohmann::json j;
	

	j = nlohmann::json::parse(file);

	for (auto& element : j["byte_signatures"]){

		
		std::string offsetName = element["offset_name"];

		std::string module = element["module"];
		std::string pattern = element["pattern"];
		std::string mask = element["mask"];

		if(!element["operations"].empty()){

			switch (element["operations"]["type"].get<int>()){

				case 1:
					//add
					pointerInfo* pInfo = new pointerInfo(
						(char*)offsetName.c_str(),
						(char*)module.c_str(),
						(char*)pattern.c_str(),
						(char*)mask.c_str(),
						element["operations"]["offset"].get<uintptr_t>()
					);

					pointers.push_back(pInfo);


					break;

			}

		
			
		} else {
			pointerInfo* pInfo = new pointerInfo(
				(char*)offsetName.c_str(),
				(char*)module.c_str(),
				(char*)pattern.c_str(),
				(char*)mask.c_str()
			);

			pointers.push_back(pInfo);
		}
	


	}



    while (true){
        if (GetAsyncKeyState(VK_END)){
            break;
        }


        Sleep(10);

    }


	for (auto pInfo : pointers){
		delete pInfo;
	}

	FreeLibraryAndExitThread(hModule, 0);
    


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
                                hModule, 0, NULL
                            );

        if (!hThread){
            CloseHandle(hThread);
        }
    }
}




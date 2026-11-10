#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

struct MemorieComuna {
    int numarator;
};

int main() {
    srand(time(NULL) ^ GetCurrentProcessId());

    HANDLE hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(MemorieComuna), "MemoriaMeaWindows");
    MemorieComuna* mem = (MemorieComuna*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MemorieComuna));

    HANDLE hSemafor = CreateSemaphoreA(NULL, 1, 1, "SemaforulMeuWindows");

    std::cout << "Procesul [" << GetCurrentProcessId() << "] a pornit!\n";

    while (true) {
        WaitForSingleObject(hSemafor, INFINITE);

        if (mem->numarator >= 1000) {
            ReleaseSemaphore(hSemafor, 1, NULL);
            break;
        }

        while ((rand() % 2 + 1) == 2 && mem->numarator < 1000) {
            mem->numarator++;
            std::cout << "Proces " << GetCurrentProcessId() << " a scris: " << mem->numarator << "\n";
        }

        ReleaseSemaphore(hSemafor, 1, NULL);
        Sleep(1);
    }

    UnmapViewOfFile(mem);
    CloseHandle(hMapFile);
    CloseHandle(hSemafor);

    std::cout << "Procesul a terminat.\n";
    system("pause");
    return 0;
}
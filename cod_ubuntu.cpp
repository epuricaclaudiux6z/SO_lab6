#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

struct MemorieComuna {
    int numarator;
};

int main() {
    srand(time(NULL) ^ getpid());

    int shm_fd = shm_open("/memoria_mea", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(MemorieComuna));
    MemorieComuna* mem = (MemorieComuna*)mmap(0, sizeof(MemorieComuna), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t* semafor = sem_open("/semaforul_meu", O_CREAT, 0666, 1);

    std::cout << "Procesul [" << getpid() << "] a pornit!\n";

    while (true) {
        sem_wait(semafor);

        if (mem->numarator >= 1000) {
            sem_post(semafor);
            break;
        }

        while ((rand() % 2 + 1) == 2 && mem->numarator < 1000) {
            mem->numarator++;
            std::cout << "Proces " << getpid() << " a scris: " << mem->numarator << "\n";
        }

        sem_post(semafor);
        usleep(1000); 
    }

    std::cout << "Procesul a terminat.\n";
    return 0;
}

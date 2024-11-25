#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 3000
#define SHM_SIZE 1024 // Adjust as needed for table size

int main() {
    int shmid;
    char *shmptr;
    char *ptr;
    int i, a;

    // Create shared memory
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach to shared memory
    shmptr = shmat(shmid, NULL, 0);
    if (shmptr == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    ptr = shmptr;

    printf("Enter ARP table entries (format: MAC IP):\n");

    // Input 3 entries for demonstration
    for (i = 0; i < 3; i++) {
        char mac[18], ip[16];
        printf("Entry %d\n", i + 1);
        printf("MAC Address: ");
        scanf("%s", mac);
        printf("IP Address: ");
        scanf("%s", ip);

        // Write entry to shared memory
        sprintf(ptr, "%-17s %-15s\n", mac, ip); // Fixed-width formatting for easier parsing
        ptr += strlen(ptr); // Move pointer forward
    }

    // Null-terminate the shared memory string
    *ptr = '\0';

    printf("\nARP table at server side:\n%s", shmptr);

    // Detach from shared memory
    shmdt(shmptr);

    return 0;
}

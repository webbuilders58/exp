#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 3000
#define SHM_SIZE 1024

int main() {
    int shmid, choice;
    char *shmptr, *ptr;
    char ip[16], mac[18], result[51];

    // Access shared memory
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
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

    printf("The ARP table is:\n%s", shmptr);

    while (1) {
        printf("\n1. ARP (IP to MAC)\n2. RARP (MAC to IP)\n3. EXIT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // ARP: IP to MAC
                printf("Enter IP address: ");
                scanf("%s", ip);
                ptr = strstr(shmptr, ip); // Search for IP in shared memory
                if (ptr) {
                    ptr -= 18; // Move back to start of corresponding MAC
                    sscanf(ptr, "%s", result);
                    printf("MAC Address: %s\n", result);
                } else {
                    printf("IP address not found.\n");
                }
                break;

            case 2:
                // RARP: MAC to IP
                printf("Enter MAC address: ");
                scanf("%s", mac);
                ptr = strstr(shmptr, mac); // Search for MAC in shared memory
                if (ptr) {
                    sscanf(ptr + 18, "%s", result); // Move forward to corresponding IP
                    printf("IP Address: %s\n", result);
                } else {
                    printf("MAC address not found.\n");
                }
                break;

            case 3:
                // Exit
                shmdt(shmptr); // Detach from shared memory
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

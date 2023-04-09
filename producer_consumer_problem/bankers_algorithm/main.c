/* Programming Project Chapter 8, 10th edition of Operating System Concepts
 * Customers request and release resources from
 * the bank. The banker will grant a request only if it leaves the system in a
 * safe state. A request that leaves the system in an unsafe state will be denied.
 * 
 * Use the command ‘RQ’ for requesting resources, ‘RL’ for releasing resources, 
 * and ‘*’ to output the values of the different data structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
/* initialized to 0 since no resources are allocated yet*/
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = { 0 };

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int is_smaller_or_equal(int a[], int b[], int length) {
    for (int i = 0; i < length; i++) {
        if (a[i] > b[i]) {
            return 0;
        }
    }
    return 1;
}

int check_safe_state() {
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];

    /* initialize work = available */
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        work[i] = available[i];
    }

    /* initialize finish[i] = False for all i*/
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        finish[i] = 0;
    }

    /* in worst case, we'll make NUMBER_OF_CUSTOMER passes*/
    for (int pass = 0; pass < NUMBER_OF_CUSTOMERS; pass++) {
        /* in each pass, we must find at least 1 process to 
        which resources can be allocated to ensure safe state 
        exists
        */
        int process_to_allocate = 0;       
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (finish[i] == 0 && is_smaller_or_equal(need[i], work, NUMBER_OF_RESOURCES)) {
                finish[i] = 1;
                process_to_allocate = 1;

                /* the found process will finish execution and 
                the allocated resources will be relinquished
                */
                for (int r = 0; r < NUMBER_OF_RESOURCES; r++) {
                    work[r] += allocation[i][r];
                }
            }
            break;
        }
        if (!process_to_allocate) {
            return 0;
        }
        
    }
    return 1;
}

int request_resources(int customer_num, int request[]) {
    /* let request be granted and check is a safe sequence 
    exists, if it exists then request permitted
    else, revert to original state.*/

    /* request should be less than or equal to available resources*/
    if (!is_smaller_or_equal(request, available, NUMBER_OF_RESOURCES)){
        return 0;
    }

    /* grant request and check for safe state*/
    for (int r = 0; r < NUMBER_OF_RESOURCES; r++) {
        available[r] -= request[r];
        allocation[customer_num][r] += request[r];
        need[customer_num][r] -= request[r];
    }

    if (!check_safe_state) {
        /* revert to original state */
        for (int r = 0; r < NUMBER_OF_RESOURCES; r++) {
            available[r] += request[r];
            allocation[customer_num][r] -= request[r];
            need[customer_num][r] += request[r];
        }
        return 0; //request not permitted
    }

    return 1; //request permitted
}

// void release_resources(int customer_num, int release[]) {
    
// }

int main(int argc, char *argv[])
{
    /* get command line arguments */
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] = atoi(argv[1 + i]);
    }

    /* initialize maximum with maximum instances of resouces
    a process will require in its lifetime.
    This is stored in a file.
    */

    FILE *fptr = fopen("max_need.txt","r"); //may need to change path
     
    if (fptr  == NULL) {
       printf("Error in opening file");
       exit(1);
    }
    int n;
    
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            fscanf(fptr, "%d,", &n);
            maximum[i][j] = n;
        }
    }
    fclose(fptr); 

    /* initialize need, will be equal to maximum since no resources are allocated yet*/
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            need[i][j] = maximum[i][j];
        }
    }

    //everything initialized now
    char command[3];
    while (1) {
        scanf("%s", command);
        //gets(command);

        if (strcmp(command, "RQ") == 0) {
            int customer_num;
            int request[NUMBER_OF_RESOURCES];

            scanf("%d", &customer_num);
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                scanf("%d", &request[i]);
            }

            if (request_resources(customer_num, request) == 1) {
                printf("Request Granted\n");
            }
            else 
                printf("Invalid request. Denied\n");
        }

        if (strcmp(command, "*") == 0) {
            printf("Available: ");
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                printf("%d ", available[i]);
            }

            printf("\n");
            printf("Maximum: \n");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                {
                    printf("%d  ", maximum[i][j]);
                }
                printf("\n");
            }

            printf("Allocation: \n");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                {
                    printf("%d  ", allocation[i][j]);
                }
                printf("\n");
            }

            printf("Need: \n");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                {
                    printf("%d  ", need[i][j]);
                }
                printf("\n");
            }
        }

        if (strcmp(command, "q") == 0) {
            break;
        }


    }
  
    return 0;
}
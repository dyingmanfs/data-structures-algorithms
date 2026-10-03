#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 100
#define MAX_SERVICE_NAME_LENGTH 50
#define MAX_COUNTRY_LENGTH 50

// DO NOT CHANGE THE DATA STRUCTURES

typedef struct {
    int subscriptionID;
    char serviceName[50];
    float serviceCharge;
    int devicesRegistered;
    char startDate[11];
    char endDate[11];
    char status[10];
    char country[MAX_COUNTRY_LENGTH];
} Subscription;


typedef struct {
    char name[MAX_NAME_LENGTH];
    int subscriptionCount;
    float subscriptionCostTotal;
    Subscription *subscriptions;
    int subscriptionCapacity;
} Customer;



//DO NOT CHANGE FUNCTION PROTOTYPES
void menu();
void printCustomers(Customer *customers, int no_of_customers);
int countCustomers(FILE *inFile);
void readSubscriptions(FILE *inFile, Customer *customers, int no_of_customers);
Customer* createHashTable(int hashTableSize);
Customer* addCustomer(Customer *hashTable, Customer addedCustomer, int *hashTableSize, int criteria);
Customer* rehash(Customer *hashTable, int *hashTableSize, int criteria);
void printTable(Customer *hashTable, int hashTableSize);
void searchTable(Customer *hashTable, int hashTableSize, char searchName[], int criteria);

//DO NOT CHANGE THE main FUNCTION
int main() {

    FILE *inFile;
    int no_of_customers = 0;
    Customer *customers;
    Customer *hashTable;

    inFile = fopen("subscriptions.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }
    no_of_customers = countCustomers(inFile);
    printf("There are %d unique customers\n", no_of_customers);

    customers = (Customer*) malloc(no_of_customers * sizeof(Customer));
    fclose(inFile);
    inFile = fopen("subscriptions.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }

    readSubscriptions(inFile, customers, no_of_customers);
    if (customers == NULL) {
        printf("\nCustomers have NOT been read successfully!\n");
        exit(1);
    }
    printCustomers(customers, no_of_customers);

    int hashTableSize = 11;
    char criteria;

    hashTable = createHashTable(hashTableSize);

    printf("Enter your hashing criteria: ");
    printf("1. Linear Probing 2. Quadratic Probing  3. Double Hashing\n");
    fflush(stdin);
    scanf("%c", &criteria);

    while (criteria != '1' && criteria != '2' && criteria != '3'){
        printf("Error! Please enter a valid hashing criteria: ");
        fflush(stdin);
        scanf("%c", &criteria);
    }
    fflush(stdin);

    int i;
    for (i = 0; i < no_of_customers; i++){
        hashTable = addCustomer(hashTable, customers[i], &hashTableSize, criteria - '0');
        printf("%s has been added to the table, the hash table now can be seen below:\n", customers[i].name);
        printTable(hashTable, hashTableSize);
        printf("\n");
    }

    char command = 0;
    int exit = 0;
    char searchName[81];
    while (exit != 1) {
        menu();
        printf("\nCommand: ");
        fflush(stdin);
        scanf("%c", &command);

        if (command == '1') {
            printf("Enter the customer's name: ");
            fflush(stdin);
            scanf("%s", searchName);
            searchTable(hashTable, hashTableSize, searchName, criteria - '0');
        }
        else if (command == '2'){
            printTable(hashTable, hashTableSize);
        }
        else if (command == '3'){
            exit = 1;
            printf("Goodbye!\n");
        }
        else{
            printf("Please enter a valid command!\n");
        }
    }


    free(customers);
    free(hashTable);

    return 0;
}

//DO NOT CHANGE THE menu FUNCTION
void menu () {
    printf("Please choose one of the following options:\n"
           "(1) Search a customer\n"
           "(2) Display hashtable\n"
           "(3) Exit\n");
}

//DO NOT CHANGE THE printCustomers FUNCTION
void printCustomers(Customer* customers, int no_of_customers) {
    printf("List of customers:\n");
    int i, j;
    for (i = 0; i < no_of_customers; ++i) {
        // Print customer details
        if (customers[i].subscriptionCount > 0) {
            printf("Name: %s, number of subscriptions = %d, total amount paid for subscriptions = %.2f, "
                   "average amount paid per subscription = %.2f\n",
                   customers[i].name, customers[i].subscriptionCount,
                   customers[i].subscriptionCostTotal, customers[i].subscriptionCostTotal / customers[i].subscriptionCount);
        } else {
            printf("Name: %s, number of subscriptions = %d, total amount paid for subscriptions = %.2f, "
                   "average amount paid per subscription = N/A\n",
                   customers[i].name, customers[i].subscriptionCount, customers[i].subscriptionCostTotal);
        }
    }
}

int countCustomers(FILE *inFile) {
    int count = 0;
    int i = 0;
    Customer name[50];  // Array of customers
    char UserName[100];
    char ServiceName[50];
    char StartDate[11];
    char EndDate[11];
    char Status[10];
    char Country[100];
    int SubscriptionID, DevicesRegistered;
    float ServiceCharge;

    // Skip the first line
    char buffer[500];
    if (fgets(buffer, sizeof(buffer), inFile) == NULL) {
        printf("Error\n");
        return 0;
    }

    while (fscanf(inFile, "%99[^;];%d;%49[^;];%f;%d;%10[^;];%10[^;];%9[^;];%99[^\n]\n",
                  UserName, &SubscriptionID, ServiceName, &ServiceCharge, &DevicesRegistered,
                  StartDate, EndDate, Status, Country) != EOF) {

        int same = 0;

        // Check for same customer names
        for (i = 0; i < count; i++) {
            if (strcmp(name[i].name, UserName) == 0) {
                same = 1;
                break;  // No need to check further, a match is found
            }
        }

        // If the customer is not a duplicate, add to the list
        if (same == 0) {
            if (count < 50) {  // Ensure we don't overflow the array
                strcpy(name[count].name, UserName);
                count++;
            } else {
                printf("Customer array is full.\n");
                break;  // Exit if the array is full
            }
        }
    }

    return count;
}

Subscription* resub(Subscription *subscriptiontable, int subscriptionTableSize) {
    printf(" subscriptionTableSize is %d\n", subscriptionTableSize);

    // Allocate new memory
    Subscription *newsubscriptiontableTable = (Subscription *)malloc(subscriptionTableSize * sizeof(Subscription));
    if (newsubscriptiontableTable == NULL) {
        printf("Memory allocation failed.\n");
        return NULL; // Handle allocation failure
    }

    // Copy existing subscriptions to the new table
    for (int i = 0; i < subscriptionTableSize; i++) {
        newsubscriptiontableTable[i].subscriptionID = subscriptiontable[i].subscriptionID;
        strcpy(newsubscriptiontableTable[i].serviceName, subscriptiontable[i].serviceName);
        newsubscriptiontableTable[i].serviceCharge = subscriptiontable[i].serviceCharge;
        newsubscriptiontableTable[i].devicesRegistered = subscriptiontable[i].devicesRegistered;
        strcpy(newsubscriptiontableTable[i].startDate, subscriptiontable[i].startDate);
        strcpy(newsubscriptiontableTable[i].endDate, subscriptiontable[i].endDate);
        strcpy(newsubscriptiontableTable[i].status, subscriptiontable[i].status);
        strcpy(newsubscriptiontableTable[i].country, subscriptiontable[i].country);
    }

    // Free the old memory and update the table size
    free(subscriptiontable);

    return newsubscriptiontableTable;
}


void readSubscriptions(FILE* inFile, Customer* customers, int no_of_customers) {
    char UserName[100];
    char ServiceName[50];
    char StartDate[11];
    char EndDate[11];
    char Status[10];
    char Country[100];
    int SubscriptionID, DevicesRegistered;
    float ServiceCharge;

    // Skip the first line
    char buffer[500];
    if (fgets(buffer, sizeof(buffer), inFile) == NULL) {
        printf("Error\n");
        return;
    }

    int i = 0;  // Customer index
    while (fscanf(inFile, "%99[^;];%d;%49[^;];%f;%d;%10[^;];%10[^;];%9[^;];%99[^\n]\n",UserName, &SubscriptionID, ServiceName, &ServiceCharge,&DevicesRegistered, StartDate, EndDate, Status, Country) != EOF) {


        int same = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(UserName, customers[j].name) == 0) {
                customers[j].subscriptionCount++;
                if(customers[j].subscriptionCount>customers[j].subscriptionCapacity-1){
                    customers[j].subscriptionCapacity = customers[j].subscriptionCapacity*2;
                    printf("Capacity is %d\n",customers[j].subscriptionCapacity);
                    customers[j].subscriptions = resub(customers[j].subscriptions, customers[j].subscriptionCapacity);
                }
                customers[j].subscriptionCostTotal += ServiceCharge;
                customers[j].subscriptionCapacity += DevicesRegistered;
                customers[j].subscriptions[customers[j].subscriptionCount-1].devicesRegistered = DevicesRegistered;
                customers[j].subscriptions[customers[j].subscriptionCount-1].subscriptionID = SubscriptionID;
                customers[j].subscriptions[customers[j].subscriptionCount-1].serviceCharge = ServiceCharge;
                strcpy(customers[j].subscriptions[customers[j].subscriptionCount-1].serviceName,ServiceName);
                strcpy(customers[j].subscriptions[customers[j].subscriptionCount-1].startDate,StartDate);
                strcpy(customers[j].subscriptions[customers[j].subscriptionCount-1].endDate,EndDate);
                strcpy(customers[j].subscriptions[customers[j].subscriptionCount-1].status,Status);
                strcpy(customers[j].subscriptions[customers[j].subscriptionCount-1].country,Country);
                same = 1;
                break;
            }
        }
        if (!same && i < no_of_customers) {
            customers[i].subscriptionCapacity = 10;
            customers[i].subscriptions = (Subscription *) malloc(customers[i].subscriptionCapacity * sizeof(Subscription));
            if (customers[i].subscriptions == NULL) {
                printf("Memory allocation failed for subscriptions.\n");
            }
            strcpy(customers[i].name,UserName);
            customers[i].subscriptionCount = 1;
            customers[i].subscriptionCostTotal = ServiceCharge;
            customers[i].subscriptions[0].devicesRegistered = DevicesRegistered;
            customers[i].subscriptions[0].subscriptionID = SubscriptionID;
            customers[i].subscriptions[0].serviceCharge = ServiceCharge;
            strcpy(customers[i].subscriptions[0].serviceName,ServiceName);
            strcpy(customers[i].subscriptions[0].startDate,StartDate);
            strcpy(customers[i].subscriptions[0].endDate,EndDate);
            strcpy(customers[i].subscriptions[0].status,Status);
            strcpy(customers[i].subscriptions[0].country,Country);
            i++;
        }
    }
}





Customer* createHashTable(int hashTableSize) {

    Customer *customerhashtable = (Customer*) malloc(hashTableSize * sizeof(Customer));
    if (customerhashtable == NULL) {
        printf("Memory allocation failed for hash table.\n");
        return NULL;  // Return NULL if allocation fails
    }
    int i = 0;
    while (i<hashTableSize){
        strcpy(customerhashtable[i].name,"unassigned");
        i++;
    }
    return customerhashtable;
}
float numberofcustomer(Customer *hashTable,int *hashTableSize){
    float count = 0;
    int i = 0;
    while (i<*hashTableSize){
        if(strcmp(hashTable[i].name, "unassigned") != 0){
            count++;
        }
        i++;
    }
    return count;
};
Customer* addCustomer(Customer *hashTable, Customer addedCustomer, int *hashTableSize, int criteria) {
    int key = 0;
    int i=0;
    // calculte cumber of customer
    float count = numberofcustomer(hashTable,hashTableSize);
    count++;

    float loadFactor = count / *hashTableSize;

    if (loadFactor > 0.5) {
        hashTable = rehash(hashTable, hashTableSize, criteria);
    }
    for (i = 0; i < strlen(addedCustomer.name); i++) {
        key ^=  addedCustomer.name[i];
    }
    if(criteria==1){
    int hashindex = key % (*hashTableSize);
    int hashindex2 = hashindex;

         i = 1;
        while(i< *hashTableSize){
    if(strcmp(hashTable[hashindex].name, "unassigned") == 0){
        hashTable[hashindex] = addedCustomer;
        return hashTable;
        }
    else{
        hashindex =  (hashindex2 % (*hashTableSize) + i) % (*hashTableSize);
    }
      i++;
    }
    }
    else if(criteria==2){
        int hashindex = key % (*hashTableSize);
        int hashindex2 = hashindex;
        i = 0;

        while(i< *hashTableSize){
            if( strcmp(hashTable[hashindex].name, "unassigned") == 0){
                hashTable[hashindex] = addedCustomer;
                return hashTable;
            }
            else{
                hashindex =  (hashindex2 + (i*i)) % (*hashTableSize);
            }
            i++;
        }
    }
    else if(criteria==3){
        int hashindex = key % (*hashTableSize);
        int hashindex2 = hashindex;
        i = 0;
        while(i< *hashTableSize){
            if( strcmp(hashTable[hashindex].name, "unassigned") == 0){
                hashTable[hashindex] = addedCustomer;
                return hashTable;
            }
            else{
                hashindex =  (hashindex2 + (i * (7 - (key % 7)))) % (*hashTableSize);
            }
            i++;
        }
    }

}

int prime(int tablesize){
// check the prime number
    for (int i = 2; i <= tablesize / 2; i++) {
        if (tablesize % i == 0) {
            //it is not prime number retun 0
            return 0;
        }

    }
    //prime number retun 1
    return 1;
}
Customer* rehash(Customer *hashTable, int *hashTableSize, int criteria) {
    int newhashTableSize = (*hashTableSize) * 2;
    printf(" newhashTableSize is %d\n",newhashTableSize);
    int primenum = 0;



    while (!prime(newhashTableSize)){
        newhashTableSize++;
    }
    Customer *newhashTable = createHashTable(newhashTableSize);
    printTable( newhashTable,  *hashTableSize);
    for (int i = 0; i < *hashTableSize; i++) {
        if (strcmp(hashTable[i].name, "unassigned") != 0) { // Check if slot is empty
            newhashTable = addCustomer(newhashTable, hashTable[i], &newhashTableSize, criteria);
        }
    }
    free(hashTable);
    *hashTableSize = newhashTableSize;
    return newhashTable;


}


void printTable(Customer *hashTable, int hashTableSize) {
    printf("Index         Name     Total Subscriptions      TotalCost\n");
    int i = 0;
    while (i<hashTableSize){
        if(strcmp(hashTable[i].name, "unassigned")==0){
            printf("%d\n",i);
        }
        else{
            printf("%-13d %-25s %-23d %-10.2f\n", i, hashTable[i].name,hashTable[i].subscriptionCount, hashTable[i].subscriptionCostTotal);
        }
        i++;
    }

}


void searchTable(Customer *hashTable, int hashTableSize, char searchName[], int criteria) {
    int key = 0;
    int i=0;
    for (i = 0; i < strlen(searchName); i++) {
        key ^= searchName[i];
    }
    if(criteria == 1){
        int hashindex = key % hashTableSize;
        i = 0;
        while(i<hashTableSize){
            if(strcmp(hashTable[hashindex].name,searchName)==0){
                printf("Customer found: %s \n",hashTable[hashindex].name);
                printf("More information about customer: \n");
                printf("Number of subscriptions:  %d\n",hashTable[hashindex].subscriptionCount);
                printf("Total cost:  %f\n",hashTable[hashindex].subscriptionCostTotal);
                printf("\nSubscriptions:\n");
                    for (int j = 0; j < hashTable[hashindex].subscriptionCount; j++) {  // Iterate through all subscriptions
                        printf("Subscription %d:\n", j + 1);
                        printf("Subscription ID: %d\n"
                               "Service Name: %s\n"
                               "Service Charge: %f\n"
                               "Devices Registered: %d\n"
                               "Start Date: %s\n"
                               "End Date: %s\n"
                               "Status: %s\n",
                               hashTable[hashindex].subscriptions[j].subscriptionID,
                               hashTable[hashindex].subscriptions[j].serviceName,
                               hashTable[hashindex].subscriptions[j].serviceCharge,
                               hashTable[hashindex].subscriptions[j].devicesRegistered,
                               hashTable[hashindex].subscriptions[j].startDate,
                               hashTable[hashindex].subscriptions[j].endDate,
                               hashTable[hashindex].subscriptions[j].status);
                        printf("\n");
                    }
                break;
                }
            i++;
            hashindex = (key+i) % hashTableSize;
        }
        if(i==hashTableSize){
            printf("Customer not found.\n");
        }
    }

    if(criteria == 2){
        int hashindex = key % hashTableSize;
        i = 0;
        int found = 0;
        while(i<hashTableSize){
            if(strcmp(hashTable[hashindex].name,searchName)==0){
                printf("Customer found: %s \n",hashTable[hashindex].name);
                printf("More information about customer: \n");
                printf("Number of subscriptions:  %d\n",hashTable[hashindex].subscriptionCount);
                printf("Total cost:  %f\n",hashTable[hashindex].subscriptionCostTotal);
                printf("\nSubscriptions:\n");
                for (int j = 0; j < hashTable[hashindex].subscriptionCount; j++) {  // Iterate through all subscriptions
                    printf("Subscription %d:\n", j + 1);
                    printf("Subscription ID: %d\n"
                           "Service Name: %s\n"
                           "Service Charge: %f\n"
                           "Devices Registered: %d\n"
                           "Start Date: %s\n"
                           "End Date: %s\n"
                           "Status: %s\n",
                           hashTable[hashindex].subscriptions[j].subscriptionID,
                           hashTable[hashindex].subscriptions[j].serviceName,
                           hashTable[hashindex].subscriptions[j].serviceCharge,
                           hashTable[hashindex].subscriptions[j].devicesRegistered,
                           hashTable[hashindex].subscriptions[j].startDate,
                           hashTable[hashindex].subscriptions[j].endDate,
                           hashTable[hashindex].subscriptions[j].status);
                    printf("\n");
                }
                break;
            }
            i++;
            hashindex = (key+i*i) % hashTableSize;
        }
        if(i==hashTableSize){
            printf("Customer not found.\n");
        }
    }
    if(criteria == 3){
        int hashindex = key % hashTableSize;
        i = 0;
        int found = 0;
        while(i<hashTableSize){
            if(strcmp(hashTable[hashindex].name,searchName)==0){
                printf("Customer found: %s \n",hashTable[hashindex].name);
                printf("More information about customer: \n");
                printf("Number of subscriptions:  %d\n",hashTable[hashindex].subscriptionCount);
                printf("Total cost:  %f\n",hashTable[hashindex].subscriptionCostTotal);
                printf("\nSubscriptions:\n");
                for (int j = 0; j < hashTable[hashindex].subscriptionCount; j++) {  // Iterate through all subscriptions
                    printf("Subscription %d:\n", j + 1);
                    printf("Subscription ID: %d\n"
                           "Service Name: %s\n"
                           "Service Charge: %f\n"
                           "Devices Registered: %d\n"
                           "Start Date: %s\n"
                           "End Date: %s\n"
                           "Status: %s\n",
                           hashTable[hashindex].subscriptions[j].subscriptionID,
                           hashTable[hashindex].subscriptions[j].serviceName,
                           hashTable[hashindex].subscriptions[j].serviceCharge,
                           hashTable[hashindex].subscriptions[j].devicesRegistered,
                           hashTable[hashindex].subscriptions[j].startDate,
                           hashTable[hashindex].subscriptions[j].endDate,
                           hashTable[hashindex].subscriptions[j].status);
                    printf("\n");
                }
                break;
            }
            i++;
            hashindex =  (key + (i * (7 - (key % 7)))) % (hashTableSize);
        }
        if(i==hashTableSize){
            printf("Customer not found.\n");
        }
    }
}

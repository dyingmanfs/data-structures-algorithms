#include <stdio.h>
#include "stdlib.h"
#include "string.h"


struct graphArc{
    int weight;
    struct graphVertex *destination;
    struct graphArc *next;
}Arc;


typedef struct graphVertex {
    struct graphVertex *next;
    char warehouseName[50];
    int inDegree;
    int outDegree;
    int processed;
    struct graphArc *firstArc;
} Vertex;


typedef struct graphHead {
    int count;
    struct graphVertex *first;
} Graph;

struct graphHead * createGraph(void);
void insertVertex(struct graphHead *head, char *data);
int insertArc(struct graphHead *head, char *fromKey, char *toKey ,int weight);
void printAllArcOfVertex(struct  graphVertex *graphVertexName);
void printAllVertex(struct graphHead *graphName);
void printAllVertexWithRoutes(struct graphHead *graphName);
void readRoutes(char *fileName,struct graphHead *warehouseGraph);
void readWarehouseNames(char *fileName, struct graphHead *warehouseGraph);
void getMostDeliveries(struct graphHead *warehouseGraph);
void sendMostDeliveries(struct graphHead *warehouseGraph);
void routesWithHighestDistance(struct graphHead *warehouseGraph);
void routesWithLowestDistance(struct graphHead *warehouseGraph);
int findPath(struct graphVertex *currentVertex, struct graphVertex *destinationVertex, int *totalDistance);
void checkDistance(struct graphHead *warehouseGraph, char *departure, char *destination);

int main (int argc, char *argv[]){
    struct graphHead *warehouseGraph = createGraph();
    printf("name 1 %s name 2%s\n",argv[1], argv[2]);
    readWarehouseNames("WarehouseLocations.txt", warehouseGraph);
    readRoutes("WarehouseRoutes.txt", warehouseGraph);

    printf("\nWarehouse and route data read successfully.\n");

   printf("\nRead warehouse locations:\n");
    printAllVertex(warehouseGraph);

    printf("\nRead routes:\n");
    printAllVertexWithRoutes(warehouseGraph);

    printf("\n");
    getMostDeliveries(warehouseGraph);

    printf("\n");
    sendMostDeliveries(warehouseGraph);

    printf("\n");
    routesWithHighestDistance(warehouseGraph);

    printf("\n");
    routesWithLowestDistance(warehouseGraph);

    printf("\n");
    if(argc > 1){
        checkDistance(warehouseGraph, argv[1], argv[2]);
    }
}

struct graphHead * createGraph(void){
    struct graphHead * head = (struct graphHead *)malloc(sizeof(struct graphHead));
    head->count = 0;
    head->first = NULL;
    return head;
}

void insertVertex(struct graphHead *head, char *data){
    struct graphVertex *vertex = (struct graphVertex *)malloc(sizeof(struct graphVertex));
    vertex->next = NULL;
    strcpy(vertex->warehouseName, data);
    vertex->inDegree = 0;
    vertex->outDegree = 0;
    vertex->firstArc = NULL;
    vertex->processed = 0;
    head->count += 1;
    if(head->first == NULL){
        head->first = vertex;
    }
    else{
        struct graphVertex *temp = head->first;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = vertex;
    }
}

int insertArc(struct graphHead *head, char *fromKey, char *toKey ,int weight){
struct graphVertex *from = head->first;
struct graphVertex *to = head->first;
struct graphVertex *tmp = head->first; //  I created temporary struct
int controlform = 0; // to check if find, it will be 1
int controlto = 0; // to check if find, it will be 1
    while (tmp != NULL){
        if (strcmp(tmp->warehouseName, fromKey) == 0) {
            from = tmp;
            controlform =1; // find set 1
        }
        if (strcmp(tmp->warehouseName, toKey) == 0) {
            to = tmp;
            controlto = 1; // find set 1
        }
        tmp = tmp->next;
    }
    if(controlform==0||controlto==0){
        printf("it doen't find from is %s to is %s\n",fromKey,toKey); // if doen't find it will retun 0

        return 0;
    }
    else{
        struct graphArc *tmpArc = (struct graphArc *)malloc(sizeof(struct graphArc)); // I created temporary struct for arc
        if (tmpArc == NULL) {
            printf("Memory allocation failed\n");
            return 0;
        }
        tmpArc->weight = weight;
        tmpArc->destination = to;
        tmpArc->next = NULL;
        if(from->firstArc == NULL){ // if doen't have any arc
            from->firstArc = tmpArc;
        }
        else{
            struct graphArc *tmpcurrent = from->firstArc;
            while (tmpcurrent->next != NULL){
                tmpcurrent = tmpcurrent->next; // to add end of arc
            }
            tmpcurrent->next = tmpArc;
        }
        from->outDegree++;
        to->inDegree++;
        return 1; // if we can add arc retun 1

    }


}

void printAllVertex(struct graphHead *graphName){
struct graphVertex *tmp = graphName->first; // I created temporary vertex struct
    while (tmp!=NULL){
        // The loop will continue until tmp becomes null
        printf("%s\n",tmp->warehouseName);
        tmp=tmp->next;
    }
}

void printAllArcOfVertex(struct  graphVertex *graphVertexName){


    struct graphVertex *tmp = graphVertexName;// I created temporary vertex struct


        struct graphArc *arctmp = tmp->firstArc;// I created temporary  arc struct

        if (arctmp == NULL) {
            // if arc is NUll doesn't write any things

        } else {
            printf("\n %s", tmp->warehouseName);
            //  The loop will continue until tmp becomes null

            while (arctmp != NULL) {
                printf(" | %s | ", arctmp->destination->warehouseName);
                arctmp = arctmp->next;
            }
        }

    }

void printAllVertexWithRoutes(struct graphHead *graphName){


    struct graphVertex *tmp = graphName->first; // I created temporary vertex struct
    //  The loop will continue until tmp becomes null
    while (tmp != NULL) {

        struct graphArc *arctmp = tmp->firstArc; // I created temporary arc struct


        if (arctmp == NULL) {
           // if arc is NUll doesn't write any things
        } else {
            printf("\n %s", tmp->warehouseName);
            //  The loop will continue until tmp becomes null
            while (arctmp != NULL) {
                printf(" | %d -> %s | ",  arctmp->weight, arctmp->destination->warehouseName);
                arctmp = arctmp->next;
            }
        }

        tmp = tmp->next;
    }}

void readWarehouseNames(char *fileName, struct graphHead *warehouseGraph){
    //To be completed
    FILE *file = fopen(fileName, "r"); // I opened file with fileName to read
    if (file == NULL) {
        printf("Can't open file %s\n", fileName);
        return;
    }

    char name[100];
    while (fscanf(file, "%99[^\n]\n", name) != EOF) {
        insertVertex(warehouseGraph, name); // copy name
    }

    fclose(file);// in end closed file
}

void readRoutes(char *fileName,struct graphHead *warehouseGraph) {
    FILE *file = fopen(fileName, "r");
    char line[100];
    char fromkey[100];
    char tokey[100];
    int weight;
    while (fgets(line, 100, file)) {
    int head = 1;
    int flag = 1;
    char *token;
        // Tokenize the line using ';' as the delimiter

        token = strtok(line,";");
        while (token!=NULL) {
            if(head == 1){
                // The first token is the source key ("fromkey")
                strcpy(fromkey,token);
                head = 0;
                // Reset head after reading the first token
            }
            else {
                // Check and remove the newline character if it exists at the end of the token
                if (token[strlen(token) - 1] == '\n')
                    token[strlen(token) - 1] = '\0';
                // first flag is integer
                if(flag==1){
                    weight = atoi(token);

                }
                // second flag is char
                if(flag==2){
                    strcpy(tokey,token);
                    insertArc(warehouseGraph,fromkey,tokey,weight);
                    flag =0;
                }
                flag++;

            }
            // Get the next token in the line
            token = strtok(NULL, ";");

        }
    }
}

void getMostDeliveries(struct graphHead *warehouseGraph){
int max = 0;
struct graphVertex *tmp = warehouseGraph->first;
    struct graphVertex *tmp2 = warehouseGraph->first;
    // First pass: Find the maximum in-degree of all warehouses

while(tmp!=NULL){
    // Update max if the current warehouse has a higher in-degree
    if(tmp->inDegree>max){
        max = tmp->inDegree;
    }
    tmp = tmp->next;
}
    // Print the maximum in-degree value
    // Second pass: Identify and print all warehouses with the maximum in-degree

    printf("\nWarehouses that receives deliveries from the most different locations (%d)\n",max);
    while (tmp2!=NULL){
        if(max==tmp2->inDegree){
            printf("- %s\n",tmp2->warehouseName);
        }
        tmp2 = tmp2->next;
    }
}

void sendMostDeliveries(struct graphHead *warehouseGraph){
    int max = 0;
    struct graphVertex *tmp = warehouseGraph->first;
    struct graphVertex *tmp2 = warehouseGraph->first;
    // First pass: Find the maximum out-degree of all warehouses
    while(tmp!=NULL){
        // Update max if the current warehouse has a higher out-degree
        if(tmp->outDegree>max){
            max = tmp->outDegree;
        }
        tmp = tmp->next;
    }
    // Print the maximum out-degree value
    // Second pass: Identify and print all warehouses with the maximum out-degree
    printf("\nWarehouses that receives deliveries from the most different locations (%d)\n",max);
    while (tmp2!=NULL){
        if(max==tmp2->outDegree){
            printf("- %s\n",tmp2->warehouseName);
        }
        tmp2 = tmp2->next;
    }
}

void routesWithHighestDistance(struct graphHead *warehouseGraph) {
    char name[100];
    char name2[100];
    int max = 0;
    struct graphVertex *tmp = warehouseGraph->first;

    while (tmp != NULL) {
        struct graphArc *arcTmp = tmp->firstArc; // I created a temporary pointer for arcs

        while (arcTmp != NULL) {
            if (arcTmp->weight > max) {
                max = arcTmp->weight;
                strcpy(name2, tmp->warehouseName); // copy the  warehouse name
                strcpy(name, arcTmp->destination->warehouseName); // copy the destination warehouse name
            }
            arcTmp = arcTmp->next;
        }
        tmp = tmp->next;
    }

    printf("\nRoutes with the highest distance (%d):\n", max);
    printf("\n%s %d->%s", name2, max, name);
}

void routesWithLowestDistance(struct graphHead *warehouseGraph) {
    char name[100];
    char name2[100];
    strcpy(name2, "empty");
    strcpy(name, "empty");
    struct graphVertex *tmp = warehouseGraph->first;
    int min = tmp->firstArc->weight;
    printf("\n min is %d\n",min);
    while (tmp != NULL) {
        struct graphArc *tmpArc = tmp->firstArc; // I  created temp pointer
        while (tmpArc != NULL) {
            if (tmpArc->weight < min) {
                min = tmpArc->weight;
                strcpy(name2, tmp->warehouseName);// copy the  warehouse name
                strcpy(name, tmpArc->destination->warehouseName);// copy the destination warehouse name
            }
            tmpArc = tmpArc->next;
        }
        tmp = tmp->next;
    }

    printf("\nRoutes with the lowest distance (%d):\n", min);
    printf("\n%s %d->%s\n",name2,min,name);
}

int findPath(struct graphVertex *currentVertex, struct graphVertex *destinationVertex, int *totalDistance) {
    if (strcmp(currentVertex->warehouseName,destinationVertex->warehouseName)==0) {
        printf("%s", currentVertex->warehouseName);
        // end of recusvie we find
        return 1;
    }

    currentVertex->processed = 1; //  vertex  visited

    struct graphArc *tmparc = currentVertex->firstArc;
    while (tmparc != NULL) {
        if (!tmparc->destination->processed) {
            int distance = *totalDistance + tmparc->weight; // add arc distance
            if (findPath(tmparc->destination, destinationVertex, &distance)) {
                *totalDistance = distance;
                printf(" <-%d- %s", tmparc->weight, currentVertex->warehouseName);
                return 1; // Path found, so return 1
            }
        }
        tmparc = tmparc->next; // Move to the next arc
    }
    return 0;// we don't find path so retun 0
}

void checkDistance(struct graphHead *warehouseGraph, char *departure, char *destination) {
    struct graphVertex *departurevertex = warehouseGraph->first;
    struct graphVertex *destinationvertex = warehouseGraph->first;
    struct graphVertex *tmp = warehouseGraph->first;

    while (tmp != NULL) {
        if (strcmp(tmp->warehouseName, departure) == 0) {
            // Found the departure vertex

            departurevertex = tmp;
        }
        if (strcmp(tmp->warehouseName, destination) == 0) {
            // Found the destination vertex

            destinationvertex = tmp;
        }
        tmp = tmp->next;
    }

    int totalDistance = 0;
    // Inform the user that the search for a route has started

    printf("Searching for a route from %s to %s...\n", departure, destination);
    // Check if a path exists between the departure and destination vertices

    if (findPath(departurevertex, destinationvertex, &totalDistance)) {
        printf("\nTotal Distance: %d\n", totalDistance);
    } else {
        // No path found, inform the user
        printf("there is no path found between %s and %s.\n", departure, destination);
    }


}
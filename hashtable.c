#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_TABLE_SIZE 7
#define MAX_KEY_SIZE 10
#define MAX_HASH_SIZE 20

typedef struct {
    char* key;
    int value;
}Item;

typedef struct {
    Item* tableArray;
    int count;
    int capacity;
} HashTable;

//int count = 0;
// int capacity = INITIAL_TABLE_SIZE;
// Item *HashTable[INITIAL_TABLE_SIZE] = {NULL};

// Item** init_hash_table(){
//     //Item** table = malloc(sizeof(Item) * capacity);
//     Item** table = calloc(capacity, sizeof(Item));
//     return table;
// }

HashTable* init_hashtable(){
    HashTable *table;
    HashTable* error = NULL;
    if (!(table = malloc(sizeof(HashTable))))
    {
        printf("Error allocating memory for Hash Table");
        return error;
    }

    if(!(table->tableArray = calloc(sizeof(Item), INITIAL_TABLE_SIZE))){
        printf("Error allocating memory for Hash Table array");
        free(table);
        return error;
    }
    table->capacity = INITIAL_TABLE_SIZE;
    table->count = 0;
    return table;
}

void free_hashtable(HashTable* table){
    free(table->tableArray);
    free(table);
}

void insert(HashTable* table, char* key, int value);
int resize_table(HashTable* table);
int isPrime(int n);
int nextPrime(int n);
int create_hash(char* key);

// Item createItem(char* key, int value)
// {
//     Item item = malloc(sizeof(Item));
//     strcpy(item->key, key);
//     item->value = value;
//     return item;
// }

void insert(HashTable* table, char* key, int value){
    int hash = create_hash(key);
    int index = hash % table->capacity;
    Item entry = {.key = key, .value = value};
    // Item *item = createItem(key, value);
    if((table->tableArray + index)->key == NULL){
        table->tableArray[index] = entry;
        table->count++;
    }
    else if((table->tableArray + index)->key == key){
        (table->tableArray + index)->value = entry.value;
    }
    else if((table->tableArray + index)->key != NULL){
        int found = 0; // begin quadratic probing
        int i = 1;
        while (!found){
            index = (index + (i * i)) % table->capacity;
            if((table->tableArray + index)->key == NULL){
                table->tableArray[index] = entry;
                table->count++;
                found = 1;
            }                                  
            i++;
        }
        }
    else{
        printf("Error inserting into table.");
    }
    double loadfactor = ((double)table->count / table->capacity);
    if (loadfactor > 0.5)
    { // check load factor
        resize_table(table);
    }
}

int resize_table(HashTable* table){
    //Item* items = malloc(sizeof(Item) * count);
    Item items[table->count];
    int newCapacity = nextPrime(table->capacity * 2);
    int itemsIndex = 0;
    Item empty = {.key = NULL, .value = 0};
    for (int i = 0; i < table->capacity; i++)
    {
        if ((table->tableArray + i)->key != NULL)
        {
            items[itemsIndex] = table->tableArray[itemsIndex];
            table->tableArray[i] = empty;
            itemsIndex++;
        }
    }
    //free(HashTable);
    if(!(table->tableArray = realloc(table->tableArray, newCapacity * sizeof(Item)))){
        printf("Error reallocating Hash Table array memory for resizing");
        free(table->tableArray);
        free(table);
        return -1;
    }
    table->count = 0;
    table->capacity = newCapacity;
    for (int i = 0; i < itemsIndex; i++)
    {
        insert(table, items[i].key, items[i].value);
    }
    //free(items);
    return 1;
}

int retrieve(HashTable* table, char* key){
    int hash = create_hash(key);
    int index = hash % table->capacity;
    int found = 0;
    int i = 0;
    Item curItem = table->tableArray[index];
    while (curItem.key != NULL){
        //int tableKey = HashTable[index + (i * i)]->key;
        if (!strcmp(curItem.key, key))
        {
            return curItem.value;
        }
        i++;
        curItem = table->tableArray[((index + (i * i)) % table->capacity)];
    }
    return -1;
}

int isPrime(int n){
    if(n % 2 == 0 || n % 3 == 0){
        return 0;
    }
    else{
        for (int i = 2; i * i <= n; i++){
            if(n % i == 0){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int nextPrime(int n){
    if(n <= 1){
        return 2;
    }
    if(isPrime(n)){
        return n;
    }

    int found = 0;
    int prime = n;

    while(!found){
        prime++;
        if(isPrime(prime)){
            found = 1;
            return prime;
        }
    }
    return 0;
}

int create_hash(char key[]){
    int hash = 0;
    int min = strlen(key) / sizeof(key[0]);
    if(min > 8){
        min = 8;
    }
    for (int i = 0; i < min; i++){
        hash += (int)key[i] * pow(31, min - 1 - i);
    }
    return hash;
}



int main(){
    HashTable* table = init_hashtable();
    insert(table, "hello", 3);
    insert(table, "hola", 4);
    insert(table, "bonjour", 5);
    insert(table, "ciao", 6);
    printf("%d",retrieve(table, "hello"));
    free_hashtable(table);
    //printf("Hello");
    // int returned_key = retrieve(table, "hello");
}

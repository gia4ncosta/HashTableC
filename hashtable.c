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

int count = 0;
int capacity = INITIAL_TABLE_SIZE;
// Item *HashTable[INITIAL_TABLE_SIZE] = {NULL};

Item** init_hash_table(){
    //Item** table = malloc(sizeof(Item) * capacity);
    Item** table = calloc(capacity, sizeof(Item));
    return table;
}

void insert(Item **HashTable, char key[], int value);
void resize_table(Item **HashTable);
int isPrime(int n);
int nextPrime(int n);
int create_hash(char key[]);

Item* createItem(char key[], int value)
{
    Item* item = malloc(sizeof(Item));
    strcpy(item->key, key);
    item->value = value;
    return item;
}

void insert(Item** HashTable, char key[], int value){
    int hash = create_hash(key);
    int index = hash % capacity;
    Item *item = createItem(key, value);
    if(HashTable[index] == NULL){
        HashTable[index] = item;
        count++;
    }
    else if(HashTable[index]->key == key){
        HashTable[index]->value = item->value;
    }
    else if(HashTable[index] != NULL){
        int found = 0; // begin quadratic probing
        int i = 1;
        while (!found){
            index = index + (i * i);
            if(HashTable[index] == NULL){
                HashTable[index] = item;
                count++;
                found = 1;
            }
            i++;
        }
        }
    else{
        printf("Error inserting into table.");
    }
    double loadfactor = ((double)count / capacity);
    if (loadfactor > 0.5)
    { // check load factor
        resize_table(HashTable);
    }
}

void resize_table(Item** HashTable){
    Item** items = malloc(sizeof(Item) * count);
    int newCapacity = nextPrime(capacity * 2);
    int itemsIndex = 0;
    for (int i = 0; i < capacity; i++)
    {
        if (HashTable[i] != NULL)
        {
            items[itemsIndex] = HashTable[itemsIndex];
            HashTable[i] = NULL;
            itemsIndex++;
        }
    }
    //free(HashTable);
    HashTable = realloc(HashTable, newCapacity);
    count = 0;
    capacity = newCapacity;
    for (int i = 0; i < itemsIndex - 1; i++)
    {
        insert(HashTable, items[i]->key, items[i]->value);
    }
    free(items);
}

int retrieve(Item** HashTable, char* key){
    int hash = create_hash(key);
    int index = hash % capacity;
    int found = 0;
    int i = 0;
    Item* curItem = HashTable[index];
    while (curItem != NULL){
        //int tableKey = HashTable[index + (i * i)]->key;
        if (!strcmp(curItem->key, key))
        {
            return curItem->value;
        }
        ++i;
        curItem = HashTable[((index + (i * i)) % capacity)];
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
    Item** table = init_hash_table();
    insert(table, "hello", 3);
    insert(table, "hola", 4);
    insert(table, "bonjour", 5);
    insert(table, "ciao", 6);
    //int returned_key = retrieve(table, "hello");
}

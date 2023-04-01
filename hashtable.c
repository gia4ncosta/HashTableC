#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_TABLE_SIZE 7

typedef struct {
    char* key;
    int value;
}Item;

typedef struct {
    Item* tableArray;
    int count;
    int capacity;
} HashTable;

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


void insert(HashTable* table, char* key, int value){
    int hash = create_hash(key);
    int index = hash % table->capacity;
    Item entry = {.key = key, .value = value}; // creating entry to be inserted
    if((table->tableArray + index)->key == NULL){ //checking if indexed spot is empty
        table->tableArray[index] = entry;
        table->count++;
    }
  
    else if((table->tableArray + index)->key != NULL){ //begin quadratic probing
        //also checks if current entry at index has the same key as the passed key
        int found = 0; 
        int i = 0;
        int newIndex;
        while (!found)
        {
            newIndex = (index + (i * i)) % table->capacity; 
            if((table->tableArray + newIndex)->key == NULL){ //checking if empty spot has been found
                table->tableArray[newIndex] = entry;
                table->count++;
                found = 1;
            }
            else if(!strcmp((table->tableArray + newIndex)->key,key)){ //checking for same keys
                (table->tableArray + newIndex)->value = entry.value; //update key's value if duplicate key
                found = 1;
            }
            i++;
        }
        }
    else{
        printf("Error inserting into table.");
    }
    double loadfactor = ((double)table->count / table->capacity);
    if (loadfactor > 0.5) //table resize check
    { // check load factor
        resize_table(table);
    }
}

int resize_table(HashTable* table){
    Item items[table->count]; //creating array to hold items within current hash table
    int newCapacity = nextPrime(table->capacity * 2);
    int itemsIndex = 0;
    Item empty = {.key = NULL, .value = 0}; //creating empty entry for resized hash table
    for (int i = 0; i < table->capacity; i++)
    {
        if ((table->tableArray + i)->key != NULL) //extracting items from hash table
        {
            items[itemsIndex] = table->tableArray[i]; 
            itemsIndex++;
        }
    }
    //free(HashTable);
    if(!(table->tableArray = realloc(table->tableArray, newCapacity * sizeof(Item)))){ //resizing hash table
        printf("Error reallocating Hash Table array memory for resizing");
        free(table->tableArray);
        free(table);
        return -1;
    }
    for (int i = 0; i < newCapacity; i++){
        table->tableArray[i] = empty;
    }
    table->count = 0;
    table->capacity = newCapacity;
    for (int i = 0; i < itemsIndex; i++) //reinserting entries into resized hash table
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
        if (!strcmp(curItem.key, key))
        {
            return curItem.value;
        }
        i++;
        curItem = table->tableArray[((index + (i * i)) % table->capacity)];
    }
    return -1;
}

//int get_

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

int create_hash(char* key){
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
    insert(table, "hellosdfsdfsdf", "value 1");
    insert(table, "holafsadfasdfsdafds", "value 2");
    insert(table, "bonjourfasdfsdfg", "value 3");
    insert(table, "ciaofdsafasdfsadf", "value 4");
    insert(table, "ciaofasdfsadfsdf", "value 5");
    printf("%d, %s",retrieve(table, "hellosdfsdfsdf"), "\n");
    printf("%d, %s",retrieve(table, "holafsadfasdfsdafds"), "\n");
    printf("%d, %s",retrieve(table, "bonjourfasdfsdfg"), "\n");
    printf("%d, %s",retrieve(table, "ciaofdsafasdfsadf"), "\n");
    printf("%d, %s",retrieve(table, "ciaofasdfsadfsdf"), "\n");

    free_hashtable(table);

}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_TABLE_SIZE 7

typedef struct {
    char* key;
    void* value;
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

void insert_item(HashTable* table, char* key, void* value);
int resize_table(HashTable* table);
int is_prime(int n);
int next_prime(int n);
int create_hash(char* key);


void insert_item(HashTable* table, char* key, void* value){
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
    int newCapacity = next_prime(table->capacity * 2);
    int itemsIndex = 0;
    Item empty = {.key = NULL, .value = NULL}; //creating empty entry for resized hash table
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
        return 0;
    }
    for (int i = 0; i < newCapacity; i++){
        table->tableArray[i] = empty;
    }
    table->count = 0;
    table->capacity = newCapacity;
    for (int i = 0; i < itemsIndex; i++) //reinserting entries into resized hash table
    {
        insert_item(table, items[i].key, items[i].value);
    }
    return 1;
}

void* retrieve_value(HashTable* table, char* key){
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
    return NULL;
}

int remove_item(HashTable* table, char* key){
    int hash = create_hash(key);
    int index = hash % table->capacity;
    int removed = 0;
    int i = 0;
    Item curItem = table->tableArray[index];
    while (curItem.key != NULL){
        if (!strcmp(curItem.key, key))
        {
            table->tableArray[index].key = NULL;
            table->tableArray[index].value = NULL;
            table->count--;
            //removed = 1;
            return 1; // return 1 if item was found and removed
        }
        i++;
        curItem = table->tableArray[((index + (i * i)) % table->capacity)];
    }
    return 0; //return 0 if item was not found 
}




int is_prime(int n){
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

int next_prime(int n){
    if(n <= 1){
        return 2;
    }
    if(is_prime(n)){
        return n;
    }

    int found = 0;
    int prime = n;

    while(!found){
        prime++;
        if(is_prime(prime)){
            found = 1;
            return prime;
        }
    }
    return 0;
}

int create_hash(char* key){
    int hash = 0;
    int min = strlen(key);
    if(min > 5){
        min = 5;
    }
    for (int i = 0; i < min; i++){
        hash += (int)key[i] * pow(31, min - 1 - i);
    }
    return hash;
}



int main(){
    //tests to test basic functionality
    HashTable* table = init_hashtable();
    char* strentry = "88";
    int intentry = 88;
    double dentry = 4235.423;
    printf("Capacity before inserting items: %d\n", table->capacity);
    printf("Count before inserting items: %d\n", table->count);
    insert_item(table, "stringtest1", strentry);
    insert_item(table, "inttest1", &intentry);
    insert_item(table, "doubletest1", &dentry);
    printf("%s, %s", (char *)retrieve_value(table, "stringtest1"), "\n");
    printf("%d, %s",*((int*)retrieve_value(table, "inttest1")), "\n");
    printf("%f, %s",*((double*)retrieve_value(table, "doubletest1")), "\n");
    printf("Capacity after inserting items: %d\n", table->capacity);
    printf("Count after inserting items: %d\n", table->count);
    printf("FIRST SET OF TESTS COMPLETE\n\n");

    //tests for replacing values of duplicate keys
    strentry = "hello, world!";
    intentry = -52345;
    dentry = 999.999;
    printf("Capacity before inserting items: %d\n", table->capacity);
    printf("Count before inserting items: %d\n", table->count);
    insert_item(table, "stringtest1", strentry);
    insert_item(table, "inttest1", &intentry);
    insert_item(table, "doubletest1", &dentry);
    printf("%s, %s", (char *)retrieve_value(table, "stringtest1"), "\n");
    printf("%d, %s",*((int*)retrieve_value(table, "inttest1")), "\n");
    printf("%f, %s",*((double*)retrieve_value(table, "doubletest1")), "\n");
    printf("Capacity after inserting items: %d\n", table->capacity);
    printf("Count after inserting items: %d\n", table->count);
    printf("SECOND SET OF TESTS COMPLETE\n\n");

    //more tests
    strentry = "this^ is a really cool test for my hashtable!!!!---==+++````~~~";
    intentry = 0;
    dentry = -45254.3423423;
    printf("Capacity before inserting items: %d\n", table->capacity);
    remove_item(table, "stringtest1");
    printf("Count before inserting items: %d\n", table->count);
    insert_item(table, "stringtest2", strentry);
    insert_item(table, "inttest2", &intentry);
    insert_item(table, "doubletest2", &dentry);
    printf("%s, %s", (char *)retrieve_value(table, "stringtest2"), "\n");
    printf("%d, %s",*((int*)retrieve_value(table, "inttest2")), "\n");
    printf("%f, %s",*((double*)retrieve_value(table, "doubletest2")), "\n");
    printf("Capacity after inserting items: %d\n", table->capacity);
    printf("Count after inserting items: %d\n", table->count);
    printf("THIRD SET OF TESTS COMPLETE\n\n");

    //more tests
    strentry = "tEsTTT2131231235.";
    intentry = 0;
    dentry = 0.0;
    printf("Capacity before inserting items: %d\n", table->capacity);
    printf("Count before inserting items: %d\n", table->count);
    insert_item(table, "stringtest3", strentry);
    insert_item(table, "inttest3", &intentry);
    insert_item(table, "doubletest3", &dentry);
    printf("%s, %s", (char *)retrieve_value(table, "stringtest3"), "\n");
    printf("%d, %s",*((int*)retrieve_value(table, "inttest3")), "\n");
    printf("%f, %s",*((double*)retrieve_value(table, "doubletest3")), "\n");
    printf("Capacity after inserting items: %d\n", table->capacity);
    printf("Count after inserting items: %d\n", table->count);
    printf("FOURTH SET OF TESTS COMPLETE\n\n");
    free_hashtable(table);

}
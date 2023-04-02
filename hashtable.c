#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_TABLE_SIZE 13 //should be prime number for best performance

typedef struct {
    char* key;
    void* value; //void pointer to allow for different types of values
}Item; 

typedef struct {
    Item* tableArray; //holds all items
    int count;
    int capacity;
} HashTable;

HashTable *init_hashtable();
void free_hashtable(HashTable *table);
void insert_item(HashTable *table, char *key, void *value);
int resize_table(HashTable* table);
void *retrieve_value(HashTable *table, char *key);
int remove_item(HashTable *table, char *key);
int is_prime(int n);
int next_prime(int n);
int create_hash(char* key);

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
            else if(!strcmp((table->tableArray + newIndex)->key,key)){ //checking for duplicate keys
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
    { 
        resize_table(table);
    }
}

int resize_table(HashTable* table){ //resizes table if load factor > 0.5 (O(n))
    Item items[table->count]; //creating array to hold items within current hash table
    int newCapacity = next_prime(table->capacity * 2); //new capacity should also be prime
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
    if(!(table->tableArray = realloc(table->tableArray, newCapacity * sizeof(Item)))){ //reallocating memory to bigger chunk
        printf("Error reallocating Hash Table array memory for resizing");
        free(table->tableArray);
        free(table);
        return 0;
    }
    for (int i = 0; i < newCapacity; i++){
        table->tableArray[i] = empty; //initializing reallocated memory to NULL
    }
    table->count = 0;
    table->capacity = newCapacity;
    for (int i = 0; i < itemsIndex; i++) //reinserting entries into resized hash table
    {
        insert_item(table, items[i].key, items[i].value);
    }
    return 1;
}

void* retrieve_value(HashTable* table, char* key){ //retrieves value from hash table given the key, returns null pointer if failure
    int hash = create_hash(key);
    int index = hash % table->capacity;
    int i = 0;
    Item curItem = table->tableArray[index];
    while (curItem.key != NULL){ 
        if (!strcmp(curItem.key, key)) //checking if current item's key is the same as passed key
        {
            return curItem.value;
        }
        i++;
        curItem = table->tableArray[((index + (i * i)) % table->capacity)]; //quadratic probing to next spot
    }
    return NULL;
}

int remove_item(HashTable* table, char* key){ //removes item from hash table if it exists, returns 1 if success and 0 otherwise
    int hash = create_hash(key);
    int index = hash % table->capacity;
    int i = 0;
    Item curItem = table->tableArray[index];
    while (curItem.key != NULL){
        if (!strcmp(curItem.key, key)) //comparing current item's key and passed key
        {
            table->tableArray[index].key = NULL;
            table->tableArray[index].value = NULL;
            table->count--;
            return 1; // return 1 if item was found and removed
        }
        i++;
        curItem = table->tableArray[((index + (i * i)) % table->capacity)]; //quadratic probing to next spot
    }
    return 0; //return 0 if item was not found 
}




int is_prime(int n){ //checks if n is prime, returns 1 if true, 0 otherwise
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

int next_prime(int n){ //looks for next prime starting at n inclusive
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

int create_hash(char* key){ //Uses Horner's rule to create hash
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

    insert_item(table, "stringtest4", strentry);
    insert_item(table, "inttest4", &intentry);
    insert_item(table, "doubletest4", &dentry);
    insert_item(table, "stringtest5", strentry);
    insert_item(table, "inttest5", &intentry);
    insert_item(table, "fgsdgsdfg", &dentry);
    insert_item(table, "stringtest6", strentry);
    insert_item(table, "inttest6", &intentry);
    insert_item(table, "doubletest6", &dentry);
    insert_item(table, "stringtest7", strentry);
    insert_item(table, "inttest7", &intentry);
    insert_item(table, "doubletest7", &dentry);
    printf("Count after inserting items: %d\n", table->count);
    printf("Capacity after inserting items: %d\n", table->capacity);


    free_hashtable(table);

}
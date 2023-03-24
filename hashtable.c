#include <stdio.h>

#define INITIAL_TABLE_SIZE 23
#define MAX_KEY_SIZE 10
#define MAX_HASH_SIZE 20

struct Item{
    char key[30];
    int value;
};

int count = 0;
int size = INITIAL_TABLE_SIZE;
struct Item *HashTable[INITIAL_TABLE_SIZE];

struct Item* createItem(char key[], int value){
    //struct Item item = {.key = key, .value = value};
    struct Item *item = (struct Item *)malloc(sizeof(struct Item));
    strcpy(item->value, key);
    item->value = value;
    return item;
}

void insert(char key[], int value){
    int hash = create_hash(key);
    int index = hash % (sizeof(HashTable)/sizeof(HashTable[0]));
    struct Item *item = createItem(key, value);
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

    if((double)(count / size) > 0.5 ){ //check load factor
        int const newSize = nextPrime(size * 2);
        struct Item *ptr = (struct Item *)malloc(size * sizeof(struct Item));
        //how to?
    }
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
    int min = sizeof(key) / sizeof(key[0]);
    if(min > 8){
        min = 8;
    }
    for (int i = 0; i < min; i++){
        hash += (int)key[i] * pow(31, min - 1 - i);
    }
    return hash;
}



int main(){
    // int n;
    // printf("Enter a number to check prime: \n");
    // scanf("%d", &n);
    // //printf("%d",n);
    // int next = nextPrime(n + 1);
    // if (isPrime(n))
    // {
    //     printf("Your number is prime and the next prime after that is: %d", next);
    // }
    // else{
    //     printf("Your number is not prime but the next prime is: %d", next);
    // }
    // return 0; 
}


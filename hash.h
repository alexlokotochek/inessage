
struct ListNode{
    char* key;
    void* data;
    struct ListNode* next;
    struct ListNode* prev;
};

struct Table{
    int numberOfCells;
    int numberOfElements;
    struct ListNode** cell;
};


struct Table* hashTable;

struct Table* createTable(int size);

int hashFunction(char* key, int size);

struct Table* insert(char* Newkey, void* data, struct Table* hashTable);

int contains(char* key, struct Table* hashTable);

void delete(char* key, struct Table* hashTable);

void outTable(struct Table* hashTable);

struct Table* hashTableResize(struct Table* oldHashTable);

void clearTable(struct Table* hashTable);

struct Table* getDataFromConsole(struct Table* hashTable, int number);

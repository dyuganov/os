#define MALLOC_FAIL (-1)
#define NULL_POINTER_ARGUMENT (-2)

struct node;
typedef struct node Node;
struct list;
typedef struct list List;
Node* createNode();
void freeNode(Node* currentNode);
List* createList();
char* createString(unsigned int length);
int addString(List* list, const char* newLine);
void deleteList(List* list);
void printList(List* list);

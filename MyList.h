#ifndef _LLIST_H_
#define _LLIST_H_


typedef struct _MyList MyList;


struct _MyList
{
  void *data;
  MyList *next;
  MyList *prev;
};

MyList        *list_erase      (MyList *myList);
MyList        *list_erased     (MyList *myList);
void         list_rerase     (MyList *myList);
void         list_rerased    (MyList *myList);
void         list_insert     (MyList *myList, void *data);
MyList        *list_push_front (MyList *myList, void *data);
MyList        *list_push_back  (MyList *myList, void *data);
void         list_pop_front  (MyList *myList);
void         list_pop_back   (MyList *myList);
void         list_popd_front (MyList *myList);
void         list_popd_back  (MyList *myList);
MyList        *list_front      (MyList *myList);
MyList        *list_back       (MyList *myList);

void         list_remove     (MyList *myList, void *data);
MyList        *list_find       (MyList *myList, void *data);
MyList        *list_nth        (MyList *myList, unsigned int n);
unsigned int list_size       (MyList *myList);



#endif

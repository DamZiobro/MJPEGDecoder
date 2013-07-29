//--------------------------------------------------------------------------------------------------

#include "MyList.h"
#include <stdlib.h>

//--------------------------------------------------------------------------------------------------

MyList *list_erase(MyList *myList)
{
  MyList *next = myList->next;

  if (myList->prev)
	  myList->prev->next = next;
  if (next)
	  myList->next->prev = myList->prev;

  free(myList);

  return next;
}

//--------------------------------------------------------------------------------------------------

MyList *list_erased(MyList *myList)
{
  if (myList->data) {
    free(myList->data);
    myList->data = 0;
  }
  return list_erase(myList);
}

//--------------------------------------------------------------------------------------------------

void list_rerase(MyList *myList)
{
  if (myList->next)
    list_rerase(myList->next);
  myList->next = 0;

  free(myList);
}

//--------------------------------------------------------------------------------------------------

void list_rerased(MyList *myList)
{
  if (myList->next)
    list_rerased(myList->next);
  myList->next = 0;

  if (myList->data) {
    free(myList->data);
    myList->data = 0;
  }
  free(myList);
}

//--------------------------------------------------------------------------------------------------

void list_insert(MyList *myList, void *data)
{
  MyList *next = (MyList *) malloc(sizeof(MyList));
  next->data = myList->data;
  next->next = myList->next;
  next->prev = myList;
  myList->data = data;
  myList->next = next;
}

//--------------------------------------------------------------------------------------------------

MyList *list_push_front(MyList *myList, void *data)
{
	myList = list_front(myList);
	myList->prev = (MyList *) malloc(sizeof(MyList));
	myList->prev->data = data;
	myList->prev->next = myList;
	myList->prev->prev = 0;

  return myList->prev;
}

//--------------------------------------------------------------------------------------------------

MyList *list_push_back(MyList *myList, void *data)
{
	myList = list_back(myList);
	myList->next = (MyList *) malloc(sizeof(MyList));
	myList->next->data = data;
	myList->next->next = 0;
	myList->next->prev = myList;

  return myList->next;
}

//--------------------------------------------------------------------------------------------------

void list_pop_front(MyList *myList)
{
  list_erase(list_front(myList));
}

//--------------------------------------------------------------------------------------------------

void list_pop_back(MyList *myList)
{
  list_erase(list_back(myList));
}

//--------------------------------------------------------------------------------------------------

void list_popd_front(MyList *myList)
{
  list_erased(list_front(myList));
}

//--------------------------------------------------------------------------------------------------

void list_popd_back(MyList *myList)
{
  list_erased(list_back(myList));
}

//--------------------------------------------------------------------------------------------------

MyList *list_front(MyList *myList)
{
  while (myList->prev)
	  myList = myList->prev;
  return myList;
}

//--------------------------------------------------------------------------------------------------

MyList *list_back(MyList *myList)
{
  while (myList->next)
	  myList = myList->next;
  return myList;
}

//--------------------------------------------------------------------------------------------------

void list_remove(MyList *myList, void *data)
{
  MyList *myList2;
  while ((myList2 = list_find(myList, data)))
	  myList = list_erase(myList2);
}

//--------------------------------------------------------------------------------------------------

MyList *list_find(MyList *myList, void *data)
{
  while (myList && (myList->data != data))
	  myList = myList->next;
  return myList;
}

//--------------------------------------------------------------------------------------------------

MyList *list_nth(MyList *myList, unsigned int n)
{
  unsigned int i;
  for (i = 0; (i < n) && (myList); ++i, myList = myList->next);
  return myList;
}

//--------------------------------------------------------------------------------------------------

unsigned int list_size(MyList *myList)
{
  int i;
  for (i = 0; (myList); ++i, myList= myList->next);
  return i;
}

//--------------------------------------------------------------------------------------------------

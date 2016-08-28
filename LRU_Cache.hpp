//
//  LRU_Cache.hpp
//  LRU_Cache
//
//  Created by chenchen on 16/8/28.
//  Copyright © 2016年 chenchen. All rights reserved.
//

#ifndef LRU_Cache_hpp
#define LRU_Cache_hpp

#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <utility>

template <typename T>
struct DListItem
{
    DListItem *pre;
    DListItem *next;
    T *data;
};

template <typename T>
class DList {
private:
    DListItem<T> head;
    DListItem<T> tail;
    int itemNum;
    int capacity;
    
    int insertAfter(DListItem<T> *pos, DListItem<T> *target);
    int deleteItem(DListItem<T> *target);
    
    
public:
    DList(int capacity = 10);
    ~DList() {}
    
    
    DListItem<T> *pushFront(T *data);
    
    //DListItem<T> *getHead() {return &head;}
    //DListItem<T> *getTail() {return &tail;}
    
    /* update last item value with data(free before), and move last item to head. */
    DListItem<T> *upgradeLast(T *data, T *&oldData);
    
    /* move target item to head. */
    int upgradeTarget(DListItem<T> *target);
    
    //void showList() const;
    
};


template <typename T>
int DList<T>::insertAfter(DListItem<T> *pos, DListItem<T> *target)
{
    if (!pos || !target)
        return -1;
    
    target->pre = pos;
    target->next = pos->next;
    pos->next->pre = target;
    pos->next = target;
    
    return 0;
}

template <typename T>
int DList<T>::deleteItem(DListItem<T> *target)
{
    if (!target)
        return -1;
    target->pre->next = target->next;
    target->next->pre = target->pre;
    
    return 0;
}


template <typename T>
DList<T>::DList(int capacity): itemNum(0), capacity(capacity)
{

    head.next = &tail;
    head.pre = NULL;
    head.data = NULL;
    
    tail.next = NULL;
    tail.pre = &head;
    tail.data = NULL;
    
}


template <typename T>
DListItem<T>* DList<T>::pushFront(T *data)
{
    
    if (itemNum >= capacity)
        return NULL;
    
    DListItem<T> *tmp = new DListItem<T>;
    tmp->data = data;
    
    this->insertAfter(&head, tmp);
    
    itemNum++;
    
    
    return tmp;
}

template <typename T>
DListItem<T> *DList<T>::upgradeLast(T *data, T *&oldData)
{
    DListItem<T> *last = tail.pre;
    
    if (last == &head)
        return NULL;
    
    //if (last->data)
        //free(last->data);
    oldData = last->data;
    
    last->data = data;
    
    this->deleteItem(last);
    this->insertAfter(&head, last);
    
    return last;
}

template <typename T>
int DList<T>::upgradeTarget(DListItem<T> *target)
{
    if (!target)
        return -1;
    
    this->deleteItem(target);
    this->insertAfter(&head, target);
    
    return 0;
}


template <typename T>
void DList<T>::showList() const
{
    DListItem<T> *ptr = head.next;
    
    while (ptr != &tail)
    {
        std::cout << *ptr->data << "-->";
        ptr = ptr->next;
    }
    
    std::cout << std::endl;
}

typedef std::pair<int, int> itemType;

class LRUCache
{
public:
    LRUCache(int capacity): itemList(capacity)
    {}
    
    int get(int key);
    
    void set(int key, int value);
    
private:
    DList<itemType> itemList;
    std::unordered_map<int, DListItem<itemType> *> map;
    
};

int LRUCache::get(int key)
{
    std::unordered_map<int, DListItem<itemType> *>::const_iterator itor = map.find(key);
    if (itor == map.end())
        return -1;
    else
    {
        DListItem<itemType> *target = itor->second;
        itemList.upgradeTarget(target);
        return target->data->second;
    }
}

void LRUCache::set(int key, int value)
{
    std::unordered_map<int, DListItem<itemType> *>::iterator itor = map.find(key);
    DListItem<itemType> *target;
    
    if (itor != map.end())
    {
        /* update value */
        target = itor->second;
        target->data->second = value;
        
        itemList.upgradeTarget(target);
    }
    else
    {
        /* try to insert firstly */
        itemType *iValue = new itemType(key, value);
        
        target = itemList.pushFront(iValue);
        if (!target)
        {
            itemType *oldData;
            target = itemList.upgradeLast(iValue, oldData);

            map.erase(oldData->first);
            
            delete oldData;
        }
        
        if (target)
            map.insert(std::pair<int, DListItem<itemType> *>(key, target));
    }
}
















#endif /* LRU_Cache_hpp */

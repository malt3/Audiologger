//
//  LinkedList.hpp
//  Audiologger
//
//  Created by Malte Poll on 15.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#pragma once
#include <iostream>

template <class T>
class LinkedList{
private:
    class Obj{
    public:
        Obj(T* var){
            variable = var;
        }
        ~Obj(){
            if (variable != nullptr) {
                delete variable;
            }
        }
        T* variable = nullptr;
        Obj* ptrNext = nullptr;
        bool hasNext(){
            return (bool)ptrNext;
        }

    };
    Obj* firstObj;
    Obj* getObjAt(int index){
        if (index > (numObjects-1) || index < 0) {
            std::cout << "index is out of range" << std::endl;
            return 0;
        }
        Obj* currentObj = firstObj;
        while (index > 0) {
            currentObj = currentObj->ptrNext;
            index--;
        }
        return currentObj;
    }
    int numObjects;
    
public:
    LinkedList();
    
    int getNumObjects();
    T* getVarAt(int index);
    void insertAt(T* var, int index);
    void deleteAt(int index);
    void push(T* var);
    void pop();
    void clear();
    T* fst();
    T* lst();
};

/* Implementation */

template <class T>
LinkedList<T>::LinkedList(){
    firstObj = nullptr;
    numObjects = 0;
}

template <class T>
int LinkedList<T>::getNumObjects(){
    return numObjects;
}

template <class T>
T* LinkedList<T>::getVarAt(int index){
    return getObjAt(index)->variable;
}

template <class T>
void LinkedList<T>::insertAt(T* var, int index){
    Obj* parentObj = getObjAt(index-1);
    Obj* oldObjAtIndex = parentObj->ptrNext;
    
    parentObj->ptrNext = new Obj(var);
    parentObj->ptrNext->ptrNext = oldObjAtIndex;
    
    numObjects++;
}

template <class T>
void LinkedList<T>::deleteAt(int index){
    if (index == 0) {
        pop();
    }else{
        Obj* parentObj = getObjAt(index-1);
        Obj* sonObj = parentObj->ptrNext->ptrNext;
        delete parentObj->ptrNext;
        parentObj->ptrNext = sonObj;
        numObjects--;
    }
    
}

template <class T>
void LinkedList<T>::push(T* var){
    if (numObjects == 0 || firstObj == nullptr) {
        firstObj = new Obj(var);
    }else{
        Obj* oldFirst = firstObj;
        firstObj = new Obj(var);
        firstObj->ptrNext = oldFirst;
    }
    numObjects++;
}

template <class T>
void LinkedList<T>::pop(){
    if (numObjects == 0 || firstObj == nullptr) {
        std::cout << "Cannot pop from empty list" << std::endl;
        return;
    }
    Obj* newFirst = firstObj->ptrNext;
    delete firstObj;
    firstObj = newFirst;
    numObjects--;
}

template <class T>
void LinkedList<T>::clear(){
    Obj* currentObj = firstObj;
    Obj* nextObj;
    firstObj = nullptr;
    while (currentObj->hasNext()) {
        nextObj = currentObj->ptrNext;
        delete currentObj;
        currentObj = nextObj;
    }
}

template <class T>
T* LinkedList<T>::fst(){
    return getVarAt(0);
}

template <class T>
T* LinkedList<T>::lst(){
    return getVarAt(numObjects-1);
}
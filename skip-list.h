#ifndef SKIP_H
#define SKIP_H

#include <vector>
#include <iostream>
#include <math.h>
#include <limits>

using namespace std;

template<typename T>
class skip_list {
    struct node {
        T datum;
        node* prev;
        node* next;
        node* down;
        node* up;
    };
    node* head;
    int numRows,numNodes;

    void copy_row(node* end) {
        node* newEnd = new node;
        while (end)
        {
            end->down = newEnd;
            newEnd->up = end;
            newEnd->datum = end->datum;

            newEnd->next = new node;
            newEnd->next->prev = newEnd;

            newEnd = newEnd->next;
            end = end->next;
        }
        newEnd = nullptr; // cut off final entry;
    }

    void build_up(node* n) {
        if (rand() % 2 == 0)
        {
            node* newDown = n;
            T newDatum = n->datum;
            n = n->prev;
            while (!n->up)
            {
                n = n->prev;
            }
            insert_between(newDatum,n);
            n->down = newDown;
            newDown->up = n;
        }
        
    }

    void insert_between(const T& newDatum, node* n) {
        node* toInsert = new node;
        toInsert->prev = n;
        toInsert->next = n->next;
        toInsert->datum = newDatum;

        n->next = toInsert;
        if (toInsert->next)
        {
            toInsert->next->prev = toInsert;
        }
        
        build_up(toInsert);
    }

    public:

    skip_list() {
        node* dummy = new node;
        T small = T(-1 * INFINITY);
        dummy->datum = small;
        head = dummy;
        numNodes = 0;
        numRows = 0;
    }

    void insert(const T& newDatum) {
        node* at = find(newDatum);
        if (!at)
        {
            return;
        }
        // insert using the current location from find
        else {
            if (numNodes == (pow(2,numRows + 1) - 1))
            {
                numRows++;
                // move node to end of row
                node* end = at;
                while (end->prev)
                {
                    end = end->prev;
                }
                
                copy_row(end);
                at = at->down;
            }
            
            insert_between(newDatum, at);
        }
        numNodes++;
        return;
    }

    void erase(const T& newDatum) {
        node* at = find(newDatum);
        if (!at)
        {
            return;
        }

        numNodes--;
        // delete the node
        if (at->prev)
        {
            at->prev->next = at->next;
        }
        if (at->next)
        {
            at->next->prev = at->prev;
        }
        // delete up
        while (at)
        {
            node* nextUp = at->up;
            delete at;
            at = nextUp;
        }
    }

    node* find(const T& newDatum) {
        node* readHead = head;
        // somewhat counterintuitively, this function returns nullptr
        // if the item is found, else returns a pointer to the left of 
        // where the new node will go
        while (newDatum != readHead->datum)
        {
            if (newDatum > readHead->datum)
            {
                if (readHead->next)
                {
                    readHead = readHead->next;
                }
                else if (readHead->down)
                {
                    readHead = readHead->down;
                }
                else {
                    return readHead;
                }
            }
            else {
                return readHead->prev; // might segfault
            }
        }
        
        return nullptr;
    }

    size_t size() {
        return numNodes;
    }
};

#endif
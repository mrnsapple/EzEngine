#ifndef STACKLINKEDLIST_H
#define STACKLINKEDLIST_H
namespace ez {

template <typename  T>
class StackLinkedList {
public:
    struct Node {
        T data;
        Node* next = nullptr;
        //Node* prev = nullptr;
        Node(){}

    };
    
    Node* head = nullptr;
public:
    StackLinkedList() = default;
    StackLinkedList(StackLinkedList &stackLinkedList);
    void push(Node * newNode);
    void read(Node * newNode);
    Node* pop();
};
}

#include "../Private/StackLinkedList.tpp"

#endif




#ifndef STACKLINKEDLISTIMPL_T
#define STACKLINKEDLISTIMPL_T

namespace ez {

template <class T>
StackLinkedList<T>::StackLinkedList(StackLinkedList &stackLinkedList):head(stackLinkedList.head)
{
}

template <class T>
void StackLinkedList<T>::push(Node * newNode) {
    newNode->next = head;
    head = newNode;
}

template <class T>
void StackLinkedList<T>::read(Node * newNode) {

    int count = 1;
    for (Node *i = newNode; i != nullptr; i = i->next) {
        if (++count > 20)
            break;

    }
}

template <class T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
    Node * top = head;
    head = head->next;
    return top;
}
}

#endif 
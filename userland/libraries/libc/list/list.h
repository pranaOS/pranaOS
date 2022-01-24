#pragma once

namespace LibC {

    template <typename T>
    struct listNode {
        listNode(const T &e) : data(e), next(0), prev(0)
        {}
        T data;
        listNode<T>* next;
        listNode<T>* prev;
    };

    template <typename T>
    class list {
    public:
        list() : head_(0), tail_(0), size_(0)
        {}

        ~list() {
            this->clear();
        }

        int size() { 
            return size_; 
        }

        void push_back(const T &e);
        void push_front(const T &e);
        void clear();

        T getAt(int index);
        T operator[](int index);
        void remove(int index);
        void remove(const T &e);

        void operator+=(const T &e);
        void operator-=(const T &e);

    private:
        listNode<T>* head_;
        listNode<T>* tail_;
        int size_;
        listNode<T>* insertInternal(const T &e, listNode<T>* pos);
        void removeInternal(listNode<T> *pos);

    public:
        class iterator {
        public:
            iterator(listNode<T> *p=0) : pos_(p) { }
            
            T &operator*() {
                return pos_->data;
            }

            T *operator->() {
                return &(pos_->data); 
            }

            bool operator!=(const iterator &rhs) {
                return this->pos_ != rhs.pos_; 
            }

            iterator operator++() {
                pos_ = pos_->next; return *this; 
            }

            iterator operator--() {
                pos_ = pos_->prev; return *this; 
            }

        private:
            listNode<T> *pos_;
        };
        iterator begin() {
            return iterator(head_);
        }

        iterator end() {
            return iterator(0);
        }
    };
}

using namespace pranaOSList;

template <typename T>
listNode<T>* List<T>::insertInternal(const T &e, listNode<T>* pos) {
    listNode<T> *n = new listNode<T>(e);
    size_++;

    n->next = pos;

    if(pos) {
        n->prev = pos->prev;
        pos->prev = n;
    }
    else {
        n->prev = tail_;
        tail_ = n;
    }
    if(n->prev) {
        n->prev->next = n;
    }
    else {
        head_ = n;
    }

    return n;
}

template <typename T>
void List<T>::push_back(const T &e) {
    insertInternal(e, 0);
}

template <typename T>
void List<T>::push_front(const T &e) {
    insertInternal(e, head_);
}

template <typename T>
void List<T>::removeInternal(listNode<T> *pos) {
	if(pos) {
		if(pos->prev)
			pos->prev->next = pos->next;
		if(pos->next)
			pos->next->prev = pos->prev;
		if(pos == head_)
			head_ = pos->next;
		if(pos == tail_)
			tail_ = pos->prev;
		delete pos;
		size_--;
	}
}

template <typename T>
void List<T>::remove(int index) {
    listNode<T>* cur = head_;
    for(int i = 0; i < index; ++i)
        cur = cur->next;
    removeInternal(cur);
}

template <typename T>
void List<T>::remove(const T &e) {
    for(int i = 0; i < size_; i++)
        if(getAt(i) == e)
            remove(i);
}

template <typename T>
void List<T>::Clear() {
    listNode<T>* current( head_ );

    while(current)
    {
        listNode<T>* next( current->next );
        delete current;
        current = next;
    }
    size_ = 0; 
    head_ = 0;
    tail_ = 0;
}

template <typename T>
T List<T>::getAt(int index) {
    listNode<T>* cur = head_;
    for(int i = 0; i < index; ++i)
        cur = cur->next;
    return cur->data;
}

template <typename T>
T List<T>::operator[](int index) {
    return getAt(index);
}

template <typename T>
void List<T>::operator+=(const T &e) {
    push_back(e);
}

template <typename T>
void List<T>::operator-=(const T &e) {
    remove(e);
}
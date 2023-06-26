#pragma once

// добавьте неоходимые include-директивы сюда

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
           node_ = other.node_;
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (*this).node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }
        
         [[nodiscard]] reference operator*() const noexcept {
            assert(node_->value != NULL);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_->value != NULL);
            return &(node_->value);
        }

        BasicIterator& operator++() noexcept {
           node_ = node_->next_node;
           return *this;
        }

        BasicIterator operator++(int) noexcept {
           BasicIterator temp{node_->next_node};
           auto temp_return = *this;
           *this = temp;
           return temp_return;
        }

    private:
        Node* node_ = nullptr;
    };
    
public:

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    void PushFront(const Type& value){
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void PopFront(){
        assert(size_ != 0);
        Node* temp = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = temp;
        --size_;
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_}; 
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        pos.node_->next_node = new Node(value, (*pos.node_).next_node);
        ++size_;
        return Iterator{pos.node_->next_node};
    }
    
    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* temp = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = temp;
        return Iterator{temp};
    }
    
    SingleLinkedList() = default;
    
    SingleLinkedList(std::initializer_list<Type> values) {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList tmp;
        for (const auto& i : values){
            tmp.PushFront(i);
        }
        
        SingleLinkedList tmp1;
        for (const auto& i : tmp){
            tmp1.PushFront(i);
        }
        
        swap(tmp1);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList tmp;
        for (const auto& i : other){
            tmp.PushFront(i);
        }
        
        SingleLinkedList tmp1;
        for (const auto& i : tmp){
            tmp1.PushFront(i);
        }
        
        swap(tmp1);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList<Type> temp(rhs);
            swap(temp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }
    
    void Clear() noexcept {
        if (size_ == 0) return;
        while (head_.next_node != nullptr){
            Node* temp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete temp;
        }
        size_ = 0;
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
  
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }
    
    ~SingleLinkedList(){
        Clear();
    }

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(lhs.size_ != rhs.size_) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
} 

// внешние функции разместите здесь

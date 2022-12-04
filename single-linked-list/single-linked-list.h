#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>

#include "single-linked-list.h"

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value{};
        Node* next_node = nullptr;
    };

    // Шаблон класса Базовый Итератор.
	// Определяет поведение итератора на элементы односвязного списка
	// ValueType - совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
    	// Класс списка объявляется дружественным, чтобы из методов списка
    	// был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) : node_(node) {
        }

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

		// Категория итератора - forward iterator
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
		// попросим компилятор сгенерировать его за нас.
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
		// Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
		// Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
		// Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        /// Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка.
		// Возвращает прежнее значение итератора
		// Инкремент итератора, не указывающего на существующий элемент списка,
		// приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            Node* old_node_ = node_;
            node_ = node_ != nullptr ? node_->next_node : node_;
            return BasicIterator(old_node_);
        }

		// Операция разыменования. Возвращает ссылку на текущий элемент
		// Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
		// приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка.
		// Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
		// приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ !=nullptr);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        Iterator it(head_.next_node);
        for (; it.node_ != nullptr;++it) {}
        return it;
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен end()
	// Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	// Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        ConstIterator it(head_.next_node);
        for (; it.node_ != nullptr; ++it) {}
        return it;
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        ConstIterator it(head_.next_node);
        for (; it.node_ != nullptr; ++it) {}
        return it;
    }

    // Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type& value) {
        Node before;
        before.next_node = head_.next_node;
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    // Очищает список за время O(N)
    void Clear() noexcept {
        if (head_.next_node != nullptr) {
            while (head_.next_node->next_node != nullptr) {
                Node name;
                name.next_node = head_.next_node->next_node;
                delete head_.next_node;
                head_.next_node = name.next_node;
                --size_;
            }
            delete head_.next_node;
            head_.next_node = nullptr;
            --size_;
        }
    }



    SingleLinkedList() = default;
    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto iter = values.end() - 1; iter > values.begin(); --iter) {
            PushFront(*iter);
        }
        PushFront(*values.begin());
    }

    SingleLinkedList(const SingleLinkedList& other) {
    	// Сначала надо удостовериться, что текущий список пуст
        assert(size_ == 0 && head_.next_node == nullptr);

        SingleLinkedList tmp;
        /* скопировать внутрь tmp элементы other */
        auto it = tmp.before_begin();
        for (auto iter : other) {
            it.node_->next_node = new Node(iter, nullptr);
            ++it;
            ++tmp.size_;
        }
        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        if (!tmp.IsEmpty()) {
            swap(tmp);
        }
        // Теперь tmp пуст, а текущий список содержит копию элементов other
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (rhs.IsEmpty()) {
            Clear();
            return *this;
        }
        if (this != &rhs) {
        	// Реализация операции присваивания с помощью идиомы Copy-and-swap
			// Если исключение будет выброшено, то на текущий объект оно не повлияет
            auto rhs_copy(rhs);
            // rhs_copy содержит копию правого аргумента
			// Обмениваемся с ним данными
            swap(rhs_copy);
            // теперь текущий объект содержит копию правого аргумента,
			// а rhs_copy - прежнее состояние текущего объекта, которое при выходе
			// из блока будет разрушено
        }
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }


    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования привед т к неопредел нному поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        Node* befor_first_copy =const_cast<Node*>(&head_);
        return Iterator (befor_first_copy);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования привед т к неопредел нному поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        Node* befor_first_copy = const_cast<Node*>(&head_);
        return Iterator(befor_first_copy);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования привед т к неопредел нному поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        Node* befor_first_copy = const_cast<Node*>(&head_);
        return Iterator(befor_first_copy);
    }

    /*
	* Вставляет элемент value после элемента, на который указывает pos.
	* Возвращает итератор на вставленный элемент
	* Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
	*/
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* old_pos = pos.node_;
        pos.node_->next_node = new Node(value, old_pos->next_node);
        ++size_;
        return Iterator(old_pos->next_node);
    }

    void PopFront() noexcept {
        Node* deleting_element = begin().node_->next_node;
        delete  begin().node_;
        head_.next_node = deleting_element;
        --size_;
    }

    /*
	* Удаляет элемент, следующий за pos.
	* Возвращает итератор на элемент, следующий за удал нным
	*/
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        Node* deleting_element = pos.node_->next_node;
        pos.node_->next_node = deleting_element->next_node;
        delete deleting_element;
        --size_;
        return Iterator(pos.node_->next_node);
    }


    ~SingleLinkedList() {
        Clear();
    }
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin());;
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()) || std::equal(lhs.begin(), lhs.end(), rhs.begin());
}



# cpp-single-linked-list
Develop my own container — a singly linked list.

Read in other languages: [English](README.md), [Русский](README.RUS.md)<br>

# Program Description
A learning project that allowed me to better understand the structure of C++ containers and use them effectively.<br>

# Build using Cmake
To build this project on linux you need:<br>
1) If you don't have Cmake installed, install Cmake<br>
2) If the "Debug" or "Release" folders are not created:<br>

```
mkdir Debug
mkdir Release
```
3) Run the command for Debug and/or Release conf:<br>

```
cmake -E chdir Debug/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Debug
cmake -E chdir Release/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release 
```
4) Go to "Debug" or "Release" folder and build:<br>

```
cmake --build .
```

5) To **Run** program - in the debug or release folder run:<br>

```
./single_linked_list
```
# System requirements:
  1. C++17(STL)
  2. GCC (MinG w64) 11.2.0  

# Technology stack:
  1. forward_list <br>
  2. Template classes<br>
  3. Iterators begin, end, before_begin <br>
  4. iterator_category, value_type, difference_type, pointer, reference<br>
  5. swap<br>
  6. Three levels of exception security<br>

## Before you start:
  0. Installation and configuration of all required components in the development environment to run the application<br>
  1. The use case and tests are shown in main.cpp .<br>

# Detailed description of the project:
### A singly linked list -
is one of the basic linked data structures. To understand how it works is to take the first step towards developing more complex structures. In the standard library, a singly linked list is represented by a template of the `forward_list` class.<br>
<br>
The list item is called a "node". The list item can be represented as a **Node** structure, which contains the value of the item and a pointer to the next node. <br>

### The template class of a SingleLinkedList<Type> has the following functionality:
 - Default constructor that creates an empty list;<br>
 - The `GetSize` method, which returns the number of items in the list;<br>
 - The `IsEmpty` method, which returns **true** if the list is empty, and **false** otherwise.<br>
 - `PushFront` - inserts an element at the beginning of a singly linked list, and the 'Clear` operation clears the list.<br>
The `PushFront` method provides a strict exception safety guarantee: if an exception is thrown during the operation of the method, the state of the list should be the same as before the method was called.<br>
 - The `Clear` method clears the list and does not throw exceptions. <br>
 - When the list is destroyed, all its elements are deleted.<br>
 - The `PopFront` method. Deletes the first element of a non-empty list in time **O(1)**. Does not throw exceptions.<br>
 - The `InsertAfter` method. During the time **O(1)** inserts a new value into the list following the element referenced by the iterator passed to insertAfter. The method provides a strict guarantee of exception safety.<br>
 - The `EraseAfter` method. During **O(1)** removes from the list the element following the element referenced by the iterator passed to `insertAfter`. Does not throw exceptions.<br>
 - Methods `before_begin` and `cbefore_begin`. Returns iterators referring to a dummy position before the first element of the list. Such an iterator is used as a parameter for the `InsertAfter` and `EraseAfter` methods when it is necessary to insert or delete an element at the beginning of the list. This iterator cannot be dereferenced.<br>

### Implemented support for iterating through the elements of the SingleLinkedList container.
The `Basic Iterator` class, on the basis of which the constant and non-constant iterator of the list are declared.<br>
The list class implements a constant and non-constant version of the `begin` and `end` methods, which return iterators to the first element of the container and the position following the last element.<br>
To make it convenient to get constant iterators, the `cbegin` and `cend` methods are implemented.<br>

### In the class of a singly linked list, the following functionality:
- Comparison operations ==, !=, <, >, <=, >=;< br>
- Exchange of the contents of two lists using the swap method and the template swap function;<br>
- Construction of a singly linked list based on `initializer_list`. The sequence of elements of the created list and `initializer_list` is the same;<br>
- Reliable copy constructor and assignment operation. The assignment operation provides a strict guarantee of exception safety. If an exception is thrown during the assignment process, the contents of the left argument of the assignment operation will remain unchanged.<br>
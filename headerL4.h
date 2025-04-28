#ifndef HEADERL3_H_INCLUDED
#define HEADERL3_H_INCLUDED

#include <vector>
#include <list>
#include <stdexcept>
#include <iterator>
#include <functional>

using namespace std;

template <typename T>
class Iterator
{
public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};

template <typename T>
class IContainer
{
public:
    virtual ~IContainer() {}
    virtual Iterator<T>* GetIterator() = 0;
};

template <typename T>
class StackIterator : public Iterator<T>
{
private:
    T* StackContainer;
    size_t Pos;
    size_t Size;

public:
    StackIterator(T* container, size_t size)
        : StackContainer(container), Pos(0), Size(size) {}

    void First() override { Pos = 0; }
    void Next() override { if (Pos < Size) Pos++; }
    bool IsDone() const override { return (Pos >= Size); }

    T GetCurrent() const override
    {
        if (Pos >= Size) throw out_of_range("Iterator out of range");
        return StackContainer[Pos];
    }
};

const size_t MaxSize = 100;

template <typename T>
class ContainerStack : public IContainer<T>
{
private:
    T Items[MaxSize];
    size_t Top;

public:
    ContainerStack() : Top(0) {}

    bool IsEmpty() const { return (Top == 0); }
    size_t Size() const { return Top; }

    void Push(const T& newObject)
    {
        if (Top >= MaxSize) throw overflow_error("Stack overflow");
        Items[Top++] = newObject;
    }

    T Pop()
    {
        if (IsEmpty()) throw underflow_error("Stack underflow");
        return Items[--Top];
    }

    T GetElementByIndex(size_t index) const
    {
        if (index >= Top) throw out_of_range("Index out of range");
        return Items[index];
    }

    Iterator<T>* GetIterator() override
    {
        return new StackIterator<T>(Items, Top);
    }
};

template <typename T>
class ListWalker : public Iterator<T>
{
private:
    list<T>* lst;
    typename list<T>::iterator It;

public:
    ListWalker(list<T>* l) : lst(l), It(l->begin()) {}

    void First() override { It = lst->begin(); }
    void Next() override { if (It != lst->end()) ++It; }
    bool IsDone() const override { return It == lst->end(); }

    T GetCurrent() const override
    {
        if (It == lst->end()) throw out_of_range("Iterator out of range");
        return *It;
    }
};

template <typename T>
class ContainerList : public IContainer<T>
{
private:
    list<T> elements;

public:
    void add_element(const T& element) { elements.push_back(element); }
    size_t count() const { return elements.size(); }

    ListWalker<T> begin() { return ListWalker<T>(&elements); }
    ListWalker<T> end() { return ListWalker<T>(&elements); }

    Iterator<T>* GetIterator() override
    {
        return new ListWalker<T>(&elements);
    }
};

template <typename T>
class STLAdapter : public Iterator<T>
{
private:
    vector<T> elements;
    typename vector<T>::iterator current;
    typename vector<T>::iterator end;

public:
    STLAdapter(Iterator<T>* it)
    {
        for (it->First(); !it->IsDone(); it->Next())
        {
            elements.push_back(it->GetCurrent());
        }
        current = elements.begin();
        end = elements.end();
    }

    void First() override { current = elements.begin(); }
    void Next() override { if (current != end) ++current; }
    bool IsDone() const override { return current == end; }

    T GetCurrent() const override
    {
        if (current == end) throw out_of_range("Iterator out of range");
        return *current;
    }
};

#endif // HEADERL4_H_INCLUDED

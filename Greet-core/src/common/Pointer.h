#pragma once

#include <common/Types.h>
#include <malloc.h>
#include <cstring>
#include <initializer_list>
#include <logging/Log.h>

namespace Greet
{

  template <class T>
  class PointerIterator
  {
    private:
      T* begin;
      T* end;
      T* current;
    public:

      PointerIterator(T* begin, T* end, T* current)
        : begin{begin}, end{end}, current{current}
      {}

      PointerIterator& operator++()
      {
        if(current != end)
          current++;
        return *this;
      }

      PointerIterator operator++(int)
      {
        PointerIterator last = *this;
        if(current != end)
          current++;
        return last;
      }

      PointerIterator& operator--()
      {
        if(current != begin)
          current--;
        return *this;
      }

      PointerIterator operator--(int)
      {
        PointerIterator last = *this;
        if(current != begin)
          current--;
        return last;
      }

      T& operator*()
      {
        return *current;
      }

      bool operator!=(const PointerIterator& other)
      {
        return current != other.current;
      }

      bool operator==(const PointerIterator& other)
      {
        return current == other.current;
      }
  };

  template <typename T>
  class Pointer final
  {
    public:
      template<typename S>
      friend class Pointer;
      uint* refCount;

    private:
      // Element count
      uint size;
      T* data;

    public:
      Pointer(uint size)
        : Pointer{nullptr, size}
      {
      }

      Pointer(T* data, uint size)
        : size{size}, data{(T*)malloc(size * sizeof(T))}, refCount{new uint{1}}
      {
        if(data != nullptr)
          memcpy(this->data, data, size * sizeof(T));
      }

      Pointer(const std::initializer_list<T>& list)
        : size{list.size()}, data{(T*)malloc(size * sizeof(T))}, refCount{new uint{1}}
      {
        memcpy(data, list.begin(), size * sizeof(T));
      }

      Pointer(const std::vector<T>& vec)
        : size{vec.size()}, data{(T*)malloc(size * sizeof(T))}, refCount{new uint{1}}
      {
        memcpy(data, vec.data(), size * sizeof(T));
      }

      template <typename S>
      Pointer(Pointer<S>&& pointer)
      : size{pointer.size}, data{(T*)pointer.data}, refCount{pointer.refCount}
      {
        ASSERT(pointer.data, "Moving invalid Pointer");
        ASSERT((pointer.size * sizeof(S)) % sizeof(T) == 0, "Pointer doesn't match new type");
        size = pointer.size * sizeof(S) / sizeof(T);
        pointer.Invalidate();
      }

      template <typename S>
      Pointer(const Pointer<S>& pointer)
      : size{pointer.size}, data{(T*)pointer.data}, refCount{pointer.refCount}
      {
        ASSERT(pointer.data, "Copying invalid Pointer", __LINE__);
        ASSERT((pointer.size * sizeof(S)) % sizeof(T) == 0, "Pointer doesn't match new type");
        size = pointer.size * sizeof(S) / sizeof(T);
        ++*refCount;
      }

      ~Pointer()
      {
        if(refCount)
        {
          --*refCount;
          if(*refCount == 0)
          {
            free(data);
            delete refCount;
          }
        }
      }

      template <typename S>
      Pointer& operator=(Pointer<S>&& pointer)
      {
        ASSERT(pointer.data, "Moving invalid Pointer");
        ASSERT((pointer.size * sizeof(S)) % sizeof(T) == 0, "Pointer doesn't match new type");
        size = pointer.size * sizeof(S) / sizeof(T);
        size = pointer.size;
        data = (T*)pointer.data;
        refCount = pointer.refCount;
        pointer.Invalidate();
        return *this;
      }

      template <typename S>
      Pointer& operator=(const Pointer<S>& pointer)
      {
        ASSERT(pointer.data, "Copying invalid Pointer");
        ASSERT((pointer.size * sizeof(S)) % sizeof(T) == 0, "Pointer doesn't match new type");
        size = pointer.size * sizeof(S) / sizeof(T);
        size = pointer.size;
        data = (T*)pointer.data;
        refCount = pointer.refCount;
        ++*refCount;
        return *this;
      }

      T& At(uint i)
      {
        ASSERT(data, "Indexing invalid Pointer");
        ASSERT(i < size, "Index out of bound in Pointer");
        return *(data + i);
      }

      const T& At(uint i) const
      {
        return At(i);
      }

      T& operator[](uint i)
      {
        return At(i);
      }

      const T& operator[](uint i) const
      {
        return At(i);
      }

      uint Size()
      {
        return size;
      }

      uint PointerSize()
      {
        return size * sizeof(T);
      }

      Pointer Copy(const Pointer& pointer)
      {
        return Pointer(pointer.data, pointer.size);
      }

      T* Data()
      {
        return (T*)data;
      }

      PointerIterator<T> begin() const
      {
        return PointerIterator(data, data + size, data);
      }

      PointerIterator<T> Begin() const
      {
        return PointerIterator(data, data + size, data);
      }

      PointerIterator<T> end() const
      {
        return PointerIterator(data, data + size, data + size);
      }

      PointerIterator<T> End() const
      {
        return PointerIterator(data, data + size, data + size);
      }

    private:
      void Invalidate()
      {
        size = 0;
        data = nullptr;
        refCount = nullptr;
      }
  };
}

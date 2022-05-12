#pragma once

namespace neith {

void** oldAlloc2DArr(int x, int y, int typeSize);

template <typename T>
T** Alloc2DArr(int x, int y)
{
    T** arr = new T*[x];
    T* arr2 = new T[x * y];

    for (int i = 0; i < x; i++) {
        arr[i] = &arr2[i * y];
    }
    return arr;
}

// void Del2DArr(void** arr);

}  // namespace neith

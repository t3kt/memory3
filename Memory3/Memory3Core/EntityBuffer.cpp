#include "EntityBuffer.h"

void IndexAllocator::reset(std::size_t newSize) {
  while (!_free.empty()) {
    _free.pop();
  }
  _size = newSize;
  for (std::size_t i = 0; i < _size; ++i) {
    _free.push(i);
  }
}

void IndexAllocator::release(std::size_t index) {
  _free.push(index);
}

std::size_t IndexAllocator::request() {
  if (full()) {
    return -1;
  }
  auto index = _free.front();
  _free.pop();
  return index;
}

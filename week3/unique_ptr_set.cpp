template <class T>
class ObjectPool {
public:
  T* Allocate();
  T* TryAllocate();

  void Deallocate(T* object);

private:
  // Определили свой компаратор
  struct Compare
  {
    using is_transparent = void;  // Сделали компаратор прозрачным
   
    // Использовали стандартное сравнение для ключей
    bool operator()(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) const {
      return lhs < rhs;
    }
    // Определили функции сравнения ключа и обычного указателя
    bool operator()(const unique_ptr<T>& lhs, const T* rhs) const {
      return less<const T*>()(lhs.get(), rhs);
    }
    bool operator()(const T* lhs, const unique_ptr<T>& rhs) const {
      return less<const T*>()(lhs, rhs.get());
    }
  };

  queue<unique_ptr<T>> free;
  set<unique_ptr<T>, Compare> allocated;  // Изменили контейнер на set и указали
                                          // компаратор вторым аргументом
};

template <typename T>
T* ObjectPool<T>::Allocate() {
  if (free.empty()) {
    free.push(make_unique<T>());
  }
  auto ptr = move(free.front());
  free.pop();
  T* ret = ptr.get();
  allocated.insert(move(ptr));  // Изменили вставку
  return ret;
}

template <typename T>
T* ObjectPool<T>::TryAllocate() {
  if (free.empty()) {
    return nullptr;
  }
  return Allocate();
}

template <typename T>
void ObjectPool<T>::Deallocate(T* object) {
  auto it = allocated.find(object);
  if (it == allocated.end()) {
    throw invalid_argument("");
  }
  // Изменили код перемещения
  free.push(move(allocated.extract(it).value()));
}
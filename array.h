template<typename T>
class Array final {
public:
    Array();
    ~Array();

    Array(int capacity);

    int insert(const T& value);
    int insert(int index, const T& value);

    void remove(int index);

    const T& operator[](int index) const;
    T& operator[](int index);

    int size() const;

    class Iterator {
    public:
        Iterator(T *ptr);

        const T& get() const; // Получает значение массива в текущей позиции итератора.
        void set(const T& value); // Устанавливает значение в текущей позиции итератора.
        void next(); //Перемещает текущую позицию итератора на следующий элемент.
        bool hasNext() const; //Возвращает true, если итератор может перейти к следующему элементу, или false в противном случае.
    private:
        T *m_dataPtr;
        Iterator(T *ptr, bool isReverse);

        bool m_isReverse;
    };
    using ConstIterator = const Iterator;

    Iterator iterator();
    ConstIterator iterator() const;

    Iterator reverseIterator();
    ConstIterator reverseIterator() const;
private:
    T *data;
    int capacity;
    int typeSize;
    int currCount = 0;
};
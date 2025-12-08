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
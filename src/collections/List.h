#ifndef LIST_H
#define LIST_H

#include <QObject>
#include <QList>
#include <functional>
#include <algorithm>

template <typename T>
class List : public QList<T>
{
public:
    template <typename... Args>
    List(Args... params);

    List(std::initializer_list<T> params);

    List<T> filtered(std::function<bool(T)> f) const;
    List<T> filtered(std::function<bool(T, int)> f) const;

    List<T> mapped(std::function<T(T)> f) const;
    List<T> mapped(std::function<T(T, int)> f) const;

    template <typename U>
    List<U> mappedTo(std::function<U(T)> f) const;
    template <typename U>
    List<U> mappedTo(std::function<U(T, int)> f) const;

    void forEach(std::function<void(T)> f) const;
    void forEach(std::function<void(T, int)> f) const;

    List<T> sorted(std::function<bool (T, T)> f = std::less<T>()) const;

    template<typename U>
    U reducedTo(std::function<U(U, T)> f, U initial) const;
    template<typename U>
    U reducedTo(std::function<U(U, T, int)> f, U initial) const;
};


template <typename T>
template <typename... Args>
List<T>::List(Args... params)
    : QList<T>(params...)
{

}

template <typename T>
List<T>::List(std::initializer_list<T> params)
    : QList<T>(params)
{

}

template <typename T>
List<T> List<T>::filtered(std::function<bool(T)> f) const
{
    return filtered([f](T item, int index) {
        Q_UNUSED(index);
        return f(item);
    });
}

template <typename T>
List<T> List<T>::filtered(std::function<bool(T, int)> f) const
{
    List<T> result;
    int i = 0;
    for (auto it = QList<T>::constBegin(); it != QList<T>::constEnd(); ++it, ++i) {
        if (f(*it, i)) {
            result.append(*it);
        }
    }
    return result;
}

template <typename T>
List<T> List<T>::mapped(std::function<T(T)> f) const
{
    return mapped([f](T item, int index) {
        Q_UNUSED(index);
        return f(item);
    });
}

template <typename T>
List<T> List<T>::mapped(std::function<T(T, int)> f) const
{
    return mappedTo<T>(f);
}

template <typename T>
template <typename U>
List<U> List<T>::mappedTo(std::function<U(T)> f) const
{
    return mappedTo<U>([f](T item, int index) {
        Q_UNUSED(index);
        return f(item);
    });
}

template <typename T>
template <typename U>
List<U> List<T>::mappedTo(std::function<U(T, int)> f) const
{
    List<U> result;
    int i = 0;
    for (auto it = QList<T>::constBegin(); it != QList<T>::constEnd(); ++it, ++i) {
        result.append(f(*it, i));
    }
    return result;
}

template <typename T>
void List<T>::forEach(std::function<void(T)> f) const
{
    return forEach([f](T item, int index) {
        Q_UNUSED(index);
        f(item);
    });
}

template <typename T>
void List<T>::forEach(std::function<void(T, int)> f) const
{
    int i = 0;
    for (auto it = QList<T>::constBegin(); it != QList<T>::constEnd(); ++it, ++i) {
        f(*it, i);
    }
}

template <typename T>
List<T> List<T>::sorted(std::function<bool(T, T)> f) const
{
    List<T> listCopy = *this;
    std::stable_sort(listCopy.begin(), listCopy.end(), f);
    return listCopy;
}

template <typename T>
template <typename U>
U List<T>::reducedTo(std::function<U(U, T)> f, U initial) const
{
    return reducedTo([f](U container, T item, int index) {
        Q_UNUSED(index);
        return f(container, item);
    });
}

template <typename T>
template <typename U>
U List<T>::reducedTo(std::function<U(U, T, int)> f, U initial) const
{
    U result = initial;

    int i = 0;
    for (auto it = QList<T>::constBegin(); it != QList<T>::constEnd(); ++it, ++i) {
        result = f(result, *it, i);
    }

    return result;
}

#endif // LIST_H

#ifndef LIST_H
#define LIST_H

#include <QObject>
#include <QList>
#include <functional>

template <typename T>
class List : public QList<T>
{
public:
    template <typename... Args>
    List(Args... params);

    List<T> map(std::function<T(T)> f) const;
    List<T> map(std::function<T(T, int)> f) const;

    template <typename U>
    List<U> mapTo(std::function<U(T)> f) const;
    template <typename U>
    List<U> mapTo(std::function<U(T, int)> f) const;
};


template <typename T>
template <typename... Args>
List<T>::List(Args... params)
    : QList<T>(params...)
{

}

template <typename T>
List<T> List<T>::map(std::function<T(T)> f) const
{
    return map([f](T item, int index) {
        Q_UNUSED(index);
        return f(item);
    });
}

template <typename T>
List<T> List<T>::map(std::function<T(T, int)> f) const
{
    return mapTo<T>(f);
}

template <typename T>
template <typename U>
List<U> List<T>::mapTo(std::function<U(T)> f) const
{
    return mapTo<U>([f](T item, int index) {
        Q_UNUSED(index);
        return f(item);
    });
}

template <typename T>
template <typename U>
List<U> List<T>::mapTo(std::function<U(T, int)> f) const
{
    List<U> result;
    int i = 0;
    for (auto it = QList<T>::constBegin(); it != QList<T>::constEnd(); ++it, ++i) {
        result.append(f(*it, i));
    }
    return result;
}


#endif // LIST_H

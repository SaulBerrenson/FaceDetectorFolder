#pragma once
#include <qdir.h>
#include <qstring.h>
#include <qsharedpointer.h>
#include <qscopedpointer.h>
#include <QWeakPointer>
#include <QQueue>


using String = QString;

#define  Separator = QDir::separator();


template<class T>
using List = QVector<T>;

template <class Key,class Value>
using Dictionary = QMap<Key, Value>;

template<class T>
using Queue = QQueue<T>;
//////////////////////////////////////////
template<class T>
using SharedPtr = QSharedPointer<T>;
template<class T>
using UniquePtr = QScopedPointer<T>;
template<class T>
using WeakPtr = QWeakPointer<T>;
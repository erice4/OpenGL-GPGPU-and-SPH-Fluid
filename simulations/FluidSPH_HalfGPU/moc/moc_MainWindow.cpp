/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 11), // "setSimulate"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 2), // "_s"
QT_MOC_LITERAL(4, 27, 8), // "setReset"
QT_MOC_LITERAL(5, 36, 8), // "setWindX"
QT_MOC_LITERAL(6, 45, 2), // "_x"
QT_MOC_LITERAL(7, 48, 8), // "setWindY"
QT_MOC_LITERAL(8, 57, 2), // "_y"
QT_MOC_LITERAL(9, 60, 9), // "setKernel"
QT_MOC_LITERAL(10, 70, 2), // "_v"
QT_MOC_LITERAL(11, 73, 14), // "setRestDensity"
QT_MOC_LITERAL(12, 88, 15), // "setLinViscosity"
QT_MOC_LITERAL(13, 104, 16), // "setQuadViscosity"
QT_MOC_LITERAL(14, 121, 12), // "setStiffness"
QT_MOC_LITERAL(15, 134, 16), // "setNearStiffness"
QT_MOC_LITERAL(16, 151, 10) // "setGravity"

    },
    "MainWindow\0setSimulate\0\0_s\0setReset\0"
    "setWindX\0_x\0setWindY\0_y\0setKernel\0_v\0"
    "setRestDensity\0setLinViscosity\0"
    "setQuadViscosity\0setStiffness\0"
    "setNearStiffness\0setGravity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x08 /* Private */,
       4,    0,   72,    2, 0x08 /* Private */,
       5,    1,   73,    2, 0x08 /* Private */,
       7,    1,   76,    2, 0x08 /* Private */,
       9,    1,   79,    2, 0x08 /* Private */,
      11,    1,   82,    2, 0x08 /* Private */,
      12,    1,   85,    2, 0x08 /* Private */,
      13,    1,   88,    2, 0x08 /* Private */,
      14,    1,   91,    2, 0x08 /* Private */,
      15,    1,   94,    2, 0x08 /* Private */,
      16,    1,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   10,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setSimulate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setReset(); break;
        case 2: _t->setWindX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setWindY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setKernel((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setRestDensity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setLinViscosity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setQuadViscosity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setStiffness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setNearStiffness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setGravity((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

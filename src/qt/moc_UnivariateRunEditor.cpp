/****************************************************************************
** Meta object code from reading C++ file 'UnivariateRunEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "UnivariateRunEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UnivariateRunEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UnivariateRunEditor_t {
    QByteArrayData data[6];
    char stringdata[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_UnivariateRunEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_UnivariateRunEditor_t qt_meta_stringdata_UnivariateRunEditor = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 13),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 13),
QT_MOC_LITERAL(4, 49, 16),
QT_MOC_LITERAL(5, 66, 6)
    },
    "UnivariateRunEditor\0insertMaskRow\0\0"
    "removeMaskRow\0handleNextButton\0finish\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UnivariateRunEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a,
       3,    0,   35,    2, 0x0a,
       4,    0,   36,    2, 0x0a,
       5,    0,   37,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UnivariateRunEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UnivariateRunEditor *_t = static_cast<UnivariateRunEditor *>(_o);
        switch (_id) {
        case 0: _t->insertMaskRow(); break;
        case 1: _t->removeMaskRow(); break;
        case 2: _t->handleNextButton(); break;
        case 3: _t->finish(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject UnivariateRunEditor::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_UnivariateRunEditor.data,
      qt_meta_data_UnivariateRunEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *UnivariateRunEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UnivariateRunEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UnivariateRunEditor.stringdata))
        return static_cast<void*>(const_cast< UnivariateRunEditor*>(this));
    return QWizard::qt_metacast(_clname);
}

int UnivariateRunEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

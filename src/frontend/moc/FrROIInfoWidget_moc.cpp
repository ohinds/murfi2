/****************************************************************************
** Meta object code from reading C++ file 'FrROIInfoWidget.h'
**
** Created: Fri 19. Dec 16:12:14 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrROIInfoWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrROIInfoWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrROIInfoWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FrROIInfoWidget[] = {
    "FrROIInfoWidget\0"
};

const QMetaObject FrROIInfoWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrROIInfoWidget,
      qt_meta_data_FrROIInfoWidget, 0 }
};

const QMetaObject *FrROIInfoWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrROIInfoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrROIInfoWidget))
	return static_cast<void*>(const_cast< FrROIInfoWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrROIInfoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}

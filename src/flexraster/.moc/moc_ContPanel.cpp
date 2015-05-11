/****************************************************************************
** Meta object code from reading C++ file 'ContPanel.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ContPanel.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ContPanel.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ContPanel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x0a,
      42,   10,   10,   10, 0x0a,
      67,   10,   10,   10, 0x0a,
      80,   10,   10,   10, 0x0a,
      89,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ContPanel[] = {
    "ContPanel\0\0qre\0resizeEvent(QResizeEvent*)\0"
    "closeEvent(QCloseEvent*)\0updateData()\0"
    "polish()\0rezoom(int)\0"
};

void ContPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ContPanel *_t = static_cast<ContPanel *>(_o);
        switch (_id) {
        case 0: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 1: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 2: _t->updateData(); break;
        case 3: _t->polish(); break;
        case 4: _t->rezoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ContPanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ContPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ContPanel,
      qt_meta_data_ContPanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ContPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ContPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ContPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ContPanel))
        return static_cast<void*>(const_cast< ContPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int ContPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

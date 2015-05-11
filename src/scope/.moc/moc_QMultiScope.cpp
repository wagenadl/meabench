/****************************************************************************
** Meta object code from reading C++ file 'QMultiScope.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QMultiScope.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMultiScope.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMultiScope[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   13,   12,   12, 0x0a,
      39,   35,   12,   12, 0x0a,
      60,   12,   12,   12, 0x0a,
      74,   72,   12,   12, 0x0a,
      96,   89,   12,   12, 0x0a,
     119,  111,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QMultiScope[] = {
    "QMultiScope\0\0extent\0setExtent(int)\0"
    "spc\0setGuideSpacing(int)\0setCenter()\0"
    "s\0setSpeedy(int)\0length\0setLength(int)\0"
    "pretrig\0setPreTrig(int)\0"
};

void QMultiScope::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMultiScope *_t = static_cast<QMultiScope *>(_o);
        switch (_id) {
        case 0: _t->setExtent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setGuideSpacing((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setCenter(); break;
        case 3: _t->setSpeedy((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setLength((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setPreTrig((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QMultiScope::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMultiScope::staticMetaObject = {
    { &QMultiGraph::staticMetaObject, qt_meta_stringdata_QMultiScope,
      qt_meta_data_QMultiScope, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMultiScope::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMultiScope::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMultiScope::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMultiScope))
        return static_cast<void*>(const_cast< QMultiScope*>(this));
    return QMultiGraph::qt_metacast(_clname);
}

int QMultiScope::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMultiGraph::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'ContRaster.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ContRaster.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ContRaster.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ContRaster[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x0a,
      44,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ContRaster[] = {
    "ContRaster\0\0t0,dt\0move(long long,long long)\0"
    "dataUpdate()\0"
};

void ContRaster::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ContRaster *_t = static_cast<ContRaster *>(_o);
        switch (_id) {
        case 0: _t->move((*reinterpret_cast< long long(*)>(_a[1])),(*reinterpret_cast< long long(*)>(_a[2]))); break;
        case 1: _t->dataUpdate(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ContRaster::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ContRaster::staticMetaObject = {
    { &SimpleRaster::staticMetaObject, qt_meta_stringdata_ContRaster,
      qt_meta_data_ContRaster, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ContRaster::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ContRaster::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ContRaster::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ContRaster))
        return static_cast<void*>(const_cast< ContRaster*>(this));
    return SimpleRaster::qt_metacast(_clname);
}

int ContRaster::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SimpleRaster::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

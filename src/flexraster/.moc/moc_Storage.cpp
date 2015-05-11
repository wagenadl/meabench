/****************************************************************************
** Meta object code from reading C++ file 'Storage.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Storage.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Storage.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Storage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   18,    8,    8, 0x0a,
      69,   63,    8,    8, 0x2a,
      96,    8,    8,    8, 0x0a,
     107,  104,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Storage[] = {
    "Storage\0\0update()\0src,n,update\0"
    "add(const Spikeinfo*,uint,bool)\0src,n\0"
    "add(const Spikeinfo*,uint)\0reset()\0"
    "si\0identitrig(Spikeinfo)\0"
};

void Storage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Storage *_t = static_cast<Storage *>(_o);
        switch (_id) {
        case 0: _t->update(); break;
        case 1: _t->add((*reinterpret_cast< const Spikeinfo*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->add((*reinterpret_cast< const Spikeinfo*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 3: _t->reset(); break;
        case 4: _t->identitrig((*reinterpret_cast< const Spikeinfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Storage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Storage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Storage,
      qt_meta_data_Storage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Storage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Storage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Storage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Storage))
        return static_cast<void*>(const_cast< Storage*>(this));
    return QObject::qt_metacast(_clname);
}

int Storage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Storage::update()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

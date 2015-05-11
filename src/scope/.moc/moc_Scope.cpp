/****************************************************************************
** Meta object code from reading C++ file 'Scope.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Scope.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Scope.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Scope[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x0a,
      29,    6,    6,    6, 0x0a,
      53,    6,    6,    6, 0x0a,
      68,    6,    6,    6, 0x0a,
      85,    6,    6,    6, 0x0a,
     101,    6,    6,    6, 0x0a,
     114,    6,    6,    6, 0x0a,
     127,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Scope[] = {
    "Scope\0\0setRawSource(QString)\0"
    "setSpikeSource(QString)\0wakeupRaw(int)\0"
    "wakeupSpike(int)\0enableDbx(bool)\0"
    "freeze(bool)\0savefrozen()\0setLengthMS(int)\0"
};

void Scope::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Scope *_t = static_cast<Scope *>(_o);
        switch (_id) {
        case 0: _t->setRawSource((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setSpikeSource((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->wakeupRaw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->wakeupSpike((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->enableDbx((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->freeze((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->savefrozen(); break;
        case 7: _t->setLengthMS((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Scope::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Scope::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Scope,
      qt_meta_data_Scope, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Scope::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Scope::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Scope::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Scope))
        return static_cast<void*>(const_cast< Scope*>(this));
    return QWidget::qt_metacast(_clname);
}

int Scope::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'Spikesound.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Spikesound.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Spikesound.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Spikesound[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      29,   11,   11,   11, 0x0a,
      51,   11,   11,   11, 0x0a,
      65,   11,   11,   11, 0x0a,
      83,   11,   11,   11, 0x0a,
     102,   11,   11,   11, 0x0a,
     117,   11,   11,   11, 0x0a,
     128,   11,   11,   11, 0x0a,
     140,   11,   11,   11, 0x0a,
     152,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Spikesound[] = {
    "Spikesound\0\0setRethresh(int)\0"
    "setNegativeOnly(bool)\0setPlay(bool)\0"
    "useRethresh(bool)\0setSource(QString)\0"
    "setVolume(int)\0canWrite()\0setA1(bool)\0"
    "setA2(bool)\0setA3(bool)\0"
};

void Spikesound::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Spikesound *_t = static_cast<Spikesound *>(_o);
        switch (_id) {
        case 0: _t->setRethresh((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setNegativeOnly((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setPlay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->useRethresh((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setSource((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->canWrite(); break;
        case 7: _t->setA1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setA2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setA3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Spikesound::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Spikesound::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Spikesound,
      qt_meta_data_Spikesound, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Spikesound::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Spikesound::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Spikesound::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Spikesound))
        return static_cast<void*>(const_cast< Spikesound*>(this));
    return QWidget::qt_metacast(_clname);
}

int Spikesound::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

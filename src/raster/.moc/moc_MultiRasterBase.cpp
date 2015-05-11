/****************************************************************************
** Meta object code from reading C++ file 'MultiRasterBase.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MultiRasterBase.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MultiRasterBase.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MultiRasterBase[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      45,   16,   16,   16, 0x05,
      65,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   16,   16,   16, 0x0a,
     112,  106,   16,   16, 0x0a,
     133,  126,   16,   16, 0x0a,
     150,  148,   16,   16, 0x0a,
     173,  169,   16,   16, 0x0a,
     193,  169,   16,   16, 0x0a,
     212,   16,   16,   16, 0x0a,
     239,  236,   16,   16, 0x0a,
     257,  236,   16,   16, 0x0a,
     281,  276,   16,   16, 0x0a,
     299,   16,   16,   16, 0x0a,
     312,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MultiRasterBase[] = {
    "MultiRasterBase\0\0reportSourceStatus(QString)\0"
    "reportTime(QString)\0reportTriggers(QString)\0"
    "wakeupSpike(int)\0prems\0setPreMS(int)\0"
    "postms\0setPostMS(int)\0n\0setPixPerTrig(int)\0"
    "ord\0setTrigChannel(int)\0setSecChannel(int)\0"
    "setSpikeSource(QString)\0on\0setDownOnly(bool)\0"
    "setUseThresh(bool)\0perc\0setThreshVal(int)\0"
    "freeze(bool)\0autotime()\0"
};

void MultiRasterBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MultiRasterBase *_t = static_cast<MultiRasterBase *>(_o);
        switch (_id) {
        case 0: _t->reportSourceStatus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->reportTime((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->reportTriggers((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->wakeupSpike((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setPreMS((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setPostMS((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setPixPerTrig((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setTrigChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setSecChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setSpikeSource((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->setDownOnly((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->setUseThresh((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->setThreshVal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->freeze((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->autotime(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MultiRasterBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MultiRasterBase::staticMetaObject = {
    { &QMultiGraph::staticMetaObject, qt_meta_stringdata_MultiRasterBase,
      qt_meta_data_MultiRasterBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MultiRasterBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MultiRasterBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MultiRasterBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MultiRasterBase))
        return static_cast<void*>(const_cast< MultiRasterBase*>(this));
    return QMultiGraph::qt_metacast(_clname);
}

int MultiRasterBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMultiGraph::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void MultiRasterBase::reportSourceStatus(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultiRasterBase::reportTime(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MultiRasterBase::reportTriggers(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

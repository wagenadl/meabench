/****************************************************************************
** Meta object code from reading C++ file 'QMultiGraph.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QMultiGraph.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMultiGraph.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMultiGraph[] = {

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
      17,   13,   12,   12, 0x0a,
      40,   36,   12,   12, 0x0a,
      58,   36,   12,   12, 0x0a,
      75,   36,   12,   12, 0x0a,
      93,   36,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QMultiGraph[] = {
    "QMultiGraph\0\0lay\0setLayout(QString)\0"
    "pen\0setTracePen(QPen)\0setZeroPen(QPen)\0"
    "setGuidePen(QPen)\0setAuxPen(QPen)\0"
};

void QMultiGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMultiGraph *_t = static_cast<QMultiGraph *>(_o);
        switch (_id) {
        case 0: _t->setLayout((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setTracePen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 2: _t->setZeroPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 3: _t->setGuidePen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 4: _t->setAuxPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QMultiGraph::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMultiGraph::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QMultiGraph,
      qt_meta_data_QMultiGraph, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMultiGraph::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMultiGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMultiGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMultiGraph))
        return static_cast<void*>(const_cast< QMultiGraph*>(this));
    return QFrame::qt_metacast(_clname);
}

int QMultiGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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

/****************************************************************************
** Meta object code from reading C++ file 'LinePanel.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LinePanel.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinePanel.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LinePanel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      38,   10,   10,   10, 0x0a,
      63,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LinePanel[] = {
    "LinePanel\0\0resizeEvent(QResizeEvent*)\0"
    "closeEvent(QCloseEvent*)\0"
    "enable(const RD_Stim*)\0"
};

void LinePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LinePanel *_t = static_cast<LinePanel *>(_o);
        switch (_id) {
        case 0: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 1: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 2: _t->enable((*reinterpret_cast< const RD_Stim*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LinePanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LinePanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LinePanel,
      qt_meta_data_LinePanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LinePanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LinePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LinePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LinePanel))
        return static_cast<void*>(const_cast< LinePanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int LinePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

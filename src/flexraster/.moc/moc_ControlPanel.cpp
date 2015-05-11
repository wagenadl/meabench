/****************************************************************************
** Meta object code from reading C++ file 'ControlPanel.H'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ControlPanel.H"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlPanel.H' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ControlPanel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      27,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   13,   13,   13, 0x0a,
      54,   13,   13,   13, 0x0a,
      70,   13,   13,   13, 0x0a,
      87,   13,   13,   13, 0x0a,
     102,   13,   13,   13, 0x0a,
     117,   13,   13,   13, 0x0a,
     131,   13,   13,   13, 0x0a,
     145,   13,   13,   13, 0x0a,
     168,   13,   13,   13, 0x0a,
     184,   13,   13,   13, 0x0a,
     201,   13,   13,   13, 0x0a,
     217,   13,   13,   13, 0x0a,
     235,   13,   13,   13, 0x0a,
     254,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ControlPanel[] = {
    "ControlPanel\0\0updateinfo()\0newsource()\0"
    "create_spont()\0create_8x8rec()\0"
    "create_8x8stim()\0create_vstim()\0"
    "create_hstim()\0create_cont()\0update_time()\0"
    "update_source(QString)\0update_pre(int)\0"
    "update_post(int)\0update_up(bool)\0"
    "update_down(bool)\0update_pixtri(int)\0"
    "wakeup()\0"
};

void ControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ControlPanel *_t = static_cast<ControlPanel *>(_o);
        switch (_id) {
        case 0: _t->updateinfo(); break;
        case 1: _t->newsource(); break;
        case 2: _t->create_spont(); break;
        case 3: _t->create_8x8rec(); break;
        case 4: _t->create_8x8stim(); break;
        case 5: _t->create_vstim(); break;
        case 6: _t->create_hstim(); break;
        case 7: _t->create_cont(); break;
        case 8: _t->update_time(); break;
        case 9: _t->update_source((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->update_pre((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->update_post((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->update_up((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->update_down((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->update_pixtri((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->wakeup(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ControlPanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ControlPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ControlPanel,
      qt_meta_data_ControlPanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ControlPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanel))
        return static_cast<void*>(const_cast< ControlPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int ControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void ControlPanel::updateinfo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ControlPanel::newsource()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE

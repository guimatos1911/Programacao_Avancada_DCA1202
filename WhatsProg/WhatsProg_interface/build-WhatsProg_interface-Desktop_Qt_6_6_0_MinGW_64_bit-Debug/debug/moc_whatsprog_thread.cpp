/****************************************************************************
** Meta object code from reading C++ file 'whatsprog_thread.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WhatsProg_interface/whatsprog_thread.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'whatsprog_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS = QtMocHelpers::stringData(
    "WhatsProgThread",
    "signExibirConversas",
    "",
    "signExibirMensagens",
    "signExibirErroMensagem",
    "std::string",
    "signDesconectarInterface",
    "slotIniciar",
    "slotEncerrar"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[16];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[23];
    char stringdata5[12];
    char stringdata6[25];
    char stringdata7[12];
    char stringdata8[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS_t qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "WhatsProgThread"
        QT_MOC_LITERAL(16, 19),  // "signExibirConversas"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 19),  // "signExibirMensagens"
        QT_MOC_LITERAL(57, 22),  // "signExibirErroMensagem"
        QT_MOC_LITERAL(80, 11),  // "std::string"
        QT_MOC_LITERAL(92, 24),  // "signDesconectarInterface"
        QT_MOC_LITERAL(117, 11),  // "slotIniciar"
        QT_MOC_LITERAL(129, 12)   // "slotEncerrar"
    },
    "WhatsProgThread",
    "signExibirConversas",
    "",
    "signExibirMensagens",
    "signExibirErroMensagem",
    "std::string",
    "signDesconectarInterface",
    "slotIniciar",
    "slotEncerrar"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWhatsProgThreadENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    1 /* Public */,
       3,    0,   51,    2, 0x06,    2 /* Public */,
       4,    1,   52,    2, 0x06,    3 /* Public */,
       6,    0,   55,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   56,    2, 0x0a,    6 /* Public */,
       8,    0,   57,    2, 0x0a,    7 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject WhatsProgThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWhatsProgThreadENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WhatsProgThread, std::true_type>,
        // method 'signExibirConversas'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signExibirMensagens'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signExibirErroMensagem'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::string &, std::false_type>,
        // method 'signDesconectarInterface'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slotIniciar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slotEncerrar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void WhatsProgThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WhatsProgThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signExibirConversas(); break;
        case 1: _t->signExibirMensagens(); break;
        case 2: _t->signExibirErroMensagem((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 3: _t->signDesconectarInterface(); break;
        case 4: _t->slotIniciar(); break;
        case 5: _t->slotEncerrar(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WhatsProgThread::*)();
            if (_t _q_method = &WhatsProgThread::signExibirConversas; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WhatsProgThread::*)();
            if (_t _q_method = &WhatsProgThread::signExibirMensagens; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WhatsProgThread::*)(const std::string & );
            if (_t _q_method = &WhatsProgThread::signExibirErroMensagem; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WhatsProgThread::*)();
            if (_t _q_method = &WhatsProgThread::signDesconectarInterface; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *WhatsProgThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WhatsProgThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWhatsProgThreadENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WhatsProgThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void WhatsProgThread::signExibirConversas()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WhatsProgThread::signExibirMensagens()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WhatsProgThread::signExibirErroMensagem(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WhatsProgThread::signDesconectarInterface()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP

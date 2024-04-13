/****************************************************************************
** Meta object code from reading C++ file 'whatsprog_login.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WhatsProg_interface/whatsprog_login.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'whatsprog_login.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS = QtMocHelpers::stringData(
    "WhatsProgLogin",
    "signConectar",
    "",
    "IP",
    "login",
    "senha",
    "novoUsuario",
    "slotLogin",
    "NovoUsuario",
    "on_buttonBox_accepted"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[15];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[3];
    char stringdata4[6];
    char stringdata5[6];
    char stringdata6[12];
    char stringdata7[10];
    char stringdata8[12];
    char stringdata9[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS_t qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "WhatsProgLogin"
        QT_MOC_LITERAL(15, 12),  // "signConectar"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 2),  // "IP"
        QT_MOC_LITERAL(32, 5),  // "login"
        QT_MOC_LITERAL(38, 5),  // "senha"
        QT_MOC_LITERAL(44, 11),  // "novoUsuario"
        QT_MOC_LITERAL(56, 9),  // "slotLogin"
        QT_MOC_LITERAL(66, 11),  // "NovoUsuario"
        QT_MOC_LITERAL(78, 21)   // "on_buttonBox_accepted"
    },
    "WhatsProgLogin",
    "signConectar",
    "",
    "IP",
    "login",
    "senha",
    "novoUsuario",
    "slotLogin",
    "NovoUsuario",
    "on_buttonBox_accepted"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWhatsProgLoginENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    4,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,   41,    2, 0x0a,    6 /* Public */,
       9,    0,   44,    2, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject WhatsProgLogin::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWhatsProgLoginENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WhatsProgLogin, std::true_type>,
        // method 'signConectar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'slotLogin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_buttonBox_accepted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void WhatsProgLogin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WhatsProgLogin *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signConectar((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        case 1: _t->slotLogin((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->on_buttonBox_accepted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WhatsProgLogin::*)(QString , QString , QString , bool );
            if (_t _q_method = &WhatsProgLogin::signConectar; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *WhatsProgLogin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WhatsProgLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWhatsProgLoginENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int WhatsProgLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WhatsProgLogin::signConectar(QString _t1, QString _t2, QString _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP

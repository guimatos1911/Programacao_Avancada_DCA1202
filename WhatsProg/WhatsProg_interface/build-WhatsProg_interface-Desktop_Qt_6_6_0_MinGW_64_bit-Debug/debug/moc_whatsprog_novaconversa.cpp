/****************************************************************************
** Meta object code from reading C++ file 'whatsprog_novaconversa.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WhatsProg_interface/whatsprog_novaconversa.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'whatsprog_novaconversa.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS = QtMocHelpers::stringData(
    "WhatsProgNovaConversa",
    "signAtualizaConversas",
    "",
    "signAtualizaMensagens",
    "on_buttonBox_accepted"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[22];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS_t qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS = {
    {
        QT_MOC_LITERAL(0, 21),  // "WhatsProgNovaConversa"
        QT_MOC_LITERAL(22, 21),  // "signAtualizaConversas"
        QT_MOC_LITERAL(44, 0),  // ""
        QT_MOC_LITERAL(45, 21),  // "signAtualizaMensagens"
        QT_MOC_LITERAL(67, 21)   // "on_buttonBox_accepted"
    },
    "WhatsProgNovaConversa",
    "signAtualizaConversas",
    "",
    "signAtualizaMensagens",
    "on_buttonBox_accepted"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWhatsProgNovaConversaENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    1 /* Public */,
       3,    0,   33,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   34,    2, 0x08,    3 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject WhatsProgNovaConversa::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWhatsProgNovaConversaENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WhatsProgNovaConversa, std::true_type>,
        // method 'signAtualizaConversas'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signAtualizaMensagens'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_buttonBox_accepted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void WhatsProgNovaConversa::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WhatsProgNovaConversa *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signAtualizaConversas(); break;
        case 1: _t->signAtualizaMensagens(); break;
        case 2: _t->on_buttonBox_accepted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WhatsProgNovaConversa::*)();
            if (_t _q_method = &WhatsProgNovaConversa::signAtualizaConversas; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WhatsProgNovaConversa::*)();
            if (_t _q_method = &WhatsProgNovaConversa::signAtualizaMensagens; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *WhatsProgNovaConversa::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WhatsProgNovaConversa::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWhatsProgNovaConversaENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int WhatsProgNovaConversa::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void WhatsProgNovaConversa::signAtualizaConversas()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WhatsProgNovaConversa::signAtualizaMensagens()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP

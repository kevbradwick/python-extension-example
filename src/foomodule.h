//
// Created by Kevin Bradwick on 21/09/2015.
//

#ifndef PYEXTENSION_FOOMODULE_H
#define PYEXTENSION_FOOMODULE_H

#include <Python.h>

static PyObject *sum(PyObject *self, PyObject *args);

static PyObject *greet(PyObject *self, PyObject *args, PyObject *kwargs);

PyMODINIT_FUNC PyInit_foo(void);

// this is the C representation of the type
typedef struct {
    PyObject_HEAD
    /* Add any type specific fields here */
} foo_FooObject;

// this is the definition of the Python type, it defines things like its name,
// implementation of methods like __str__ and __call__
static PyTypeObject foo_FooType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "foo.Foo",                      /* the name of the object */
    sizeof(foo_FooObject),          /* size of object */
    0,                              /* tp_itemsize */
    0,                              /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
    "Foo object"                    /* tp_doc */
};

#endif //PYEXTENSION_FOOMODULE_H

//
// Created by Kevin Bradwick on 21/09/2015.
//

#ifndef PYEXTENSION_FOOMODULE_H
#define PYEXTENSION_FOOMODULE_H

#include <Python.h>

static PyObject *sum(PyObject *self, PyObject *args);

static PyObject *greet(PyObject *self, PyObject *args, PyObject *kwargs);

PyMODINIT_FUNC PyInit_foo(void);

#endif //PYEXTENSION_FOOMODULE_H

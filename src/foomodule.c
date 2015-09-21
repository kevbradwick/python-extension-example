//
// Foo module.
//
// A project for learning how to extend Python. This module is based on
// Python 3.
//
// Ref: https://docs.python.org/3.5/extending/

#include "foomodule.h"

static PyObject *FooError;

// sum is a function in the foo module, so will be available as foo.sum.
//
// a C function that appears in a Python module must return a PyObject value.
// In this case, the function will calculate the sum of all arguments passed in
// in as a tuple e.g.
//
// >>> foo.sum(1, 1, 1, 2, 5)
// 10
static PyObject *sum(PyObject *self, PyObject *args) {
    // the total number of arguments passed
    Py_ssize_t total_args = PyTuple_Size(args);

    // initialise an array of ints to hold the arguments
    long total = 0;

    for (int i = 0; i < total_args; i++) {
        // get the argument at the loop position i
        PyObject *item = PyTuple_GetItem(args, i);
        Py_INCREF(item);
        // check if this is item is an int, if it is not then we should raise
        // an exception.
        if (!PyLong_Check(item)) {
            PyErr_SetString(FooError, "Only integers are allowed in sum()");
            Py_DECREF(item);
            return NULL;
        } else {
            total += PyLong_AsLong(item);
        }
        Py_DECREF(item);
    }

    return Py_BuildValue("i", total);
}

// build an array of Python method definitions
static PyMethodDef FooMethods[] = {
    {"sum", sum, METH_VARARGS, "Calculate the sum of all integers"},
    {NULL, NULL, 0, NULL}
};

// the module definition itself
static struct PyModuleDef foo_module = {
    PyModuleDef_HEAD_INIT,
    "foo",
    NULL,
    -1,
    FooMethods
};

// the module initialisation function. When the code import foo for the first
// time, this function will get called.
PyMODINIT_FUNC PyInit_foo(void)
{
    PyObject *m = PyModule_Create(&foo_module);
    if (m == NULL) {
        return NULL;
    }

    FooError = PyErr_NewException("foo.error", NULL, NULL);
    Py_INCREF(FooError);
    PyModule_AddObject(m, "error", FooError);
    return m;
}
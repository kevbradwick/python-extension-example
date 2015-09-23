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
        PyObject *item = PyTuple_GetItem(args, i);  // this is a 'borrowed' reference
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

// greet simply returns a string that will say 'Hello [name]'.
//
// this is a demonstration of how keyword arguments are extracted and used.
static PyObject *greet(PyObject *self, PyObject *args, PyObject *kwargs)
{
    char *name;

    static char *kwlist[] = {"name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &name)) {
        PyErr_SetString(FooError, "Who should I be greeting?");
        return NULL;
    }

    // buffer length is the length of the name, Hello\s! + one extra character
    // so that the string can be null terminated
    long str_len = strlen(name) + 7 + 1;
    char message[str_len];
    strcpy(message, "Hello ");
    strcat(message, name);
    strcat(message, "!");

    return Py_BuildValue("s", message);
}

// build an array of Python method definitions
static PyMethodDef FooMethods[] = {
    {"sum", (PyCFunction) sum, METH_VARARGS, "Calculate the sum of all integers"},
    {"greet", (PyCFunction) greet, METH_VARARGS | METH_KEYWORDS, "Print a hello greeting for [name]"},
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
    foo_FooType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&foo_FooType) < 0) {
        return NULL;
    }

    PyObject *m = PyModule_Create(&foo_module);
    if (m == NULL) {
        return NULL;
    }

    FooError = PyErr_NewException("foo.error", NULL, NULL);
    Py_INCREF(FooError);
    Py_INCREF(&foo_FooType);
    PyModule_AddObject(m, "error", FooError);
    PyModule_AddObject(m, "Foo", (PyObject *)&foo_FooType);
    return m;
}
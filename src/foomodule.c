//
// Foo module.
//
// A project for learning how to extend Python. This module is based on
// Python 3.
//
// Ref: https://docs.python.org/3.5/extending/

#include "foomodule.h"
#include "structmember.h"

// this is the C representation of the type
typedef struct {
    PyObject_HEAD
    /* Add any type specific fields here */
    PyObject *first_name;
    PyObject *last_name;
} foo_FooObject;

// class constructor function
//
// this is called every time we create a new instance of Foo e.g. foo.Foo()
static PyObject *Foo_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    foo_FooObject *self;

    self = (foo_FooObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->first_name = PyUnicode_FromString("");

        // something went wrong and couldn't assign a value to first name so we
        // exit and decrement the counter for its pointer reference
        if (self->first_name == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        self->last_name = PyUnicode_FromString("");
        if (self->last_name == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}

static void Foo_dealloc(foo_FooObject *self)
{
    Py_XDECREF(self->first_name);
    Py_XDECREF(self->last_name);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static int Foo_init(foo_FooObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *first = NULL;
    PyObject *last = NULL;
    PyObject *tmp;

    static char *kw_list[] = {"first", "last", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|OO", kw_list, &first, &last)) {
        return -1;
    }

    if (first) {
        tmp = self->first_name;
        Py_INCREF(first);
        self->first_name = first;
        Py_XDECREF(tmp);
    }

    if (last) {
        tmp = self->last_name;
        Py_INCREF(last);
        self->last_name = last;
        Py_XDECREF(tmp);
    }

    return 0;
}

// class member property definition
static PyMemberDef Foo_members[] = {
    {"first_name", T_OBJECT_EX, offsetof(foo_FooObject, first_name), 0, "first name"},
    {"last_name", T_OBJECT_EX, offsetof(foo_FooObject, last_name), 0, "last name"},
    {NULL}
};

// defines a method on the instance called name()
//
// when called, it will concatenate the first and last names together and
// return a unicode string.
static PyObject *Foo_name(foo_FooObject *self)
{
    if (self->first_name == NULL) {
        PyErr_SetString(PyExc_AttributeError, "first_name");
        return NULL;
    }

    if (self->last_name == NULL) {
        PyErr_SetString(PyExc_AttributeError, "last_name");
        return NULL;
    }

    return PyUnicode_FromFormat("%S %S", self->first_name, self->last_name);
}

// type method definition
static PyMethodDef Foo_methods[] = {
    {"name", (PyCFunction)Foo_name, METH_NOARGS, "Return the full name"},
    {NULL}
};

// this is the definition of the Python type, it defines things like its name,
// implementation of methods like __str__ and __call__ as well as class
// properties.
static PyTypeObject foo_FooType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "foo.Foo",                      /* the name of the object */
    sizeof(foo_FooObject),          /* size of object */
    0,                              /* tp_itemsize */
    (destructor)Foo_dealloc,        /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT
        | Py_TPFLAGS_BASETYPE,      /* tp_flags */
    "Foo object",                   /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    Foo_methods,                    /* tp_methods */
    Foo_members,                    /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)Foo_init,             /* tp_init */
    0,                              /* tp_alloc */
    Foo_new,                        /* tp_new */
};


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
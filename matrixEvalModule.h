
#ifndef MATRIXEVALUATOR_MATRIXEVALMODULE_H
#define MATRIXEVALUATOR_MATRIXEVALMODULE_H
#include <Python.h>
#include <structmember.h>
#include "MatrixEvaluator.h"
#define MODULE_NAME "MatrixEval"

typedef struct {
    PyObject_HEAD
    int         m_value;
    MatrixEvaluator*    m_myclass;
} MyClassObject;

int MyClass_init(PyObject *self, PyObject *args, PyObject *kwds);
PyObject *MyClass_new(PyTypeObject *subtype, PyObject *args, PyObject *kwds);
void MyClass_dealloc(MyClassObject *self);
PyObject* MyClass_addOne(PyObject *self, PyObject *args);

static PyMethodDef MyClass_methods[] = {
        {"addOne", (PyCFunction)MyClass_addOne, METH_NOARGS,  PyDoc_STR("Return an incrmented integer")},
        {NULL, NULL} /* Sentinel */
};

static struct PyMemberDef MyClass_members[] = {
        {"value", T_INT, offsetof(MyClassObject, m_value)},
        {NULL} /* Sentinel */
};

static PyType_Slot MyClass_slots[] = {
        {Py_tp_new, (void*)MyClass_new},
        {Py_tp_init, (void*)MyClass_init},
        {Py_tp_dealloc, (void*)MyClass_dealloc},
        {Py_tp_members,  MyClass_members},
        {Py_tp_methods, MyClass_methods},
        {0, nullptr}
};

static PyType_Spec spec_myclass = {
        "EvalMatrix",                               // name
        sizeof(MyClassObject) + sizeof(MatrixEvaluator), // basicsize
        0,                                          // itemsize
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   // flags
        MyClass_slots                               // slots
};


#endif //MATRIXEVALUATOR_MATRIXEVALMODULE_H

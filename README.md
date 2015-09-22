Python C Extension Example
==========================

This is a little repository with practical examples of building an extension
for Python using the C API. This extension has been tested against Python 3.5.

### Building

    python setup.py build && python setup.py install --record files.txt

### Uninstalling

    cat files.txt | xargs rm -rf

from distutils.core import setup, Extension

module1 = Extension('foo', sources=['src/foomodule.c',])
setup(
    name='Foo',
    version='1.0',
    description='A foo module',
    ext_modules=[module1]
)
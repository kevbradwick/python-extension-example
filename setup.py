from distutils.core import setup, Extension

module1 = Extension('foo', sources=['src/foomodule.c'])
setup(
    name='Spam',
    version='1.0',
    description='A spam module',
    ext_modules=[module1]
)
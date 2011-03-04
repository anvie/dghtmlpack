try:
    from setuptools import setup, find_packages
except ImportError:
    from ez_setup import use_setuptools
    use_setuptools()
    from setuptools import setup, find_packages

from distutils.extension import Extension

module = Extension("dghtmlpack/htmlpacker",
                   sources = ['dghtmlpack/htmlpacker.c'])

setup(
    name='htmlpacker',
    version='0.1',
    description='Digaku HTML Packer',
    author='Robin Syihab',
    author_email='r@nosql.asia',
    url='',
    install_requires=[],
    setup_requires=["PasteScript>=1.6.3"],
    packages=find_packages(exclude=['ez_setup']),
    include_package_data=True,
    package_data={'':['*.so']},
    zip_safe=False,
    ext_modules = [module]
)


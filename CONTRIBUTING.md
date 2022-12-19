## Contributing

Welcome and thank you for your interest in contributing.


### Compiling Code

Project uses Qt 5.12 with Qt Creator being the preferred development
environment.

All code is situated in `src` directory.


#### Linux

To build project under Linux, the following packages are required (in addition
to Qt Creator):
  * build-essential
  * libqt5x11extras5-dev
  * qtbase5-dev
  * lintian

To build executables, one can use `make`:

    make clean
    make

To build debian package:

    make clean
    make package


### Contributing Fixes

Fixes can be contributed using pull requests. Do note project is under MIT
license and any contribution will inherit the same.

LF line ending is strongly preferred. To have Git check it for you, configure
`core.whitespace` setting:

    git config core.whitespace blank-at-eol,blank-at-eof,space-before-tab,cr-at-eol

All textual files should be encoded as UTF-8 without BOM.


### Coding style

For non code files (xml, etc), use the best judgment. Albeit, do note that `tab`
character should be avoided and a number of `space` characters should be used
instead (4 being preferred).

Code uses K&R coding style. Please do make sure any contribution follows the
coding style already in use.


# Thank You!

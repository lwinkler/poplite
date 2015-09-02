**Author** Laurent Winkler
**Date**   September 2015


Mandelbrot is an adaptation of the Mandelbrot example of Qt: 

	http://doc.qt.io/qt-5/qtcore-threads-mandelbrot-example.html


Build
-----
To build this example you need to parse the header files manually

	 ../../parser/popgen.py renderthread.hpp     -I/usr/include -I../.. -I. -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore
	 ../../parser/popgen.py mandelbrotwidget.hpp -I/usr/include -I../.. -I. -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore
	 qmake
	 make

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QImage>

// poplite includes
#include "class/system.hpp"
#include "com/accesspoint.hpp"
#include <boost/serialization/vector.hpp>


class QImage;
class MandelbrotWidget_iface;

namespace boost {
	namespace serialization {

			template<class Archive>
			static void serialize(Archive & ar, QSize & t, const unsigned int /*version*/)
			{
				ar & t.rwidth();
				ar & t.rheight();
			}

			static void serialize(boost::archive::text_oarchive & ar, QImage & t, const unsigned int /*version*/)
			{
				int width  = t.width();
				int height = t.height();
				int format = static_cast<int>(t.format());
				ar << width;
				ar << height;
				ar << format;
				for(int x = 0 ; x < width ; x++)
					for(int y = 0 ; y < height ; y++)
					{
						unsigned int val = t.pixel(x,y);
						ar << val;
					}
			}
			static void serialize(boost::archive::text_iarchive & ar, QImage & t, const unsigned int /*version*/)
			{
				int width = 0;
				int height = 0;
				int format = 0;
				ar >> width;
				ar >> height;
				ar >> format;
				unsigned int val = 0;
				QImage im(width, height, static_cast<QImage::Format>(format));
				for(int x = 0 ; x < width ; x++)
					for(int y = 0 ; y < height ; y++)
					{
						ar >> val;
						im.setPixel(x, y, val);
					}
				t = im;
			}
	}
}

POP_CLASS RenderThread // : protected QObject
{
public:
    RenderThread();
    ~RenderThread();
    void setReferenceToWidget(const pop::accesspoint& ap);

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize);

// signals:
    // void renderedImage(const QImage &image, double scaleFactor);

protected:
    void run(); //  Q_DECL_OVERRIDE;

private:
    uint rgbFromWaveLength(double wave);

    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;
    bool restart;
    bool abort;

    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];

    MandelbrotWidget_iface* p_widget;
};

#endif // RENDERTHREAD_H

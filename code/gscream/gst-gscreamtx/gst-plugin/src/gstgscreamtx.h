/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2018  <<user@hostname.org>>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_GSCREAMTX_H__
#define __GST_GSCREAMTX_H__

#include <gst/gst.h>
#include "RtpQueue.h"
#include "ScreamTx.h"
#include <pthread.h>


G_BEGIN_DECLS

/* #defines don't like whitespacey bits */
#define GST_TYPE_GSCREAMTX \
  (gst_g_scream_tx_get_type())
#define GST_GSCREAMTX(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_GSCREAMTX,GstgScreamTx))
#define GST_GSCREAMTX_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_GSCREAMTX,GstgScreamTxClass))
#define GST_IS_GSCREAMTX(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_GSCREAMTX))
#define GST_IS_GSCREAMTX_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_GSCREAMTX))

typedef struct _GstgScreamTx      GstgScreamTx;
typedef struct _GstgScreamTxClass GstgScreamTxClass;

struct _GstgScreamTx
{
  GstElement element;

  GstPad *sinkpad, *srcpad;

  gboolean silent;

  RtpQueue *rtpQueue;
  GObject *rtpSession;
  ScreamTx *screamTx;
  GstClockTime gstClockTimeRef;
  GstElement *encoder;
  float lastRateChangeT;
  pthread_mutex_t lock_rtp_queue;
  pthread_mutex_t lock_scream;
  GstClockID clockId;

  guint media_src;
  guint min_bitrate;
  guint start_bitrate;
  guint max_bitrate;
};

struct _GstgScreamTxClass
{
  GstElementClass parent_class;
};

GType gst_g_scream_tx_get_type (void);

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static void task_print_loop();

//static void
//    bind_communication (GstElement * scream);

#define gst_g_scream_tx_parent_class parent_class
G_DEFINE_TYPE (GstgScreamTx, gst_g_scream_tx, GST_TYPE_ELEMENT);

static void gst_g_scream_tx_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_g_scream_tx_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_g_scream_tx_sink_event (GstPad * pad, GstObject * parent, GstEvent * event);
static GstFlowReturn gst_g_scream_tx_chain (GstPad * pad, GstObject * parent, GstBuffer * buf);


G_END_DECLS


#endif /* __GST_GSCREAMTX_H__ */

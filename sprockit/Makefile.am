#
#   This file is part of SST/macroscale:
#                The macroscale architecture simulator from the SST suite.
#   Copyright (c) 2009 Sandia Corporation.
#   This software is distributed under the BSD License.
#   Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#   the U.S. Government retains certain rights in this software.
#   For more information, see the LICENSE file in the top
#   SST/macroscale directory.
#

ACLOCAL_AMFLAGS = -I acinclude

EXTRA_DIST = bin/runtest

SUBDIRS = sprockit test
if EXTERNAL_BOOST
AM_LDFLAGS = $(BOOST_LDFLAGS)
AM_LDFLAGS += $(BOOST_REGEX_LIB)
endif



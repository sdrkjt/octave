/*

Copyright (C) 1996, 1997 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "oct-iostrm.h"

// Position a stream at OFFSET relative to ORIGIN.

int
octave_base_iostream::seek (std::streamoff, std::ios::seekdir)
{
  invalid_operation ();
  return -1;
}

// Return current stream position.

long
octave_base_iostream::tell (void) const
{
  invalid_operation ();
  return -1;
}

// Return non-zero if EOF has been reached on this stream.

bool
octave_base_iostream::eof (void) const
{
  invalid_operation ();
  return false;
}

void
octave_base_iostream::invalid_operation (void) const
{
  ::error ("%s: invalid operation", stream_type ());
}

// Return non-zero if EOF has been reached on this stream.

bool
octave_istream::eof (void) const
{
  return is && is->eof ();
}

octave_stream
octave_istream::create (std::istream *arg, const std::string& nm)
{
  return octave_stream (new octave_istream (arg, nm));
}

// Return non-zero if EOF has been reached on this stream.

bool
octave_ostream::eof (void) const
{
  return os && os->eof ();
}

octave_stream
octave_ostream::create (std::ostream *arg, const std::string& nm)
{
  return octave_stream (new octave_ostream (arg, nm));
}

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/

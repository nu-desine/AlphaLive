/*
	oscpack -- Open Sound Control (OSC) packet manipulation library
    http://www.rossbencina.com/code/oscpack

    Copyright (c) 2004-2013 Ross Bencina <rossb@audiomulch.com>

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
	ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	The text above constitutes the entire oscpack license; however, 
	the oscpack developer(s) also make the following non-binding requests:

	Any person wishing to distribute modifications to the Software is
	requested to send the modifications to the original developer so that
	they can be incorporated into the canonical version. It is also 
	requested that these non-binding requests be included whenever the
	above license is reproduced.
*/
#ifndef INCLUDED_OSCPACK_OSCHOSTENDIANNESS_H
#define INCLUDED_OSCPACK_OSCHOSTENDIANNESS_H

/*
    Make sure either OSC_HOST_LITTLE_ENDIAN or OSC_HOST_BIG_ENDIAN is defined

    If you know a way to enhance the detection below for Linux and/or MacOSX
    please let me know! I've tried a few things which don't work.
*/

#if defined(OSC_HOST_LITTLE_ENDIAN) || defined(OSC_HOST_BIG_ENDIAN)

// you can define one of the above symbols from the command line
// then you don't have to edit this file.

#elif defined(__WIN32__) || defined(WIN32) || defined(WINCE)

// assume that __WIN32__ is only defined on little endian systems

#define OSC_HOST_LITTLE_ENDIAN 1
#undef OSC_HOST_BIG_ENDIAN

#elif defined(__APPLE__)

#if defined(__LITTLE_ENDIAN__)
#define OSC_HOST_LITTLE_ENDIAN 1
#undef OSC_HOST_BIG_ENDIAN
#else
#define OSC_HOST_BIG_ENDIAN 1
#undef OSC_HOST_LITTLE_ENDIAN
#endif

#else

#define OS_LINUX ((LINUX)) || (__linux__)
#if OS_LINUX
#else
#warning no host endianness defined, assuming little-endian. please edit OSCHostEndianness.h to configure endianness.
#endif

#define OSC_HOST_LITTLE_ENDIAN 1
#undef OSC_HOST_BIG_ENDIAN


#endif

#endif /* INCLUDED_OSCPACK_OSCHOSTENDIANNESS_H */


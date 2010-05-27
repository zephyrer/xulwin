//
// FileStream.h
//
// $Id: //poco/1.3/Foundation/include/Poco/FileStream.h#2 $
//
// Library: Foundation
// Package: Streams
// Module:  FileStream
//
// Definition of the FileStreamBuf, FileInputStream and FileOutputStream classes.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_FileStream_INCLUDED
#define Foundation_FileStream_INCLUDED


#include "Poco/Foundation.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "FileStream_WIN32.h"
#else
#include "FileStream_POSIX.h"
#endif
#include <istream>
#include <ostream>


namespace Poco {


class Foundation_API FileIOS: public virtual std::ios
	/// The base class for FileInputStream and FileOutputStream.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
	///
	/// Files are always opened in binary mode, a text mode
	/// with CR-LF translation is not supported. Thus, the
	/// file is always opened as if the std::ios::binary flag
	/// was specified.
	/// Use an InputLineEndingConverter or OutputLineEndingConverter
	/// if you require CR-LF translation.
	///
	/// On Windows platforms, if POCO_WIN32_UTF8 is #define'd,
	/// UTF-8 encoded Unicode paths are correctly handled.
{
public:
	FileIOS(std::ios::openmode defaultMode);
		/// Creates the basic stream.
		
	~FileIOS();
		/// Destroys the stream.

	void open(const std::string& path, std::ios::openmode mode);
		/// Opens the file specified by path, using the given mode.
		///
		/// Throws a FileException (or a similar exception) if the file 
		/// does not exist or is not accessible for other reasons and
		/// a new file cannot be created.

	void close();
		/// Closes the file stream.
		///
		/// If, for an output stream, the close operation fails (because
		/// the contents of the stream buffer cannot synced back to
		/// the filesystem), the bad bit is set in the stream state.

	FileStreamBuf* rdbuf();
		/// Returns a pointer to the underlying streambuf.

protected:
	FileStreamBuf _buf;
	std::ios::openmode _defaultMode;
};


class Foundation_API FileInputStream: public FileIOS, public std::istream
	/// An input stream for reading from a file.
	///
	/// Files are always opened in binary mode, a text mode
	/// with CR-LF translation is not supported. Thus, the
	/// file is always opened as if the std::ios::binary flag
	/// was specified.
	/// Use an InputLineEndingConverter if you require CR-LF translation.
	///
	/// On Windows platforms, if POCO_WIN32_UTF8 is #define'd,
	/// UTF-8 encoded Unicode paths are correctly handled.
{
public:
	FileInputStream();
		/// Creates an unopened FileInputStream.
	
	FileInputStream(const std::string& path, std::ios::openmode mode = std::ios::in);
		/// Creates the FileInputStream for the file given by path, using
		/// the given mode.
		///
		/// The std::ios::in flag is always set, regardless of the actual
		/// value specified for mode.
		///
		/// Throws a FileNotFoundException (or a similar exception) if the file 
		/// does not exist or is not accessible for other reasons.

	~FileInputStream();
		/// Destroys the stream.
};


class Foundation_API FileOutputStream: public FileIOS, public std::ostream
	/// An output stream for writing to a file.
	///
	/// Files are always opened in binary mode, a text mode
	/// with CR-LF translation is not supported. Thus, the
	/// file is always opened as if the std::ios::binary flag
	/// was specified.
	/// Use an OutputLineEndingConverter if you require CR-LF translation.
	///
	/// On Windows platforms, if POCO_WIN32_UTF8 is #define'd,
	/// UTF-8 encoded Unicode paths are correctly handled.
{
public:
	FileOutputStream();
		/// Creats an unopened FileOutputStream.
		
	FileOutputStream(const std::string& path, std::ios::openmode mode = std::ios::out | std::ios::trunc);
		/// Creates the FileOutputStream for the file given by path, using
		/// the given mode.
		///
		/// The std::ios::out is always set, regardless of the actual 
		/// value specified for mode.
		///
		/// Throws a FileException (or a similar exception) if the file 
		/// does not exist or is not accessible for other reasons and
		/// a new file cannot be created.

	~FileOutputStream();
		/// Destroys the FileOutputStream.
};


class Foundation_API FileStream: public FileIOS, public std::iostream
	/// A stream for reading from and writing to a file.
	///
	/// Files are always opened in binary mode, a text mode
	/// with CR-LF translation is not supported. Thus, the
	/// file is always opened as if the std::ios::binary flag
	/// was specified.
	/// Use an InputLineEndingConverter or OutputLineEndingConverter
	/// if you require CR-LF translation.
	///
	/// A seek (seekg() or seekp()) operation will always set the 
	/// read position and the write position simultaneously to the
	/// same value.
	///
	/// On Windows platforms, if POCO_WIN32_UTF8 is #define'd,
	/// UTF-8 encoded Unicode paths are correctly handled.
{
public:
	FileStream();
		/// Creats an unopened FileStream.
	
	FileStream(const std::string& path, std::ios::openmode mode = std::ios::out | std::ios::in);
		/// Creates the FileStream for the file given by path, using
		/// the given mode.

	~FileStream();
		/// Destroys the FileOutputStream.
};


} // namespace Poco


#endif // Foundation_FileStream_INCLUDED
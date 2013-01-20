/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __ANYOPTION_H__
#define __ANYOPTION_H__

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#define COMMON_OPT   1
#define COMMAND_OPT  2
#define FILE_OPT  3
#define COMMON_FLAG  4
#define COMMAND_FLAG    5
#define FILE_FLAG    6

#define COMMAND_OPTION_TYPE   1
#define COMMAND_FLAG_TYPE  2
#define FILE_OPTION_TYPE   3
#define FILE_FLAG_TYPE     4
#define UNKNOWN_TYPE       5

#define DEFAULT_MAXOPTS    10
#define MAX_LONG_PREFIX_LENGTH   2

#define DEFAULT_MAXUSAGE   3
#define DEFAULT_MAXHELP         10

#define TRUE_FLAG "true"

using namespace std;

class AnyOption
{

public:

	/* the public interface */

	AnyOption ();
	AnyOption (int maxoptions);
	AnyOption (int maxoptions, int maxcharoptions);
	~AnyOption ();

	/*
	 * following set methods specifies the
	 * special characters and delimiters
	 * if not set traditional defaults will be used
	 */

	void
	setCommandPrefixChar (char _prefix);    /* '-' in "-w" */

	void
	setCommandLongPrefix (char* _prefix);   /* '--' in "--width" */

	void
	setFileCommentChar (char _comment);     /* '#' in shellscripts */

	void
	setFileDelimiterChar (char _delimiter); /* ':' in "width : 100" */

	/*
	 * provide the input for the options
	 * like argv[ ] for commndline and the
	 * option file name  to use;
	 */

	void
	useCommandArgs (int _argc, char** _argv);

	void
	useFiileName (const char* _filename);

	/*
	 * turn off the POSIX style options
	 * this means anything starting with a '-' or "--"
	 * will be considered a valid option
	 * which alo means you cannot add a bunch of
	 * POIX options chars together like "-lr"  for "-l -r"
	 *
	 */

	void
	noPOSIX ();

	/*
	 * prints warning verbose if you set anything wrong
	 */
	void
	setVerbose ();

	/*
	 * there are two types of options
	 *
	 * Option - has an associated value ( -w 100 )
	 * Flag  - no value, just a boolean flag  ( -nogui )
	 *
	 * the options can be either a string ( GNU style )
	 * or a character ( traditional POSIX style )
	 * or both ( --width, -w )
	 *
	 * the options can be common to the commandline and
	 * the optionfile, or can belong only to either of
	 * commandline and optionfile
	 *
	 * following set methods, handle all the aboove
	 * cases of options.
	 */

	/* options comman to command line and option file */
	void
	setOption (const char* opt_string);

	void
	setOption (char opt_char);

	void
	setOption (const char* opt_string, char opt_char);

	void
	setFlag (const char* opt_string);

	void
	setFlag (char opt_char);

	void
	setFlag (const char* opt_string, char opt_char);

	/* options read from commandline only */
	void
	setCommandOption (const char* opt_string);

	void
	setCommandOption (char opt_char);

	void
	setCommandOption (const char* opt_string, char opt_char);

	void
	setCommandFlag (const char* opt_string);

	void
	setCommandFlag (char opt_char);

	void
	setCommandFlag (const char* opt_string, char opt_char);

	/* options read from an option file only  */
	void
	setFileOption (const char* opt_string);

	void
	setFileOption (char opt_char);

	void
	setFileOption (const char* opt_string, char opt_char);

	void
	setFileFlag (const char* opt_string);

	void
	setFileFlag (char opt_char);

	void
	setFileFlag (const char* opt_string, char opt_char);

	/*
	 * process the options, registerd using
	 * useCommandArgs() and useFileName();
	 */
	void
	processOptions ();

	void
	processCommandArgs ();

	void
	processCommandArgs (int max_args);

	bool
	processFile ();

	/*
	 * process the specified options
	 */
	void
	processCommandArgs (int _argc, char** _argv);

	void
	processCommandArgs (int _argc, char** _argv, int max_args);

	bool
	processFile (const char* _filename);

	/*
	 * get the value of the options
	 * will return NULL if no value is set
	 */
	char*
	getValue (const char* _option);

	bool
	getFlag (const char* _option);

	char*
	getValue (char _optchar);

	bool
	getFlag (char _optchar);

	/*
	 * Print Usage
	 */
	void
	printUsage ();

	void
	printAutoUsage ();

	void
	addUsage (const char* line);

	void
	printHelp ();

	/* print auto usage printing for unknown options or flag */
	void
	autoUsagePrint (bool flag);

	/*
	 * get the argument count and arguments sans the options
	 */
	int
	getArgc ();

	char**
	getArgv ();

	char*
	getArgv (int index);

	bool
	hasOptions ();


private:

	/* the hidden data structure */

	int         argc;                                  /* commandline arg count  */
	char**      argv;                                  /* commndline args */
	const char* filename;                              /* the option file */
	char*       appname;                               /* the application name from argv[0] */

	int* new_argv;                                     /* arguments sans options (index to argv) */
	int  new_argc;                                     /* argument count sans the options */
	int  max_legal_args;                               /* ignore extra arguments */

	/* option strings storage + indexing */
	int          max_options;                          /* maximum number of options */
	const char** options;                              /* storage */
	int*         optiontype;                           /* type - common, command, file */
	int*         optionindex;                          /* index into value storage */
	int          option_counter;                       /* counter for added options  */

	/* option chars storage + indexing */
	int   max_char_options;                            /* maximum number options */
	char* optionchars;                                 /*  storage */
	int*  optchartype;                                 /* type - common, command, file */
	int*  optcharindex;                                /* index into value storage */
	int   optchar_counter;                             /* counter for added options  */

	/* values */
	char** values;                                     /* common value storage */
	int    g_value_counter;                            /* globally updated value index LAME! */

	/* help and usage */
	const char** usage;                                /* usage */
	int          max_usage_lines;                      /* max usage lines reseverd */
	int          usage_lines;                          /* number of usage lines */

	bool command_set;                                  /* if argc/argv were provided */
	bool file_set;                                     /* if a filename was provided */
	bool mem_allocated;                                /* if memory allocated in init() */
	bool posix_style;                                  /* enables to turn off POSIX style options */
	bool verbose;                                      /* silent|verbose */
	bool print_usage;                                  /* usage verbose */
	bool print_help;                                   /* help verbose */

	char opt_prefix_char;                              /*  '-' in "-w" */
	char long_opt_prefix [MAX_LONG_PREFIX_LENGTH + 1]; /* '--' in "--width" */
	char file_delimiter_char;                          /* ':' in width : 100 */
	char file_comment_char;                            /*  '#' in "#this is a comment" */
	char equalsign;
	char comment;
	char delimiter;
	char endofline;
	char whitespace;
	char nullterminate;

	bool set;                                          //was static member
	bool once;                                         //was static member

	bool hasoptions;
	bool autousage;


private:

	/* the hidden utils */

	void
	init ();

	void
	init (int maxopt, int maxcharopt);

	bool
	alloc ();

	void
	cleanup ();

	bool
	valueStoreOK ();

	/* grow storage arrays as required */
	bool
	doubleOptStorage ();

	bool
	doubleCharStorage ();

	bool
	doubleUsageStorage ();

	bool
	setValue (const char* option, char* value);

	bool
	setFlagOn (const char* option);

	bool
	setValue (char optchar, char* value);

	bool
	setFlagOn (char optchar);

	void
	addOption (const char* option, int type);

	void
	addOption (char optchar, int type);

	void
	addOptionError (const char* opt);

	void
	addOptionError (char opt);

	bool
	findFlag (char* value);

	void
	addUsageError (const char* line);

	bool
	CommandSet ();

	bool
	FileSet ();

	bool
	POSIX ();

	char
	parsePOSIX (char* arg);

	int
	parseGNU (char* arg);

	bool
	matchChar (char c);

	int
	matchOpt (char* opt);

	/* dot file methods */
	char*
	readFile ();

	char*
	readFile (const char* fname);

	bool
	consumeFile (char* buffer);

	void
	processLine (char* theline, int length);

	char*
	chomp (char* str);

	void
	valuePairs (char* type, char* value);

	void
	justValue (char* value);

	void
	printVerbose (const char* msg);

	void
	printVerbose (char* msg);

	void
	printVerbose (char ch);

	void
	printVerbose ();

};

#endif /* ! _ANYOPTION_H */

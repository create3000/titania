## Process this file with automake to produce Makefile.in


lib_LTLIBRARIES =  \
	libtitania-peaseblossom.la

libtitania_peaseblossom_la_SOURCES =  \
	Titania/PeaseBlossom/Base/GarbargeCollector.cpp \
	Titania/PeaseBlossom/Base/pbChildObject.cpp \
	Titania/PeaseBlossom/Bits/Identifier.cpp \
	Titania/PeaseBlossom/Execution/Block.cpp \
	Titania/PeaseBlossom/Execution/pbBlock.cpp \
	Titania/PeaseBlossom/Execution/pbExecutionContext.cpp \
	Titania/PeaseBlossom/Execution/Program.cpp \
	Titania/PeaseBlossom/Expressions/pbExpression.cpp \
	Titania/PeaseBlossom/InputOutput/Genrator.cpp \
	Titania/PeaseBlossom/Objects/Array.cpp \
	Titania/PeaseBlossom/Objects/Function.cpp \
	Titania/PeaseBlossom/Objects/NativeFunction.cpp \
	Titania/PeaseBlossom/Objects/Object.cpp \
	Titania/PeaseBlossom/Objects/pbFunction.cpp \
	Titania/PeaseBlossom/Objects/pbObject.cpp \
	Titania/PeaseBlossom/Parser/Parser.cpp \
	Titania/PeaseBlossom/Primitives/var.cpp \
	Titania/PeaseBlossom/Standard/Function.cpp \
	Titania/PeaseBlossom/Standard/Global.cpp \
	Titania/PeaseBlossom/Standard/Math.cpp \
	Titania/PeaseBlossom/Standard/Object.cpp 

libtitania_peaseblossom_la_LIBADD = $(TITANIA_STANDARD_LIBS) \
	$(top_builddir)/libtitania-standard/libtitania-standard.la

AM_CPPFLAGS = $(TITANIA_STANDARD_CFLAGS) \
	-I$(top_builddir)/libtitania-standard

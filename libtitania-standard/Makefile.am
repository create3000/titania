## Process this file with automake to produce Makefile.in

current_dir = $(shell pwd)

lib_LTLIBRARIES =  \
	libtitania-standard.la

libtitania_standard_la_SOURCES =  \
	$(current_dir)/Titania/Backtrace.cpp \
	$(current_dir)/Titania/Basic/Path.cpp \
	$(current_dir)/Titania/Basic/URI.cpp \
	$(current_dir)/Titania/Chrono/ClockBase.cpp \
	$(current_dir)/Titania/Chrono/CountingClock.cpp \
	$(current_dir)/Titania/Chrono/Now.cpp \
	$(current_dir)/Titania/Chrono/StopWatch.cpp \
	$(current_dir)/Titania/Chrono/SystemClock.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/AABB.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/Common/MyMatrixTypeDefs.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/ConvexHull2D.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/Diameter/alloc.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/Diameter/EstimateDiameter.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/Diameter/rand.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/Diameter/util.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/GeometryPredicates/Predicates.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/GeometryPredicates/PredicatesInit.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/MinAreaRectangle.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/OOBB.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/pugixml/pugixml.cpp \
	$(current_dir)/Titania/External/ApproxMVBB/RandomGenerators.cpp \
	$(current_dir)/Titania/Geospatial/BasicConverter.cpp \
	$(current_dir)/Titania/Geospatial/Geodetic.cpp \
	$(current_dir)/Titania/Geospatial/UniversalTransverseMercator.cpp \
	$(current_dir)/Titania/InputOutput/Character.cpp \
	$(current_dir)/Titania/InputOutput/InverseCharacter.cpp \
	$(current_dir)/Titania/InputOutput/InverseSequence.cpp \
	$(current_dir)/Titania/InputOutput/InverseString.cpp \
	$(current_dir)/Titania/InputOutput/MultiLineComment.cpp \
	$(current_dir)/Titania/InputOutput/MultiString.cpp \
	$(current_dir)/Titania/InputOutput/QuotedString.cpp \
	$(current_dir)/Titania/InputOutput/Sequence.cpp \
	$(current_dir)/Titania/InputOutput/SingleLineComment.cpp \
	$(current_dir)/Titania/InputOutput/String.cpp \
	$(current_dir)/Titania/Math/Algorithms/CatmullRomSplineInterpolator.cpp \
	$(current_dir)/Titania/Math/Algorithms/L-System.cpp \
	$(current_dir)/Titania/Math/Geometry/Box2.cpp \
	$(current_dir)/Titania/Math/Geometry/Box3.cpp \
	$(current_dir)/Titania/Math/Geometry/ConvexHull2.cpp \
	$(current_dir)/Titania/Math/Geometry/ConvexHull3.cpp \
	$(current_dir)/Titania/Math/Geometry/Cylinder3.cpp \
	$(current_dir)/Titania/Math/Geometry/Line2.cpp \
	$(current_dir)/Titania/Math/Geometry/Line3.cpp \
	$(current_dir)/Titania/Math/Geometry/LineSegment2.cpp \
	$(current_dir)/Titania/Math/Geometry/LineSegment3.cpp \
	$(current_dir)/Titania/Math/Geometry/Plane3.cpp \
	$(current_dir)/Titania/Math/Geometry/Sphere3.cpp \
	$(current_dir)/Titania/Math/Geometry/Spheroid3.cpp \
	$(current_dir)/Titania/Math/Geometry/Triangle2.cpp \
	$(current_dir)/Titania/Math/Geometry/Triangle3.cpp \
	$(current_dir)/Titania/Math/Mesh/Combine.cpp \
	$(current_dir)/Titania/Math/Mesh/NurbsTessellator.cpp \
	$(current_dir)/Titania/Math/Numbers/Color3.cpp \
	$(current_dir)/Titania/Math/Numbers/Color4.cpp \
	$(current_dir)/Titania/Math/Numbers/Matrix2.cpp \
	$(current_dir)/Titania/Math/Numbers/Matrix3.cpp \
	$(current_dir)/Titania/Math/Numbers/Matrix4.cpp \
	$(current_dir)/Titania/Math/Numbers/Quaternion.cpp \
	$(current_dir)/Titania/Math/Numbers/Rotation4.cpp \
	$(current_dir)/Titania/Math/Numbers/Vector2.cpp \
	$(current_dir)/Titania/Math/Numbers/Vector3.cpp \
	$(current_dir)/Titania/Math/Numbers/Vector4.cpp \
	$(current_dir)/Titania/Math/Utility/MatrixStack.cpp \
	$(current_dir)/Titania/OS/find_data_file.cpp \
	$(current_dir)/Titania/RegEx/regex_escape.cpp \
	$(current_dir)/Titania/String/dtoa.cpp \
	$(current_dir)/Titania/String/naturally_compare.cpp \
	$(current_dir)/Titania/String/sprintf.cpp \
	$(current_dir)/Titania/String/trim.cpp

libtitania_standard_la_LIBADD = $(TITANIA_STANDARD_LIBS)

AM_CPPFLAGS = $(TITANIA_STANDARD_CFLAGS)

AM_LDFLAGS = -lCGAL -lgmp -lmpfr

## Process this file with automake to produce Makefile.in


lib_LTLIBRARIES =  \
	libtitania-standard.la

libtitania_standard_la_SOURCES =  \
	Titania/Backtrace.cpp \
	Titania/Basic/Path.cpp \
	Titania/Basic/URI.cpp \
	Titania/Chrono/ClockBase.cpp \
	Titania/Chrono/CountingClock.cpp \
	Titania/Chrono/Now.cpp \
	Titania/Chrono/StopWatch.cpp \
	Titania/Chrono/SystemClock.cpp \
	Titania/External/ApproxMVBB/AABB.cpp \
	Titania/External/ApproxMVBB/Common/MyMatrixTypeDefs.cpp \
	Titania/External/ApproxMVBB/ConvexHull2D.cpp \
	Titania/External/ApproxMVBB/Diameter/alloc.cpp \
	Titania/External/ApproxMVBB/Diameter/EstimateDiameter.cpp \
	Titania/External/ApproxMVBB/Diameter/rand.cpp \
	Titania/External/ApproxMVBB/Diameter/util.cpp \
	Titania/External/ApproxMVBB/GeometryPredicates/Predicates.cpp \
	Titania/External/ApproxMVBB/GeometryPredicates/PredicatesInit.cpp \
	Titania/External/ApproxMVBB/MinAreaRectangle.cpp \
	Titania/External/ApproxMVBB/OOBB.cpp \
	Titania/External/ApproxMVBB/pugixml/pugixml.cpp \
	Titania/External/ApproxMVBB/RandomGenerators.cpp \
	Titania/Geospatial/BasicConverter.cpp \
	Titania/Geospatial/Geodetic.cpp \
	Titania/Geospatial/UniversalTransverseMercator.cpp \
	Titania/InputOutput/Character.cpp \
	Titania/InputOutput/InverseCharacter.cpp \
	Titania/InputOutput/InverseSequence.cpp \
	Titania/InputOutput/InverseString.cpp \
	Titania/InputOutput/MultiLineComment.cpp \
	Titania/InputOutput/MultiString.cpp \
	Titania/InputOutput/QuotedString.cpp \
	Titania/InputOutput/Sequence.cpp \
	Titania/InputOutput/SingleLineComment.cpp \
	Titania/InputOutput/String.cpp \
	Titania/Math/Algorithms/CatmullRomSplineInterpolator.cpp \
	Titania/Math/Algorithms/L-System.cpp \
	Titania/Math/Geometry/Box2.cpp \
	Titania/Math/Geometry/Box3.cpp \
	Titania/Math/Geometry/ConvexHull2.cpp \
	Titania/Math/Geometry/ConvexHull3.cpp \
	Titania/Math/Geometry/Cylinder3.cpp \
	Titania/Math/Geometry/Line2.cpp \
	Titania/Math/Geometry/Line3.cpp \
	Titania/Math/Geometry/LineSegment2.cpp \
	Titania/Math/Geometry/LineSegment3.cpp \
	Titania/Math/Geometry/Plane3.cpp \
	Titania/Math/Geometry/Sphere3.cpp \
	Titania/Math/Geometry/Spheroid3.cpp \
	Titania/Math/Geometry/Triangle2.cpp \
	Titania/Math/Geometry/Triangle3.cpp \
	Titania/Math/Mesh/Combine.cpp \
	Titania/Math/Mesh/NurbsTessellator.cpp \
	Titania/Math/Numbers/Color3.cpp \
	Titania/Math/Numbers/Color4.cpp \
	Titania/Math/Numbers/Matrix2.cpp \
	Titania/Math/Numbers/Matrix3.cpp \
	Titania/Math/Numbers/Matrix4.cpp \
	Titania/Math/Numbers/Quaternion.cpp \
	Titania/Math/Numbers/Rotation4.cpp \
	Titania/Math/Numbers/Vector2.cpp \
	Titania/Math/Numbers/Vector3.cpp \
	Titania/Math/Numbers/Vector4.cpp \
	Titania/Math/Utility/MatrixStack.cpp \
	Titania/OS/find_data_file.cpp \
	Titania/RegEx/regex_escape.cpp \
	Titania/String/dtoa.cpp \
	Titania/String/naturally_compare.cpp \
	Titania/String/sprintf.cpp \
	Titania/String/trim.cpp

libtitania_standard_la_LIBADD = $(TITANIA_STANDARD_LIBS) 

AM_CPPFLAGS = $(TITANIA_STANDARD_CFLAGS) 

AM_LDFLAGS = -lCGAL -lgmp -lmpfr

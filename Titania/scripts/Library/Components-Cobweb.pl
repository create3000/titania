#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use Glib;
use File::Basename qw (dirname basename);

say "Generate Cobweb Components";

my $script      = dirname $0;
my $solution    = "$script/../../../";
my $TitaniaInfo = "$solution/titania-info/titania-info";
my $components  = "$solution/Titania/share/titania/Library/Components";

my $nodes = new Glib::KeyFile ();
$nodes -> load_from_data (join ("", `$TitaniaInfo -i=nodes 2>/dev/null`), "none");

my %cobweb = map { $_ => 1 } qw (
          Anchor
          Appearance
          Arc2D
          ArcClose2D
          AudioClip
          Background
          //BallJoint
          Billboard
          BooleanFilter
          BooleanSequencer
          BooleanToggle
          BooleanTrigger
          //BoundedPhysicsModel
          Box
          CADAssembly
          CADFace
          CADLayer
          CADPart
          Circle2D
          ClipPlane
          //CollidableOffset
          //CollidableShape
          Collision
          //CollisionCollection
          //CollisionSensor
          //CollisionSpace
          Color
          //ColorChaser
          ColorDamper
          ColorInterpolator
          ColorRGBA
          ComposedCubeMapTexture
          ComposedShader
          //ComposedTexture3D
          Cone
          //ConeEmitter
          //Contact
          //Contour2D
          //ContourPolyline2D
          Coordinate
          //CoordinateChaser
          CoordinateDamper
          CoordinateDouble
          CoordinateInterpolator
          CoordinateInterpolator2D
          Cylinder
          CylinderSensor
          //DISEntityManager
          //DISEntityTypeMapping
          DirectionalLight
          Disk2D
          //DoubleAxisHingeJoint
          EaseInEaseOut
          ElevationGrid
          //EspduTransform
          //ExplosionEmitter
          Extrusion
          //FillProperties
          //FloatVertexAttribute
          Fog
          //FogCoordinate
          FontStyle
          //ForcePhysicsModel
          //GeneratedCubeMapTexture
          //GeoCoordinate
          //GeoElevationGrid
          //GeoLOD
          //GeoLocation
          //GeoMetadata
          //GeoOrigin
          //GeoPositionInterpolator
          //GeoProximitySensor
          //GeoTouchSensor
          //GeoTransform
          //GeoViewpoint
          Group
          //HAnimDisplacer
          //HAnimHumanoid
          //HAnimJoint
          //HAnimSegment
          //HAnimSite
          //ImageCubeMapTexture
          ImageTexture
          //ImageTexture3D
          IndexedFaceSet
          IndexedLineSet
          IndexedQuadSet
          IndexedTriangleFanSet
          IndexedTriangleSet
          IndexedTriangleStripSet
          Inline
          IntegerSequencer
          IntegerTrigger
          KeySensor
          LOD
          Layer
          LayerSet
          Layout
          LayoutGroup
          LayoutLayer
          //LinePickSensor
          LineProperties
          LineSet
          LoadSensor
          LocalFog
          Material
          //Matrix3VertexAttribute
          //Matrix4VertexAttribute
          MetadataBoolean
          MetadataDouble
          MetadataFloat
          MetadataInteger
          MetadataSet
          MetadataString
          //MotorJoint
          MovieTexture
          //MultiTexture
          //MultiTextureCoordinate
          //MultiTextureTransform
          NavigationInfo
          Normal
          NormalInterpolator
          //NurbsCurve
          //NurbsCurve2D
          //NurbsOrientationInterpolator
          //NurbsPatchSurface
          //NurbsPositionInterpolator
          //NurbsSet
          //NurbsSurfaceInterpolator
          //NurbsSweptSurface
          //NurbsSwungSurface
          //NurbsTextureCoordinate
          //NurbsTrimmedSurface
          //OrientationChaser
          OrientationDamper
          OrientationInterpolator
          OrthoViewpoint
          //PackagedShader
          //ParticleSystem
          //PickableGroup
          PixelTexture
          //PixelTexture3D
          PlaneSensor
          //PointEmitter
          PointLight
          //PointPickSensor
          PointSet
          Polyline2D
          //PolylineEmitter
          Polypoint2D
          //PositionChaser
          //PositionChaser2D
          PositionDamper
          PositionDamper2D
          PositionInterpolator
          PositionInterpolator2D
          //PrimitivePickSensor
          //ProgramShader
          ProximitySensor
          QuadSet
          //ReceiverPdu
          Rectangle2D
          //RigidBody
          //RigidBodyCollection
          //ScalarChaser
          ScalarDamper
          ScalarInterpolator
          //ScreenFontStyle
          ScreenGroup
          Script
          ShaderPart
          //ShaderProgram
          Shape
          //SignalPdu
          //SingleAxisHingeJoint
          //SliderJoint
          Sound
          Sphere
          SphereSensor
          SplinePositionInterpolator
          SplinePositionInterpolator2D
          SplineScalarInterpolator
          SpotLight
          SquadOrientationInterpolator
          StaticGroup
          //StringSensor
          //SurfaceEmitter
          Switch
          //TexCoordChaser2D
          TexCoordDamper2D
          Text
          TextureBackground
          TextureCoordinate
          TextureCoordinate3D
          TextureCoordinate4D
          //TextureCoordinateGenerator
          TextureProperties
          TextureTransform
          TextureTransform3D
          TextureTransformMatrix3D
          TimeSensor
          TimeTrigger
          //TouchGroup
          TouchSensor
          Transform
          //TransformSensor
          //TransmitterPdu
          TriangleFanSet
          TriangleSet
          TriangleSet2D
          TriangleStripSet
          TwoSidedMaterial
          //UniversalJoint
          Viewpoint
          ViewpointGroup
          Viewport
          VisibilitySensor
          //VolumeEmitter
          //VolumePickSensor
          //WindPhysicsModel
          WorldInfo
);

foreach my $node ($nodes -> get_groups ())
{
	say $node;

	next unless exists $cobweb {$node};

	my $componentName = $nodes -> get_string ($node, "componentName");

	system "mkdir", "-p", "$components/$componentName";
	system "touch", "$components/$componentName/.$node-Cobweb";
}

#pragma once
#include "app_base.h"
#include "utility_functions.h"
#include "camera.h"
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include <math.h>

// Types for Mesh
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef CGAL::Point_with_normal_3<Kernel> Point_with_normal;
typedef Kernel::Sphere_3 Sphere;
typedef std::vector<Point_with_normal> PointList;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Poisson_reconstruction_function<Kernel> Poisson_reconstruction_function;
typedef CGAL::Surface_mesh_default_triangulation_3 STr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;
typedef CGAL::Implicit_surface_3<Kernel, Poisson_reconstruction_function> Surface_3;

struct UserData : public AppBase {
	UserData() {}
	//////전역변수 대신 이곳에 변수를 선언한다.
	// Application states
	int width, height;
	float aspectRatio;
	glm::mat4 VP;
	glm::mat4 T;
	bool isColor = false;
	bool showFace = false;
	std::vector<glm::vec3> face_inlier;
	std::vector<glm::vec3> face_inlier_filtered;


	// OpenGL Objects
	Program program_pointcloud;
	VertexArray VAO_pointcloud;

	Program program_color;
	VertexArray VAO_color;

	Program program_face_boundary;
	VertexArray VAO_face_boundary;

	Program program_face_pointcloud;
	VertexArray VAO_face_pointcloud;

	// RealSense
	rs2::pipeline pipe;
	rs2_intrinsics realsense_intrinsics;
	rs2::pointcloud pc;
	rs2::frameset frames;
	rs2::frame depth;
	rs2::frame color;
	rs2::points points;
	texture_info realsense_tex;

	// Trackball
	camera::Trackball trackball;
	camera::Trackball::Behavior behavior;

	// Pointcloud Data
	std::vector<glm::vec3> depth_points;
	std::vector<glm::vec2> depth_texcoords;
	std::vector<ubyte3> depth_colors;

	// OpenCV
	std::string cascadeName;
	std::string nestedCascadeName;
	cv::VideoCapture capture;
	cv::Mat frame, image;
	std::string inputName;
	bool tryflip;
	cv::CascadeClassifier cascade, nestedCascade;
	double scale;
	bool isDetect = false;
	cv::Rect face;

	//////보조로 사용될 함수도 이곳에 선언한다.
	void Init_OpenGL(int width, int height);
	void Init_RealSense();
	void Init_Trackball(int width, int height);
	void Init_OpenCV();

	void Update_RealSense();
	void Update_OpenCV();

	void Cleanup_OpenGL();
	void Cleanup_RealSense();

	//OpenCV Haar-cascade face detection
	void detectFace(cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale, bool tryflip);
	void filterFace();
};


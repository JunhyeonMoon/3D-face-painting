#pragma once
#include "app_base.h"
#include "utility_functions.h"
#include "camera.h"
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct UserData : public AppBase {
	UserData() {}
	//////전역변수 대신 이곳에 변수를 선언한다.
	// Application states
	int width, height;
	float aspectRatio;
	glm::mat4 VP;
	glm::mat4 T;
	bool isDetect = false;
	bool isTrack = false;
	bool isColor = false;
	bool isColor_FeaturePoints = false;;
	bool isColor_Mesh = false;
	bool isDepth = false;
	bool isDepth_FeaturePoints = false;;
	bool isDepth_Mesh = false;
	bool isDepth_Paint = false;
	bool isDepth_PointCloud = false;
	std::vector<std::string> Console_msg;
	
	std::vector<glm::vec3> face_inlier;
	std::vector<glm::vec3> face_inlier_filtered;
	std::vector<glm::vec2> face_features;
	std::vector<glm::vec2> face_features_fixed;
	std::vector<glm::vec2> face_features_mesh2D;
	std::vector<glm::vec2> face_features_mesh2D_update;
	std::vector<glm::vec3> face_features_mesh3D;
	std::vector<glm::vec3> face_plane;
	std::vector<glm::vec2> face_tex_coordinate;
	
	std::vector<glm::vec2> face_tex_coord_buf; // capture해서 저장된 feature point --> mesh 생성
	glm::vec3 init_pos; // 처음 모든 점의 평균위치
	glm::vec3 init_nose; // 처음 코
	glm::vec3 init_vert;
	glm::vec3 init_hori;
	glm::vec3 init_norm; // capture해서 저장할 normal vector
	glm::mat4 Rv;
	glm::mat4 Rh;
	glm::mat4 Rn;
	glm::mat4 TN; // 움직여진 코
	glm::mat4 TM; // tracking matrix
	
	texture_info tex1;
	texture_info tex2;
	texture_info tex3;


	// OpenGL Objects
	Program program_pointcloud;
	VertexArray VAO_pointcloud;

	Program program_color;
	VertexArray VAO_color;

	Program program_face_boundary;
	VertexArray VAO_face_boundary;

	Program program_face_pointcloud;
	VertexArray VAO_face_pointcloud;

	Program program_face_mesh2D;
	VertexArray VAO_face_mesh2D;

	Program program_face_mesh3D;
	VertexArray VAO_face_mesh3D;

	Program program_face_mesh3D_paint;
	VertexArray VAO_face_mesh3D_paint;


	// RealSense
	rs2::pipeline pipe;
	rs2_intrinsics realsense_intrinsics_color;
	rs2_intrinsics realsense_intrinsics_depth;
	rs2::pipeline_profile selection;
	rs2_extrinsics extrinsics_depth_to_color;
	rs2_extrinsics extrinsics_color_to_depth;

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

	//ImGui
	bool show_another_window = true;
	ImVec2 MainMenu_size;
	ImVec2 SideBar_size;
	ImVec2 Console_size;
	
	//dlib
	dlib::frontal_face_detector detector;
	dlib::shape_predictor sp;

	//////보조로 사용될 함수도 이곳에 선언한다.
	void Init_OpenGL(int width, int height);
	void Init_ImGui();
	void Init_RealSense();
	void Init_Trackball(int width, int height);
	void Init_dlib();

	void Update_RealSense();
	void Update_ImGui();
	void Update_ImGui_MainMenu();
	void Update_ImGui_SideBar();
	void Update_ImGui_Console();


	void Update_dlib();
	void Update_Mesh();

	void Track_face();
	void Capture_Point();

	void Cleanup_OpenGL();
	void Cleanup_RealSense();
	void Cleanup_ImGui();

	void filterFace();

	// Draw a single point
	void draw_point(cv::Mat& img, cv::Point2f fp, cv::Scalar color);
	// Draw delaunay triangles
	void draw_delaunay(cv::Mat& img, cv::Subdiv2D& subdiv, cv::Scalar delaunay_color);
	void transform_2Dto3D(std::vector<glm::vec2>&, std::vector<glm::vec3>&);

};


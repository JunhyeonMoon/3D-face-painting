#include "pch.h"
#include "global_table.h"
#include "shader_sources.h"


// 보조로 사용될 함수의 정의를 이곳에 작성한다.

void UserData::Init_OpenGL(int width, int height) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	aspectRatio = (float)width / (float)height;
	//////pointcloud
	std::map<GLenum, const char*> pointcloud_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("pointcloud_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("pointcloud_fs") }
	};
	program_pointcloud = CreateProgram(pointcloud_shader_map);

	std::vector<VBO_info> VBO_info_pointcloud = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
		{ VBO_TEX, 0, nullptr, 0, sizeof(glm::vec2) }
	};
	std::vector<attrib_info> attrib_info_pointcloud = {
		{ 0, GL_FLOAT, 3, 0 },
		{ 1, GL_FLOAT, 2, 0 }
	};
	VAO_pointcloud = CreateVertexArray(VBO_info_pointcloud, attrib_info_pointcloud);


	////color
	glm::vec3 colorv[6] = {
	{-1.f, 1.f, 0.f}, {-1.f, -1.f, 0.f}, {1.f, -1.f, 0.f},
	{-1.f, 1.f, 0.f}, {1.f, -1.f, 0.f}, {1.f, 1.f, 0.f}
	};
	glm::vec2 colortc[6] = {
	{0.f, 0.f}, {0.f, 1.f}, {1.f, 1.f},
	{0.f, 0.f}, {1.f, 1.f}, {1.f, 0.f}
	};

	std::map<GLenum, const char*> color_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("color_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("color_fs") }
	};
	program_color = CreateProgram(color_shader_map);

	std::vector<VBO_info> VBO_info_color = {
		{ VBO_POS, 6, colorv, 0, sizeof(glm::vec3) },
		{ VBO_TEX, 6, colortc, 0, sizeof(glm::vec2) }
	};
	std::vector<attrib_info> attrib_info_color = {
		{ 0, GL_FLOAT, 3, 0 },
		{ 1, GL_FLOAT, 2, 0 }
	};
	VAO_color = CreateVertexArray(VBO_info_color, attrib_info_color);

	//2D face boundary
	std::map<GLenum, const char*> face_boundary_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_boundary_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_boundary_fs") }
	};
	program_face_boundary = CreateProgram(face_boundary_shader_map);

	std::vector<VBO_info> VBO_info_face_boundary = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) }
	};
	std::vector<attrib_info> attrib_info_face_boundary = {
		{ 0, GL_FLOAT, 3, 0 }
	};
	VAO_face_boundary = CreateVertexArray(VBO_info_face_boundary, attrib_info_face_boundary);

	//3D face point cloud
	std::map<GLenum, const char*> face_pointcloud_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_pointcloud_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_pointcloud_fs") }
	};
	program_face_pointcloud = CreateProgram(face_pointcloud_shader_map);

	std::vector<VBO_info> VBO_info_face_pointcloud = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) }
	};
	std::vector<attrib_info> attrib_info_face_pointcloud = {
		{ 0, GL_FLOAT, 3, 0 }
	};
	VAO_face_pointcloud = CreateVertexArray(VBO_info_face_pointcloud, attrib_info_face_pointcloud);


	//////OpenGL state
	glPointSize(3.0f);
	glEnable(GL_CULL_FACE); // 삼각형이 뒷면이 보이는 경우 그리지 않는다.
	glEnable(GL_DEPTH_TEST); // 뒷면에 가려지는 도형은 그리지 않는다.

	// 나중에 pixel unpack 연산 (pixel 데이터를 GPU로 보내는 작업)을 할 때 주석을 풀자.

	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
}

void UserData::Init_RealSense() {
	pipe.start();
	realsense_intrinsics = pipe.get_active_profile().get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>().get_intrinsics();
}

void UserData::Init_Trackball(int width, int height) {
	trackball.curr.eye = glm::vec3{};
	trackball.curr.at = glm::vec3{ 0, 0, 1 };
	trackball.curr.up = glm::vec3{ 0, -1, 0 };
	trackball.curr.width = float(width) / 1000;
	trackball.curr.height = float(height) / 1000;
	trackball.curr.fovy = glm::radians<float>(46.f);
	trackball.curr.dnear = 0.001f;
	trackball.prev = trackball.home = trackball.curr;
	trackball.curr.dfar = 20.0f;
}

void UserData::Init_OpenCV() {
	cascadeName = "haarcascades\\haarcascade_frontalface_alt.xml";
	nestedCascadeName = "haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
	scale = 1;
	tryflip = false;
	inputName = "";

	if(!nestedCascade.load(nestedCascadeName))
		std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
	
	if (!cascade.load(cascadeName))
	{
		std::cerr << "ERROR: Could not load classifier cascade" << std::endl;
	}
}

void UserData::detectFace(cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale, bool tryflip)
{
	//double t = 0;
	std::vector<cv::Rect> faces, faces2;
	//const static cv::Scalar colors[] =
	//{
	//	cv::Scalar(255,0,0),
	//	cv::Scalar(255,128,0),
	//	cv::Scalar(255,255,0),
	//	cv::Scalar(0,255,0),
	//	cv::Scalar(0,128,255),
	//	cv::Scalar(0,255,255),
	//	cv::Scalar(0,0,255),
	//	cv::Scalar(255,0,255)
	//};
	cv::Mat gray, smallImg;

	cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	double fx = 1 / scale;
	cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR_EXACT);
	cv::equalizeHist(smallImg, smallImg);

	//t = (double)cv::getTickCount();

	//detect face
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		//|cv::CASCADE_FIND_BIGGEST_OBJECT
		//|cv::CASCADE_DO_ROUGH_SEARCH
		| cv::CASCADE_SCALE_IMAGE,
		cv::Size(30, 30));

	if (faces.size() > 0) {
		isDetect = true;
		face = faces[0];
	}

	//if (tryflip)
	//{
	//	flip(smallImg, smallImg, 1);
	//	cascade.detectMultiScale(smallImg, faces2,
	//		1.1, 2, 0
	//		//|CASCADE_FIND_BIGGEST_OBJECT
	//		//|CASCADE_DO_ROUGH_SEARCH
	//		| cv::CASCADE_SCALE_IMAGE,
	//		cv::Size(30, 30));
	//	for (std::vector<cv::Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r)
	//	{
	//		faces.push_back(cv::Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
	//	}
	//}
	//t = (double)cv::getTickCount() - t;
	
	//for (size_t i = 0; i < faces.size(); i++)
	//{
	//	cv::Rect r = faces[i]; //왼쪽 상단의 x, y, width, height값을 가짐
	//	cv::Mat smallImgROI;
	//	std::vector<cv::Rect> nestedObjects;
	//	cv::Point center;
	//	//cv::Scalar color = colors[i % 8];
	//	int radius;
	//
	//	double aspect_ratio = (double)r.width / r.height;
	//	if (0.75 < aspect_ratio && aspect_ratio < 1.3)
	//	{
	//		center.x = cvRound((r.x + r.width*0.5)*scale);
	//		center.y = cvRound((r.y + r.height*0.5)*scale);
	//		radius = cvRound((r.width + r.height)*0.25*scale);
	//		circle(img, center, radius, color, 3, 8, 0);
	//	}
	//	else
	//		rectangle(img, cv::Point(cvRound(r.x*scale), cvRound(r.y*scale)),
	//			cv::Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
	//			color, 3, 8, 0);
	//
	//
	//	//눈을 찾고 그리는 코드
	//	if (nestedCascade.empty())
	//		continue;
	//	smallImgROI = smallImg(r);
	//	nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
	//		1.1, 2, 0
	//		//|cv::CASCADE_FIND_BIGGEST_OBJECT
	//		//|cv::CASCADE_DO_ROUGH_SEARCH
	//		//|cv::CASCADE_DO_CANNY_PRUNING
	//		| cv::CASCADE_SCALE_IMAGE,
	//		cv::Size(30, 30));
	//	for (size_t j = 0; j < nestedObjects.size(); j++)
	//	{
	//		cv::Rect nr = nestedObjects[j];
	//		center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
	//		center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
	//		radius = cvRound((nr.width + nr.height)*0.25*scale);
	//		circle(img, center, radius, color, 3, 8, 0);
	//	}
	//}
	//imshow("result", img);
}

#include <stdlib.h>

void UserData::Update_RealSense() {
	frames = pipe.wait_for_frames();

	depth = frames.get_depth_frame();
	color = frames.get_color_frame();

	points = pc.calculate(depth);
	pc.map_to(color);


	//get depth points and texture coordinates
	depth_points.clear();
	depth_texcoords.clear();

	const rs2::vertex *vertices = points.get_vertices();
	const rs2::texture_coordinate *texcoord = points.get_texture_coordinates();

	glm::vec3 tv;
	glm::vec2 tc;
	for (int i = 0; i < int(points.size()); i++) {
		if (vertices[i].z == 0) continue;
		tv.x = vertices[i].x;
		tv.y = vertices[i].y;
		tv.z = vertices[i].z;
		depth_points.emplace_back(tv);

		tc.x = texcoord[i].u;
		tc.y = texcoord[i].v;
		depth_texcoords.push_back(tc);
	} 

	VertexBufferData(VAO_pointcloud, VBO_POS, depth_points.size(), sizeof(glm::vec3), depth_points.data(), GL_STREAM_DRAW);
	VertexBufferData(VAO_pointcloud, VBO_TEX, depth_texcoords.size(), sizeof(glm::vec2), depth_texcoords.data(), GL_STREAM_DRAW);

	//update realsense color texture
	if (realsense_tex.tex == 0) {
		glGenTextures(1, &realsense_tex.tex);
	}
	glBindTexture(GL_TEXTURE_2D, realsense_tex.tex);

	realsense_tex.width = static_cast<rs2::video_frame>(color).get_width();
	realsense_tex.height = static_cast<rs2::video_frame>(color).get_height();
	//static_cast<rs2::video_frame>(color).get_data();
	rs2_format format = static_cast<rs2::video_frame>(color).get_profile().format();
	
	switch (format)
	{
	case RS2_FORMAT_RGB8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, realsense_tex.width, realsense_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, static_cast<rs2::video_frame>(color).get_data());
		break;
	case RS2_FORMAT_RGBA8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realsense_tex.width, realsense_tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<rs2::video_frame>(color).get_data());
		break;
	default:
		printf("unsupported format: %d", format);
		throw std::runtime_error("The requested format is not suported by this demo!");
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void UserData::Update_OpenCV() {
	//Realsense의 color_frame의 data를 cv::Mat형식으로 변환
	//https://github.com/IntelRealSense/librealsense/blob/master/doc/stepbystep/getting_started_with_openCV.md
	cv::Mat frame1(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	isDetect = false;
	detectFace(frame1, cascade, nestedCascade, scale, tryflip);
}


void UserData::Cleanup_OpenGL() {
	DestroyProgram(program_pointcloud);
	DestroyVertexArray(VAO_pointcloud);

	DestroyProgram(program_color);
	DestroyVertexArray(VAO_color);

	DestroyProgram(program_face_boundary);
	DestroyVertexArray(VAO_face_boundary);

	DestroyProgram(program_face_pointcloud);
	DestroyVertexArray(VAO_face_pointcloud);
}


void UserData::Cleanup_RealSense() {
	pipe.stop();
}

void UserData::filterFace() {
	face_inlier_filtered.clear();
	float sumOfz = 0;
	for (int i = 0; i < face_inlier.size(); i++) {
		sumOfz += face_inlier[i].z;
	}

	float avgz = sumOfz / (float)face_inlier.size();
	
	//표준편차를 이용해서 얼굴이 아닌 points걸러냄
	float variance = 0; //분산
	for (int i = 0; i < face_inlier.size(); i++) {
		variance += pow(face_inlier[i].z - avgz, 2);
	}
	variance /= (float)face_inlier.size();
	float std_deviation = sqrt(variance); //표준편차
	
	float c = 1.f;
	float referenceDepth = avgz + c * std_deviation;
	for (int i = 0; i < face_inlier.size(); i++) {
		if (face_inlier[i].z < referenceDepth) {
			face_inlier_filtered.emplace_back(face_inlier[i]);
		}
	}
}

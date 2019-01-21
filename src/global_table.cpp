#include "pch.h"
#include "global_table.h"
#include "shader_sources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 보조로 사용될 함수의 정의를 이곳에 작성한다.

void UserData::Init_OpenGL(int width, int height) {
	tx.data = stbi_load("image.png", &tx.width, &tx.height, &tx.n, 4);
	tx.tex = CreateTexture2D(tx.width, tx.height, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, tx.data);
	stbi_image_free(tx.data);

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
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
	};
	std::vector<attrib_info> attrib_info_face_pointcloud = {
		{ 0, GL_FLOAT, 3, 0 },
	};
	VAO_face_pointcloud = CreateVertexArray(VBO_info_face_pointcloud, attrib_info_face_pointcloud);

	//2D face mesh
	std::map<GLenum, const char*> face_mesh2D_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_mesh2D_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_mesh2D_fs") }
	};
	program_face_mesh2D = CreateProgram(face_mesh2D_shader_map);

	std::vector<VBO_info> VBO_info_face_mesh2D = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec2) },
	};
	std::vector<attrib_info> attrib_info_face_mesh2D = {
		{ 0, GL_FLOAT, 2, 0 },
	};
	VAO_face_mesh2D = CreateVertexArray(VBO_info_face_mesh2D, attrib_info_face_mesh2D);

	//3D face mesh
	std::map<GLenum, const char*> face_mesh3D_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_mesh3D_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_mesh3D_fs") }
	};
	program_face_mesh3D = CreateProgram(face_mesh3D_shader_map);

	std::vector<VBO_info> VBO_info_face_mesh3D = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
	};
	std::vector<attrib_info> attrib_info_face_mesh3D = {
		{ 0, GL_FLOAT, 3, 0 },
	};
	VAO_face_mesh3D = CreateVertexArray(VBO_info_face_mesh3D, attrib_info_face_mesh3D);

	// 3D face mesh painting
	std::map<GLenum, const char*> face_mesh3D_paint_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_mesh3D_paint_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_mesh3D_paint_fs") }
	};
	program_face_mesh3D_paint = CreateProgram(face_mesh3D_paint_shader_map);

	std::vector<VBO_info> VBO_info_face_mesh3D_paint = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
		{ VBO_TEX, 0, nullptr, 0, sizeof(glm::vec2) }
	};
	std::vector<attrib_info> attrib_info_face_mesh3D_paint = {
		{ 0, GL_FLOAT, 3, 0 },
		{ 1, GL_FLOAT, 2, 0 }
	};
	VAO_face_mesh3D_paint = CreateVertexArray(VBO_info_face_mesh3D_paint, attrib_info_face_mesh3D_paint);

	//////OpenGL state
	glPointSize(3.0f);
	glEnable(GL_CULL_FACE); // 삼각형이 뒷면이 보이는 경우 그리지 않는다.
	glEnable(GL_DEPTH_TEST); // 뒷면에 가려지는 도형은 그리지 않는다.

	// 나중에 pixel unpack 연산 (pixel 데이터를 GPU로 보내는 작업)을 할 때 주석을 풀자.

	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
}

void UserData::Init_ImGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
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

void UserData::Init_dlib() {
	detector = dlib::get_frontal_face_detector();
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
}

void UserData::Update_dlib() {
	dlib::array2d<dlib::rgb_pixel> img;
	
	cv::Mat frame1(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(frame1));
	// Make the image larger so we can detect small faces.
	//pyramid_up(img);

	// Now tell the face detector to give us a list of bounding boxes
	// around all the faces in the image.
	std::vector<dlib::rectangle> dets = detector(img);
	//std::cout << "Number of faces detected: " << dets.size() << std::endl;
	
	if (dets.size() > 0) {
		isDetect = true;
		dlib::full_object_detection shape;
		shape = sp(img, dets[0]);
		face_features.clear();
		
		for (int i = 0; i < shape.num_parts(); i++) {
			glm::vec2 point = { shape.part(i).x(), shape.part(i).y() };
			face_features.push_back(point);
		}

		//위의 texture좌표를 OpenGL좌표로 변환
		std::vector<glm::vec3> face_features_gl;
		face_features_gl.clear();
		
		int w = realsense_tex.width;
		int h = realsense_tex.height;
		for (int i = 0; i < face_features.size(); i++) {
			glm::vec3 temp = { 2 * (face_features[i].x / (float)w - 0.5), -2 * (face_features[i].y / (float)h - 0.5) , 0.f };
			face_features_gl.push_back(temp);
		}
		VertexBufferData(VAO_face_boundary, VBO_POS, face_features_gl.size(), sizeof(glm::vec3), face_features_gl.data(), GL_STREAM_DRAW);


		face_inlier.clear();
		int dw = static_cast<rs2::depth_frame>(depth).get_width();
		int dh = static_cast<rs2::depth_frame>(depth).get_height();
		int cw = static_cast<rs2::video_frame>(color).get_width();
		int ch = static_cast<rs2::video_frame>(color).get_height();

		for (int i = 0; i < face_features.size(); i++) {
			float pixel_distance_in_meters = static_cast<rs2::depth_frame>(depth).get_distance(face_features[i].x * dw / cw, face_features[i].y * dh / ch);
			float tp[3] = { 0.f, 0.f, 0.f };
			const float pix[2] = { face_features[i].x, face_features[i].y };
			rs2_deproject_pixel_to_point(tp, &realsense_intrinsics, pix, pixel_distance_in_meters);
			glm::vec3 temp = { tp[0], tp[1], tp[2] };
			face_inlier.emplace_back(temp);
		}
		// 표준편차 
		float avg_of_z = 0.0;
		for (int i = 0; i < face_inlier.size(); i++) {
			avg_of_z += face_inlier[i].z;
		}
		avg_of_z /= (float)face_inlier.size();
		float var_of_z = 0.0;
		for (int i = 0; i < face_inlier.size(); i++) {
			var_of_z += pow((avg_of_z - face_inlier[i].z), 2);
		}
		var_of_z /= (float)face_inlier.size();
		float dev_of_z = sqrt(var_of_z);

		float c = 1.f;
		float refer_Depth = avg_of_z + c * dev_of_z;
		for (int i = 0; i < face_inlier.size(); i++) {
			if (face_inlier[i].z > refer_Depth) {
				if (i >= 0 && i < 6) {
					for (int j = 0; j < 6; j++) {
						if (face_inlier[j].z <= refer_Depth) {
							face_inlier[i].z = face_inlier[j].z;
							break;
						}
					}
				}
				else if (i >= 6 && i < 11) {
					for (int j = 6; j < 11; j++) {
						if (face_inlier[j].z <= refer_Depth) {
							face_inlier[i].z = face_inlier[j].z;
							break;
						}
					}
				}
				else if (i >= 11 && i < 17) {
					for (int j = 11; j < 17; j++) {
						if (face_inlier[j].z <= refer_Depth) {
							face_inlier[i].z = face_inlier[j].z;
							break;
						}
					}
				}
				else if (i >= 17 && i < 22) {
					for (int j = 17; j < 22; j++) {
						if (face_inlier[j].z <= refer_Depth) {
							face_inlier[i].z = face_inlier[j].z;
							break;
						}
					}
				}
				else if (i >= 22 && i < 27) {
					for (int j = 22; j < 27; j++) {
						if (face_inlier[j].z <= refer_Depth) {
							face_inlier[i].z = face_inlier[j].z;
							break;
						}
					}
				}
			}
		}

		face_features_depth.clear();
		for (int i = 0; i < face_inlier.size(); i++) {
			face_features_depth.push_back(face_inlier[i].z);
		}

		VertexBufferData(VAO_face_pointcloud, VBO_POS, face_inlier.size(), sizeof(glm::vec3), face_inlier.data(), GL_STREAM_DRAW);
	}
	else {
		isDetect = false;
	}

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

void UserData::Update_ImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

void UserData::Update_Mesh() {

	// Read in the image.
	cv::Mat img(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	
	// Keep a copy around
	cv::Mat img_orig = img.clone();

	// Rectangle to be used with Subdiv2D
	cv::Size size = img.size();
	cv::Rect rect(0, 0, size.width, size.height);

	// Create an instance of Subdiv2D
	cv::Subdiv2D subdiv(rect);

	// Create a vector of points.
	std::vector<cv::Point2f> points;

	// Read in the points from a text file
	for (int i = 0; i < face_features.size(); i++)
		points.push_back(cv::Point2f(face_features[i].x, face_features[i].y));

	// Insert points into subdiv
	for (std::vector<cv::Point2f>::iterator it = points.begin(); it != points.end(); it++)
	{
		subdiv.insert(*it);
	}
	// Draw delaunay triangles
	//draw_delaunay(img, subdiv, delaunay_color);
	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);
	face_features_mesh2D.clear();

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[2] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[1] = cv::Point(cvRound(t[4]), cvRound(t[5]));

		glm::vec2 temp[3];
		for (int j = 0; j < 3; j += 1) {
			temp[j].x = pt[j].x;
			temp[j].y = pt[j].y;
			face_features_mesh2D.push_back(temp[j]);
		}

		// Draw rectangles completely inside the image.
		//if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
		//{
		//	line(img, pt[0], pt[1], delaunay_color, 1, cv::LINE_AA, 0);
		//	line(img, pt[1], pt[2], delaunay_color, 1, cv::LINE_AA, 0);
		//	line(img, pt[2], pt[0], delaunay_color, 1, cv::LINE_AA, 0);
		//}
	}
	

	

	face_features_mesh3D.clear();
	int dw = static_cast<rs2::depth_frame>(depth).get_width();
	int dh = static_cast<rs2::depth_frame>(depth).get_height();
	int cw = static_cast<rs2::video_frame>(color).get_width();
	int ch = static_cast<rs2::video_frame>(color).get_height();
	
	// deprojection 
	for (int i = 0; i < face_features_mesh2D.size(); i++) {

		// float pixel_distance_in_meters = static_cast<rs2::depth_frame>(depth).get_distance(face_features_mesh2D[i].x * dw / cw, face_features_mesh2D[i].y * dh / ch);
		float pixel_distance_in_meters;
		for (int j = 0; j < face_features.size(); j++) {
			if (face_features[j] == face_features_mesh2D[i]) {
				pixel_distance_in_meters = face_features_depth[j];
				break;
			}
		}
		float tp[3] = { 0.f, 0.f, 0.f };
		const float pix[2] = { face_features_mesh2D[i].x, face_features_mesh2D[i].y };
		rs2_deproject_pixel_to_point(tp, &realsense_intrinsics, pix, pixel_distance_in_meters);
		glm::vec3 temp = { tp[0], tp[1], tp[2] };
		face_features_mesh3D.emplace_back(temp);
		

	}
	VertexBufferData(VAO_face_mesh3D, VBO_POS, face_features_mesh3D.size(), sizeof(glm::vec3), face_features_mesh3D.data(), GL_STREAM_DRAW);


	for (int i = 0; i < face_features_mesh2D.size(); i += 1) {
		face_features_mesh2D[i].x = (face_features_mesh2D[i].x / (float)size.width - 0.5) * 2.f;
		face_features_mesh2D[i].y = (face_features_mesh2D[i].y / (float)size.height - 0.5) * (-2.f);
	}

	VertexBufferData(VAO_face_mesh2D, VBO_POS, face_features_mesh2D.size(), sizeof(glm::vec2), face_features_mesh2D.data(), GL_STREAM_DRAW);
	
	// 3D mesh painting
	std::vector<glm::vec3> ulgul;
	ulgul.clear();
	for (int i = 0; i < face_features_mesh3D.size() / 3; i++) {
		ulgul.push_back(face_features_mesh3D[i]);
		
	}
	VertexBufferData(VAO_face_mesh3D_paint, VBO_POS, ulgul.size(), sizeof(glm::vec3), ulgul.data(), GL_STREAM_DRAW);

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

	DestroyProgram(program_face_mesh2D);
	DestroyVertexArray(VAO_face_mesh2D);

}


void UserData::Cleanup_RealSense() {
	pipe.stop();
}

void UserData::Cleanup_ImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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
			face_inlier[i].z -= 0.02f;
			face_inlier_filtered.emplace_back(face_inlier[i]);
		}
	}
}

// Draw delaunay triangles
void UserData::draw_delaunay(cv::Mat& img, cv::Subdiv2D& subdiv, cv::Scalar delaunay_color)
{

	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);
	cv::Size size = img.size();
	cv::Rect rect(0, 0, size.width, size.height);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));

		// Draw rectangles completely inside the image.
		if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
		{
			line(img, pt[0], pt[1], delaunay_color, 1, cv::LINE_AA, 0);
			line(img, pt[1], pt[2], delaunay_color, 1, cv::LINE_AA, 0);
			line(img, pt[2], pt[0], delaunay_color, 1, cv::LINE_AA, 0);
		}
	}
}
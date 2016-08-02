#pragma once

/*
Reconstructed particle with position and time data
*/
struct Particle
{
	double x;
	double y;
	double t;
	double x_uv;
	double y_uv;
	bool xy_uv;
	double x_uw;
	double y_uw;
	bool xy_uw;
	double x_vw;
	double y_vw;
	bool xy_vw;
	double x_uv_uw;
	double y_uv_uw;
	double x_uv_vw;
	double y_uv_vw;
	double x_uw_vw;
	double y_uw_vw;
}; 

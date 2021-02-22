/**
 * @file "vector.cpp"
 *
 * @author Alexandre Chetafi
**/

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vector.hpp"

using namespace ez;

TEST(Math, VectorInitialisation)
{
	Vector3<double>	v(1, 2, 3);

	EXPECT_DOUBLE_EQ(v.x(), 1);
	EXPECT_DOUBLE_EQ(v.y(), 2);
	EXPECT_DOUBLE_EQ(v.z(), 3);

	Vector2<double>	vv(1, 2);
	EXPECT_DOUBLE_EQ(v.x(), 1);
	EXPECT_DOUBLE_EQ(v.y(), 2);
}

TEST(Math, VectorRotate2D)
{
	Vector2<double>	v(1, 2);

	rotate(v, 90);
	EXPECT_DOUBLE_EQ(v.x(), 2.f);
	EXPECT_DOUBLE_EQ(v.y(), -1.f);

	Vector2<double>	vv(1, 2);
	rotate(vv, 45);
	EXPECT_DOUBLE_EQ(vv.x(), cos(45) - 2 * sin(45));
	EXPECT_DOUBLE_EQ(vv.y(), sin(45) + 2 * cos(45));
}

TEST(Math, VectorRotate3Dx)
{
	Vector3<double>	v(1, 2, 3);

	rotatex(v, 45);
	EXPECT_DOUBLE_EQ(v.x(), 1);
	EXPECT_DOUBLE_EQ(v.y(), 2 * cos(45) + 3 * sin(45));
	EXPECT_DOUBLE_EQ(v.z(), - 2 * sin(45) + 3 * cos(45));
}

TEST(Math, VectorRotate3Dy)
{
	Vector3<double>	v(1, 2, 3);

	rotatey(v, 45);
	EXPECT_DOUBLE_EQ(v.x(), cos(45) - 3 * sin(45));
	EXPECT_DOUBLE_EQ(v.y(), 2);
	EXPECT_DOUBLE_EQ(v.z(), sin(45) + 3 * cos(45));
}

TEST(Math, VectorRotate3Dz)
{
	Vector3<double>	v(1, 2, 3);

	rotatez(v, 45);
	EXPECT_DOUBLE_EQ(v.x(), cos(45) + 2 * sin(45));
	EXPECT_DOUBLE_EQ(v.y(), - sin(45) + 2 * cos(45));
	EXPECT_DOUBLE_EQ(v.z(), 3);
}

TEST(Math, VectorNormalize)
{
	ez::Vector3<float> vecA(0.56f, 1.45f, 54.3f);
	ez::Vector3<float> vecB(1.34f, 2.0f, 31.56f);

	glm::vec3 vecC(0.56f, 1.45f, 54.3f);
	glm::vec3 vecD(1.34f, 2.0f, 31.56f);

	ez::Matrix<float, 3, 1> resultA = ez::normalize(vecA);
	ez::Matrix<float, 3, 1> resultB = ez::normalize(vecB);
	glm::vec3 resultC = glm::normalize(vecC);
	glm::vec3 resultD = glm::normalize(vecD);

	for (int i = 0; i < 3; ++i)
	{
		EXPECT_FLOAT_EQ(resultA.get(i, 0), resultC[i]);
		EXPECT_FLOAT_EQ(resultB.get(i, 0), resultD[i]);
	}
}

TEST(Math, VectorDotProduct)
{
	ez::Vector3<float> vecA(0.56f, 1.45f, 54.3f);
	ez::Vector3<float> vecB(1.34f, 2.0f, 31.56f);

	glm::vec3 vecC(0.56f, 1.45f, 54.3f);
	glm::vec3 vecD(1.34f, 2.0f, 31.56f);

	EXPECT_EQ(ez::dot(vecA, vecB), glm::dot(vecC, vecD));
	EXPECT_EQ(ez::dot(vecB, vecA), glm::dot(vecD, vecC));

	vecA = ez::Vector3<float>(0.0f, 0.0f, 1.0f);
	vecB = ez::Vector3<float>(0.0f, 1.0f, 0.0f);

	EXPECT_EQ(ez::dot(vecA, vecB), 0.0f);

	vecA = ez::Vector3<float>(1.0f, 0.1f, 0.01f);
	vecB = ez::Vector3<float>(3.0f, 4.0f, 5.0f);

	EXPECT_EQ(ez::dot(vecA, vecB), 3.45f);
}

TEST(Math, VectorCrossProduct)
{
	ez::Vector3<float> vecA(0.56f, 1.45f, 54.3f);
	ez::Vector3<float> vecB(1.34f, 2.0f, 31.56f);

	glm::vec3 vecC(0.56f, 1.45f, 54.3f);
	glm::vec3 vecD(1.34f, 2.0f, 31.56f);

	ez::Matrix<float, 3, 1> resultA = ez::cross(vecA, vecB);
	ez::Matrix<float, 3, 1> resultB = ez::cross(vecB, vecA);
	glm::vec3 resultC = glm::cross(vecC, vecD);
	glm::vec3 resultD = glm::cross(vecD, vecC);

	for (int i = 0; i < 3; ++i)
	{
		EXPECT_EQ(resultA.get(i, 0), resultC[i]);
		EXPECT_EQ(resultB.get(i, 0), resultD[i]);
	}
}
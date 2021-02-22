/**
 * @file "__tests__/Core/Math/matrix.cpp"
 * A file to test matrix
 *
 * @Author Alexandre Chetafi
 */

#define TEST_MATH

#define GLM_FORCE_DEPTH_NEGATIVE_ONE_TO_ONE
#define GLM_FORCE_LEFT_HANDED

#include <gtest/gtest.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Matrix.hpp"

using namespace ez;

TEST(Math, MatrixExcpectCrash)
{
	auto w = []() {
		Matrix<int, 0, 1>	l;
	};
	auto x = []() {
		Matrix<int, 1, 0>	l;
	};
	auto y = []() {
		Matrix<int, 1, 1>	l;
		l/0;
	};
	EXPECT_ANY_THROW(w());
	EXPECT_ANY_THROW(x());
	EXPECT_ANY_THROW(y());
}

TEST(Math, MatrixEmptyEquality)
{
	Matrix<int, 2, 2>	a;
	Matrix<int, 2, 2>	b;


	EXPECT_TRUE(a == b);
	EXPECT_TRUE(b == a);
}

TEST(Math, MatrixEmptyEqualityFalse)
{
	Matrix<int, 2, 2>	a;
	Matrix<int, 2, 2>	b;
	a(0, 1) = 2;

	EXPECT_FALSE(a == b);
	EXPECT_FALSE(b == a);
}

TEST(Math, MatrixTestingEmptyOperation)
{
	Matrix<int, 2, 2>	a;
	Matrix<int, 2, 2>	b;

	EXPECT_NO_THROW(a+=b);
	EXPECT_NO_THROW(a-=b);
	EXPECT_NO_THROW(a*=b);
	EXPECT_NO_THROW(a/=2);
	EXPECT_NO_THROW(a*=2);
	EXPECT_NO_THROW(a+b);
	EXPECT_NO_THROW(a-b);
	EXPECT_NO_THROW(a*3);
	EXPECT_NO_THROW(a*b);
	//EXPECT_NO_THROW(a/3);
}

TEST(Math, MatrixTestingAddition)
{
	Matrix<int, 3, 3>	a;
	Matrix<int, 3, 3>	b;
	Matrix<int, 3, 3>	c;

	int res_data[3][3]= {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	int a_data[3][3] = {{955, -434, 2}, {0, 50, 234}, {-324, -349, 34}};
	int b_data[3][3] = {{-955, 435, 0}, {3, -46, -229}, {330, 356, -26}};
	std::memcpy(a.getTdata(), a_data, sizeof(int) * 9);
	std::memcpy(b.getTdata(), b_data, sizeof(int) * 9);
	std::memcpy(c.getTdata(), res_data, sizeof(int) * 9);

	auto x = a + b;
	EXPECT_TRUE(x == c);

	Matrix<int, 3, 3> xx;
	xx += a + b;
	EXPECT_TRUE(xx == c);

}

TEST(Math, MatrixTestingDifference)
{
	Matrix<int, 3, 3>	a;
	Matrix<int, 3, 3>	b;
	Matrix<int, 3, 3>	c;

	int res_data[3][3]= {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	int a_data[3][3] = {{955, -434, 2}, {0, 50, 234}, {-324, -349, 34}};
	int b_data[3][3] = {{955, -435, 0}, {-3, 46, 229}, {-330, -356, 26}};
	std::memcpy(a.getTdata(), a_data, sizeof(int) * 9);
	std::memcpy(b.getTdata(), b_data, sizeof(int) * 9);
	std::memcpy(c.getTdata(), res_data, sizeof(int) * 9);

	auto x = a - b;
	auto xy = b - a;
	EXPECT_TRUE(x == c);
	EXPECT_FALSE(xy == c);

	Matrix<int, 3, 3> xx;
	xx = a;
	xx -= b;
	EXPECT_TRUE(xx == c);

}

TEST(Math, MatrixTestingMultiplication)
{
	Matrix<int, 3, 3>	a;
	Matrix<int, 3, 3>	b;
	Matrix<int, 3, 3>	c;
	Matrix<int, 3, 3>	d;

	int res_data[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	int res_u_data[3][3] = {{0, 2, 4}, {6, 8, 10}, {12, 14, 16}};
	int a_data[3][3] = {{955, -434, 2}, {0, 50, 234}, {-324, -349, 34}};
	int b_data[3][3] = {{-955, 435, 0}, {3, -46, -229}, {330, 356, -26}};
	std::memcpy(a.getTdata(), a_data, sizeof(int) * 9);
	std::memcpy(b.getTdata(), b_data, sizeof(int) * 9);
	std::memcpy(c.getTdata(), res_data, sizeof(int) * 9);
	std::memcpy(d.getTdata(), res_u_data, sizeof(int) * 9);

	auto x = c * 2;
	EXPECT_TRUE(x == d);
	auto xx = c;
	xx *= 2;
	EXPECT_TRUE(xx == d);

	auto I = makeIdentity<int, 3>();
	auto y = c * I;
	EXPECT_TRUE(y == c);

	Matrix<int, 3, 3>	e;
	int res_multp_data[3][3] = {{30, 36, 42}, {84, 108, 132}, {138, 180, 222}};
	std::memcpy(e.getTdata(), res_multp_data, sizeof(int) * 9);
	y = c * d;
	EXPECT_TRUE(y == e);

	Matrix<int, 3, 3> z;
	z = a;
	z *= I;
	EXPECT_TRUE(z == a);

	z = c;
	z *= d;
	EXPECT_TRUE(z == e);
}

TEST(Math, MatrixTestingDivision)
{
	Matrix<int, 3, 3>	a;
	Matrix<int, 3, 3>	b;
	Matrix<int, 3, 3>	c;
	Matrix<int, 3, 3>	d;

	int res_u_data[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	int res_data[3][3] = {{0, 2, 4}, {6, 8, 10}, {12, 14, 16}};
	int a_data[3][3] = {{955, -434, 2}, {0, 50, 234}, {-324, -349, 34}};
	int b_data[3][3] = {{-955, 435, 0}, {3, -46, -229}, {330, 356, -26}};
	std::memcpy(a.getTdata(), a_data, sizeof(int) * 9);
	std::memcpy(b.getTdata(), b_data, sizeof(int) * 9);
	std::memcpy(c.getTdata(), res_data, sizeof(int) * 9);
	std::memcpy(d.getTdata(), res_u_data, sizeof(int) * 9);

	auto x = c / 2;
	EXPECT_TRUE(x == d);
	auto xx = c;
	xx /= 2;
	EXPECT_TRUE(xx == d);
}

TEST(Math, MatrixTestingDeterminant)
{
	Matrix<int, 3, 3>	a;
	int res_a_data[3][3] = {{7, 1, 3}, {2, 4, 1}, {1, 5, 1}};
	Matrix<int, 2, 2>	b;
	int res_b_data[2][2] = {{0, 1}, {3, 4}};
	Matrix<int, 4, 4>	c;
	int res_c_data[4][4] = {{1, 0, 4, -6}, {2, 5, 0, 3}, {-1, 2, 3, 5}, {2, 1, -2, 3}};

	std::memcpy(a.getTdata(), res_a_data, sizeof(int) * 9);
	EXPECT_EQ(getDeterminant(a), 10);

	std::memcpy(b.getTdata(), res_b_data, sizeof(int) * 4);
	EXPECT_EQ(getDeterminant(b), -3);

	std::memcpy(c.getTdata(), res_c_data, sizeof(int) * 16);
	EXPECT_EQ(getDeterminant(c), 318);
}

TEST(Math, CofactorTesting)
{
	Matrix<int, 2, 2>	a;
	Matrix<int, 2, 2>	test_a;

	int	to_test_a[2][2] = {{3, 4}, {6, 2}};
	int correct_a[2][2] = {{2, -6}, {-4, 3}};
	std::memcpy(a.getTdata(), to_test_a, sizeof(int) * 2 * 2);
	std::memcpy(test_a.getTdata(), correct_a, sizeof(int) * 2 * 2);
	auto co_a = getCofactor(a);

	EXPECT_TRUE(co_a == test_a);

	Matrix<int, 3, 3>	b;
	Matrix<int, 3, 3>	test_b;

	int to_test_b[3][3] = {{9, 4, 32}, {3, 5, 2}, {67, 1, 0}};
	int correct_b[3][3] = {{-2, 134, -332}, {32, -2144, 259}, {-152, 78, 33}};
	std::memcpy(b.getTdata(), to_test_b, sizeof(int) * 3 * 3);
	std::memcpy(test_b.getTdata(), correct_b, sizeof(int) * 3 * 3);
	auto co_b = getCofactor(b);

	EXPECT_TRUE(co_b == test_b);
}

TEST(Math, InverseMatrix)
{
	Matrix<int, 2, 2>	a;
	Matrix<double, 2, 2>	test_a;

	int to_test_a[2][2] = {{2, 4}, {3, 5}};
	double correct_a[2][2] = {{-2.5, 2}, {1.5, -1}};
	std::memcpy(a.getTdata(), to_test_a, sizeof(int) * 4);
	std::memcpy(test_a.getTdata(), correct_a, sizeof(double) * 4);
	auto inv_a = getInverse(a);

	EXPECT_TRUE(inv_a == test_a);

	Matrix<int, 3, 3>	b;
	Matrix<double, 3, 3>	test_b;
	int to_test_b[3][3] = {{2, 4, 0}, {3, 5, 4}, {1, 2, 1}};
	double correct_b[3][3] = {{1.5, 2, -8}, {- .5, -1, 4}, {- .5, 0, 1}};
	std::memcpy(b.getTdata(), to_test_b, sizeof(int) * 9);
	std::memcpy(test_b.getTdata(), correct_b, sizeof(double) * 9);
	auto inv_b = getInverse(b);

	// double precision make it not work, waiting for specialisation

/*
	for (auto i = 0; i < 3; i++) {
		for (auto j = 0; j < 3; j++) {
			std::cout << inv_b.get(i, j) << " ";
		}
		std::cout << std::endl;
	}

	EXPECT_TRUE(inv_b == test_b);
 */
}

TEST(Math, DegreeToRadianConversion)
{
	EXPECT_NEAR(ez::radians(47.0f),				glm::radians(47.0f),			0.0000005f);
	EXPECT_NEAR(ez::radians(0.0f),				glm::radians(0.0f),				0.0000005f);
	EXPECT_NEAR(ez::radians(395.6f),			glm::radians(395.6f),			0.0000005f);
	EXPECT_NEAR(ez::radians(-137.123456789f),	glm::radians(-137.123456789f),	0.0000005f);
}

TEST(Math, MatrixCreation)
{
	ez::Matrix<float, 4, 4> matA;
	glm::mat4 matB(0);

	ez::Matrix<float, 4, 4> matC = ez::makeIdentity<float, 4>();
	glm::mat4 matD(1);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			EXPECT_EQ(matA.get(i, j), 0);
			EXPECT_EQ(matB[i][j], 0);

			if (i == j)
			{
				EXPECT_EQ(matC.get(i, j), 1);
				EXPECT_EQ(matD[i][j], 1);
			}
			else
			{
				EXPECT_EQ(matC.get(i, j), 0);
				EXPECT_EQ(matD[i][j], 0);
			}
		}
}

TEST(Math, MatrixRotation)
{
	ez::Matrix<float, 3, 1> axisZ;
	axisZ(2, 0) = 1.0f;

	// ez::Matrix<float, 4, 4> matA;
	// glm::mat4 matB(0);

	ez::Matrix<float, 4, 4> matA = ez::makeIdentity<float, 4>();
	glm::mat4 matB(1);
	glm::mat4 matC(1);

	float time = 0.57f;

	matA = ez::rotate(matA, time * ez::radians(90.0f), axisZ);
	matB = glm::rotate(matB, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	matC = glm::rotate_slow(matC, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// for (int i = 0; i < 4; ++i)
	// 	for (int j = 0; j < 4; ++j)
	// 		EXPECT_NEAR(matA.get(i, j), matB[i][j], 0.0000005f);
	
	EXPECT_NEAR(matA.get(0, 0), matB[0][0], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 0), matB[1][0], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 0), matB[2][0], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 0), matB[3][0], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 1), matB[0][1], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 1), matB[1][1], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 1), matB[2][1], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 1), matB[3][1], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 2), matB[0][2], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 2), matB[1][2], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 2), matB[2][2], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 2), matB[3][2], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 3), matB[0][3], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 3), matB[1][3], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 3), matB[2][3], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 3), matB[3][3], 0.0000005f);

	EXPECT_DOUBLE_EQ(matB[0][0], matC[0][0]);
	EXPECT_DOUBLE_EQ(matB[1][0], matC[1][0]);
	EXPECT_DOUBLE_EQ(matB[2][0], matC[2][0]);
	EXPECT_DOUBLE_EQ(matB[3][0], matC[3][0]);
	EXPECT_DOUBLE_EQ(matB[0][1], matC[0][1]);
	EXPECT_DOUBLE_EQ(matB[1][1], matC[1][1]);
	EXPECT_DOUBLE_EQ(matB[2][1], matC[2][1]);
	EXPECT_DOUBLE_EQ(matB[3][1], matC[3][1]);
	EXPECT_DOUBLE_EQ(matB[0][2], matC[0][2]);
	EXPECT_DOUBLE_EQ(matB[1][2], matC[1][2]);
	EXPECT_DOUBLE_EQ(matB[2][2], matC[2][2]);
	EXPECT_DOUBLE_EQ(matB[3][2], matC[3][2]);
	EXPECT_DOUBLE_EQ(matB[0][3], matC[0][3]);
	EXPECT_DOUBLE_EQ(matB[1][3], matC[1][3]);
	EXPECT_DOUBLE_EQ(matB[2][3], matC[2][3]);
	EXPECT_DOUBLE_EQ(matB[3][3], matC[3][3]);
}

TEST(Math, MatrixLookAt)
{
	ez::Matrix<float, 3, 1> axisY;
	axisY(1, 0) = 1.0f;

	ez::Matrix<float, 3, 1> vecA;
	vecA(0, 0) = 1.0f;
	vecA(1, 0) = 1.8f;
	vecA(2, 0) = 2.0f;
	ez::Matrix<float, 3, 1> vecB;

	glm::vec3 vecC(1.0f, 1.8f, 2.0f);
	glm::vec3 vecD(0);

	ez::Matrix<float, 4, 4> matA = ez::lookAt(vecA, vecB, axisY);
	glm::mat4 matB = glm::lookAtLH(vecC, vecD, glm::vec3(0.0f, 1.0f, 0.0f));

	// for (int i = 0; i < 4; ++i)
	// 	for (int j = 0; j < 4; ++j)
	// 		EXPECT_NEAR(matA.get(i, j), matB[i][j], 0.0000005f);

	EXPECT_NEAR(matA.get(0, 0), matB[0][0], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 0), matB[1][0], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 0), matB[2][0], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 0), matB[3][0], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 1), matB[0][1], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 1), matB[1][1], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 1), matB[2][1], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 1), matB[3][1], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 2), matB[0][2], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 2), matB[1][2], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 2), matB[2][2], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 2), matB[3][2], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 3), matB[0][3], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 3), matB[1][3], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 3), matB[2][3], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 3), matB[3][3], 0.0000005f);
}

TEST(Math, MatrixPerspective)
{
	float fA = 90.0f;
	float fB = 35.0f;
	float fC = 1.0f;
	float fD = 10.0f;

	ez::Matrix<float, 4, 4> matA = ez::perspective(fA, fB, fC, fD);
	glm::mat4 matB = glm::perspective(fA, fB, fC, fD);

	// for (int i = 0; i < 4; ++i)
	// 	for (int j = 0; j < 4; ++j)
	// 		EXPECT_NEAR(matA.get(i, j), matB[i][j], 0.0000005f);

	EXPECT_NEAR(matA.get(0, 0), matB[0][0], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 0), matB[1][0], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 0), matB[2][0], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 0), matB[3][0], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 1), matB[0][1], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 1), matB[1][1], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 1), matB[2][1], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 1), matB[3][1], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 2), matB[0][2], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 2), matB[1][2], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 2), matB[2][2], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 2), matB[3][2], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 3), matB[0][3], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 3), matB[1][3], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 3), matB[2][3], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 3), matB[3][3], 0.0000005f);
}

TEST(Math, UpdateUniformBuffer)
{
	float width = 800;
	float height = 600;
	float time = 0.57f;

	ez::Matrix<float, 3, 1> axisZ;
	ez::Matrix<float, 3, 1> vecA;
	ez::Matrix<float, 3, 1> vecB;

	axisZ(2, 0) = 1.0f;
	vecA(0, 0) = 2.0f;
	vecA(1, 0) = 2.0f;
	vecA(2, 0) = 2.0f;

	ez::Matrix<float, 4, 4> matA = ez::rotate(ez::makeIdentity<float, 4>(), time * ez::radians(90.0f), axisZ);
    ez::Matrix<float, 4, 4> matB = ez::lookAt(vecA, vecB, axisZ);
    ez::Matrix<float, 4, 4> matC = ez::perspective(ez::radians(45.0f), width / height, 0.1f, 10.0f);
    matC(1, 1) *= -1;

    glm::mat4 matD = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 matE = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 matF = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 10.0f);
    matF[1][1] *= -1;

    EXPECT_NEAR(matA.get(0, 0), matD[0][0], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 0), matD[1][0], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 0), matD[2][0], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 0), matD[3][0], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 1), matD[0][1], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 1), matD[1][1], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 1), matD[2][1], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 1), matD[3][1], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 2), matD[0][2], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 2), matD[1][2], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 2), matD[2][2], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 2), matD[3][2], 0.0000005f);
	EXPECT_NEAR(matA.get(0, 3), matD[0][3], 0.0000005f);
	EXPECT_NEAR(matA.get(1, 3), matD[1][3], 0.0000005f);
	EXPECT_NEAR(matA.get(2, 3), matD[2][3], 0.0000005f);
	EXPECT_NEAR(matA.get(3, 3), matD[3][3], 0.0000005f);

	EXPECT_NEAR(matB.get(0, 0), matE[0][0], 0.0000005f);
	EXPECT_NEAR(matB.get(1, 0), matE[1][0], 0.0000005f);
	EXPECT_NEAR(matB.get(2, 0), matE[2][0], 0.0000005f);
	EXPECT_NEAR(matB.get(3, 0), matE[3][0], 0.0000005f);
	EXPECT_NEAR(matB.get(0, 1), matE[0][1], 0.0000005f);
	EXPECT_NEAR(matB.get(1, 1), matE[1][1], 0.0000005f);
	EXPECT_NEAR(matB.get(2, 1), matE[2][1], 0.0000005f);
	EXPECT_NEAR(matB.get(3, 1), matE[3][1], 0.0000005f);
	EXPECT_NEAR(matB.get(0, 2), matE[0][2], 0.0000005f);
	EXPECT_NEAR(matB.get(1, 2), matE[1][2], 0.0000005f);
	EXPECT_NEAR(matB.get(2, 2), matE[2][2], 0.0000005f);
	EXPECT_NEAR(matB.get(3, 2), matE[3][2], 0.0000005f);
	EXPECT_NEAR(matB.get(0, 3), matE[0][3], 0.0000005f);
	EXPECT_NEAR(matB.get(1, 3), matE[1][3], 0.0000005f);
	EXPECT_NEAR(matB.get(2, 3), matE[2][3], 0.0000005f);
	EXPECT_NEAR(matB.get(3, 3), matE[3][3], 0.0000005f);

	EXPECT_NEAR(matC.get(0, 0), matF[0][0], 0.0000005f);
	EXPECT_NEAR(matC.get(1, 0), matF[1][0], 0.0000005f);
	EXPECT_NEAR(matC.get(2, 0), matF[2][0], 0.0000005f);
	EXPECT_NEAR(matC.get(3, 0), matF[3][0], 0.0000005f);
	EXPECT_NEAR(matC.get(0, 1), matF[0][1], 0.0000005f);
	EXPECT_NEAR(matC.get(1, 1), matF[1][1], 0.0000005f);
	EXPECT_NEAR(matC.get(2, 1), matF[2][1], 0.0000005f);
	EXPECT_NEAR(matC.get(3, 1), matF[3][1], 0.0000005f);
	EXPECT_NEAR(matC.get(0, 2), matF[0][2], 0.0000005f);
	EXPECT_NEAR(matC.get(1, 2), matF[1][2], 0.0000005f);
	EXPECT_NEAR(matC.get(2, 2), matF[2][2], 0.0000005f);
	EXPECT_NEAR(matC.get(3, 2), matF[3][2], 0.0000005f);
	EXPECT_NEAR(matC.get(0, 3), matF[0][3], 0.0000005f);
	EXPECT_NEAR(matC.get(1, 3), matF[1][3], 0.0000005f);
	EXPECT_NEAR(matC.get(2, 3), matF[2][3], 0.0000005f);
	EXPECT_NEAR(matC.get(3, 3), matF[3][3], 0.0000005f);
}
#include "MyRigidBody.h"
using namespace BTX;
//Allocation
uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	//TODO: Calculate the SAT algorithm I STRONGLY suggest you use the
	//Real Time Collision detection algorithm for OBB here but feel free to
	//implement your own solution.

	// Get the radii of the two objects
	float radiiA = this->GetRadius();
	float radiiB = a_pOther->GetRadius();

	// Generate Rotation Matrices
	glm::mat3x3 rotationA;
	glm::mat3x3 rotationB;

	// Get a vector of vector3s with the format of (axis, min, max) for this object
	std::vector<vector3> au;
	au.push_back(vector3(this->GetModelMatrix()[0][0], this->GetModelMatrix()[0][1], this->GetModelMatrix()[0][2]));
	au.push_back(vector3(this->GetModelMatrix()[1][0], this->GetModelMatrix()[1][1], this->GetModelMatrix()[1][2]));
	au.push_back(vector3(this->GetModelMatrix()[2][0], this->GetModelMatrix()[2][1], this->GetModelMatrix()[2][2]));

	// Get a vector of vector3s with the format of (axis, min, max) for the other object
	std::vector<vector3> bu;
	bu.push_back(vector3(a_pOther->GetModelMatrix()[0][0], a_pOther->GetModelMatrix()[0][1], a_pOther->GetModelMatrix()[0][2]));
	bu.push_back(vector3(a_pOther->GetModelMatrix()[1][0], a_pOther->GetModelMatrix()[1][1], a_pOther->GetModelMatrix()[1][2]));
	bu.push_back(vector3(a_pOther->GetModelMatrix()[2][0], a_pOther->GetModelMatrix()[2][1], a_pOther->GetModelMatrix()[2][2]));

	// Get the centers of both objects
	vector3 aCenter;
	vector3 bCenter;

	aCenter = this->GetCenterGlobal();
	bCenter = a_pOther->GetCenterGlobal();

	// Get the Half Widths of both object
	vector3 aHalf;
	vector3 bHalf;

	aHalf = this->GetHalfWidth();
	bHalf = a_pOther->GetHalfWidth();

	// compute rotation matrix of the other object into this object's coodinate frame
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			rotationA[i][j] = glm::dot(au[i], bu[j]);
		}
	}

	// Compute translation vector
	vector3 translation = a_pOther->GetCenterGlobal() - this->GetCenterGlobal();

	// Bring translation vector int this objects coordinate frame.
	translation = vector3(glm::dot(translation, au[0]), glm::dot(translation, au[1]), glm::dot(translation, au[2]));

	// Compute common subexpressions.
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			rotationB[i][j] = glm::abs(rotationA[i][j]) + FLT_EPSILON;
		}
	}

	// Test axes Ax, Ay, Az
	for (size_t i = 0; i < 3; i++)
	{
		radiiA = aHalf[i];
		radiiB = bHalf[0] * rotationB[i][0] + bHalf[1] * rotationB[i][1] + bHalf[2] * rotationB[i][2];
		if (glm::abs(translation[i]) > radiiA + radiiB)
		{
			return BTXs::eSATResults::SAT_NONE;
		}
	}

	// Test axes Bx,By, Bz
	for (size_t i = 0; i < 3; i++)
	{
		radiiA = aHalf[0] * rotationB[0][i] + aHalf[1] * rotationB[1][i] + aHalf[2] * rotationB[2][i];
		radiiB = bHalf[i];
		if (glm::abs(translation[0] * rotationA[0][i] + translation[1] * rotationA[1][i] + translation[2] * rotationA[2][i]) > radiiA + radiiB)
		{
			return BTXs::eSATResults::SAT_NONE;
		}
	}

	// Test axis Ax x Bx
	radiiA = aHalf[1] * rotationB[2][0] + aHalf[2] * rotationB[1][0];
	radiiB = bHalf[1] * rotationB[0][2] + bHalf[2] * rotationB[0][1];
	if (glm::abs(translation[2] * rotationA[1][0] - translation[1] * rotationA[2][0]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Ax x By
	radiiA = aHalf[1] * rotationB[2][1] + aHalf[2] * rotationB[1][1];
	radiiB = bHalf[0] * rotationB[0][2] + bHalf[2] * rotationB[0][0];
	if (glm::abs(translation[2] * rotationA[1][1] - translation[1] * rotationA[2][1]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Ax x Bz
	radiiA = aHalf[1] * rotationB[2][2] + aHalf[2] * rotationB[1][2];
	radiiB = bHalf[0] * rotationB[0][1] + bHalf[1] * rotationB[0][0];
	if (glm::abs(translation[2] * rotationA[1][2] - translation[1] * rotationA[2][2]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Ay x Bx
	radiiA = aHalf[0] * rotationB[2][0] + aHalf[2] * rotationB[0][0];
	radiiB = bHalf[1] * rotationB[1][2] + bHalf[2] * rotationB[1][1];
	if (glm::abs(translation[0] * rotationA[2][0] - translation[2] * rotationA[0][0]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Ay x By
	radiiA = aHalf[0] * rotationB[2][1] + aHalf[2] * rotationB[0][1];
	radiiB = bHalf[0] * rotationB[1][2] + bHalf[2] * rotationB[1][0];
	if (glm::abs(translation[0] * rotationA[2][1] - translation[2] * rotationA[0][1]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Ay x Bz
	radiiA = aHalf[0] * rotationB[2][2] + aHalf[2] * rotationB[0][2];
	radiiB = bHalf[0] * rotationB[1][1] + bHalf[1] * rotationB[1][0];
	if (glm::abs(translation[0] * rotationA[2][2] - translation[2] * rotationA[0][2]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Az x Bx
	radiiA = aHalf[0] * rotationB[1][0] + aHalf[1] * rotationB[0][0];
	radiiB = bHalf[1] * rotationB[2][2] + bHalf[2] * rotationB[2][1];
	if (glm::abs(translation[0] * rotationA[2][2] - translation[2] * rotationA[0][2]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Az x By
	radiiA = aHalf[0] * rotationB[1][1] + aHalf[1] * rotationB[0][1];
	radiiB = bHalf[0] * rotationB[2][2] + bHalf[2] * rotationB[2][0];
	if (glm::abs(translation[1] * rotationA[0][1] - translation[0] * rotationA[1][1]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// Test axis Az x Bz
	radiiA = aHalf[0] * rotationB[1][2] + aHalf[1] * rotationB[0][2];
	radiiB = bHalf[0] * rotationB[2][1] + bHalf[1] * rotationB[2][0];
	if (glm::abs(translation[1] * rotationA[0][2] - translation[0] * rotationA[1][2]) > radiiA + radiiB)
	{
		return BTXs::eSATResults::SAT_NONE;
	}

	// No separation
	return 1;
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding
	float distanceBetween = glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal());
	float radiusSum = m_fRadius + a_pOther->m_fRadius;
	bool bColliding = distanceBetween < radiusSum;
	/*
	* We use Bounding Spheres or ARBB as a pre-test to avoid expensive calculations (SAT)
	* we default bColliding to true here to always fall in the need of calculating
	* SAT for the sake of the assignment.
	*/
	if (bColliding) //they are colliding with bounding sphere
	{
		uint nResult = SAT(a_pOther);

		if (nResult == 1) //The SAT shown they are colliding
		{
			this->AddCollisionWith(a_pOther);
			a_pOther->AddCollisionWith(this);
		}
		else //they are not colliding
		{
			this->RemoveCollisionWith(a_pOther);
			a_pOther->RemoveCollisionWith(this);
		}
	}
	else //they are not colliding with bounding sphere
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}
	return bColliding;
}
void MyRigidBody::Init(void)
{
	m_pModelMngr = ModelManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pModelMngr, a_pOther.m_pModelMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pModelMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void) { return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pModelMngr = a_pOther.m_pModelMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };

//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}

void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}
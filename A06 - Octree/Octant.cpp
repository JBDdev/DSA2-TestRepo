#include "Octant.h"
using namespace Simplex;

int Octant::octantCount = 0;
int Octant::maxLevel = 3;
int Octant::idealEntityCount = 5;
int Octant::GetOctantCount(void) 
{
	return octantCount;
}

void Octant::Init(void) 
{
	numOfChildren = 0;

	octantSize = 0.0f;

	octantID = octantCount;
	octantLevel = 0;

	center = vector3(0.0f);
	min = vector3(0.0f);
	max = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	rootOctant = nullptr;
	parent = nullptr;
	for (int i = 0; i < 8; i++) 
	{
		children[i] = NULL;
	}
}

void Octant::Swap(Octant& other) 
{
	swap(numOfChildren, other.numOfChildren);

	swap(octantSize, other.octantSize);
	swap(octantID, other.octantID);
	swap(rootOctant, other.rootOctant);
	swap(children, other.children);

	swap(center, other.center);
	swap(min, other.min);
	swap(max, other.max);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	swap(octantLevel, other.octantLevel);
	swap(parent, other.parent);

	for (int i = 0; i < 8; i++)
	{
		swap(children[i], other.children[i]);
	}
}

Octant* Octant::GetParent(void)
{
	return parent;
}

void Octant::Release(void) 
{
	if (octantLevel == 0) //Kills the branches if this is the root octant
	{
		KillBranches();
	}

	numOfChildren = 0;
	octantSize = 0.0f;
	entityList.clear();
	KillTheChildren();
	
}

Octant::Octant(int maximumLevel, int idealEntityNum)
{
	Init();

	octantCount = 0;
	maxLevel = maximumLevel;
	idealEntityCount = idealEntityNum;
	octantID = octantCount;

	rootOctant = this;
	KillTheChildren();

	vector<vector3> minsAndMaxes; //Holds the min and max vectors of the bounding object

	int numOfObjects = m_pEntityMngr->GetEntityCount();
	for (int i = 0; i < numOfObjects; i++)
	{
		MyEntity* e = m_pEntityMngr->GetEntity(i);
		MyRigidBody* r = e->GetRigidBody();
		minsAndMaxes.push_back(r->GetMinGlobal());
		minsAndMaxes.push_back(r->GetMaxGlobal());
	}

	MyRigidBody* octantRigidBody = new MyRigidBody(minsAndMaxes);
	vector3 halfWidth = octantRigidBody->GetHalfWidth();
	float mx = halfWidth.x;
	for (int i = 0; i < 3; i++)
	{
		if (mx < halfWidth[i])
		{
			mx = halfWidth[i];
		}
	}
	vector3 localCenter = octantRigidBody->GetCenterLocal();
	minsAndMaxes.clear();
	SafeDelete(octantRigidBody);

	octantSize = mx * 2.0f;
	center = localCenter;
	min = localCenter - (vector3(mx));
	max = localCenter + (vector3(mx));

	octantCount++;

	ConstructTree(maxLevel);
}

Octant::Octant(vector3 centerPt, float size) 
{
	Init();

	center = centerPt;
	octantSize = size;
	
	min = center - (vector3(octantSize) / 2.0f);
	max = center + (vector3(octantSize) / 2.0f);

	octantCount++;
}

Octant::Octant(Octant const& other) 
{
	numOfChildren = other.numOfChildren;
	center = other.center;
	min = other.min;
	max = other.max;

	octantSize = other.octantSize;
	octantID = other.octantID;
	octantLevel = other.octantLevel;
	parent = other.parent;

	rootOctant = other.rootOctant;
	children, other.children;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (int i = 0; i < 8; i++) 
	{
		children[i] = other.children[i];
	}

}

Octant& Octant::operator=(Octant const& other) 
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}

Octant::~Octant() 
{
	Release();
}

float Octant::GetSize(void)
{
	return octantSize;
}

vector3 Octant::GetCenterGlobal(void) 
{
	return center;
}

vector3 Octant::GetMinGlobal(void) 
{
	return min;
}

vector3 Octant::GetMaxGlobal(void)
{
	return max;
}

void Octant::Display(int index, vector3 color) 
{
	if (octantID == index) 
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(octantSize)), color, RENDER_WIRE);
		return;
	}

	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->Display(index);
	}
}

void Octant::Display(vector3 color) 
{
	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->Display(color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(octantSize)), color, RENDER_WIRE);

}

void Octant::Subdivide(void) 
{
	if (octantLevel >= maxLevel) 
	{
		return;
	}

	if (numOfChildren != 0) 
	{
		return;
	}

	numOfChildren = 8;

	float newSize = octantSize / 4.0f;
	float newSizeD = newSize * 2.0f;

	vector3 v3Center = center;

	v3Center.x -= newSize;
	v3Center.y -= newSize;
	v3Center.z -= newSize;

	children[0] = new Octant(v3Center, newSizeD);

	v3Center.x += newSizeD;
	children[1] = new Octant(v3Center, newSizeD);

	v3Center.z += newSizeD;
	children[2] = new Octant(v3Center, newSizeD);

	v3Center.x -= newSizeD;
	children[3] = new Octant(v3Center, newSizeD);

	v3Center.y += newSizeD;
	children[4] = new Octant(v3Center, newSizeD);

	v3Center.z -= newSizeD;
	children[5] = new Octant(v3Center, newSizeD);

	v3Center.x += newSizeD;
	children[6] = new Octant(v3Center, newSizeD);

	v3Center.z += newSizeD;
	children[7] = new Octant(v3Center, newSizeD);

	for (int i = 0; i < 8; i++) 
	{
		children[i]->rootOctant = rootOctant;
		children[i]->parent = this;
		children[i]->octantLevel = octantLevel + 1;

		if (children[i]->ContainsMoreThan(idealEntityCount)) 
		{
			children[i]->Subdivide();
		}
	}
}

Octant* Octant::GetChild(int child) 
{
	if (child > 7) 
	{
		return nullptr;
	}
	return children[child];
}

bool Octant::IsColliding(int rbIndex) 
{
	int objectCount = m_pEntityMngr->GetEntityCount();

	if (rbIndex > objectCount) 
	{
		return false;
	}

	MyEntity* e = m_pEntityMngr->GetEntity(rbIndex);
	MyRigidBody* r = e->GetRigidBody();
	vector3 minOther = r->GetMinGlobal();
	vector3 maxOther = r->GetMaxGlobal();

	//Check x, y, and z bounds
	if (max.x < minOther.x || min.x > maxOther.x) 
	{
		return false;
	}

	if (max.y < minOther.y || min.y > maxOther.y)
	{
		return false;
	}

	if (max.z < minOther.z || min.z > maxOther.z)
	{
		return false;
	}

	return true;

}

bool Octant::IsLeaf(void) 
{
	if (numOfChildren == 0) 
	{
		return true;
	}

	return false;
}

bool Octant::ContainsMoreThan(int numEntities) 
{
	int count = 0;
	int objectCount = m_pEntityMngr->GetEntityCount();

	for (int i = 0; i < objectCount; i++) 
	{
		if (IsColliding(i))
		{
			count++;
		}

		if (count > numEntities) 
		{
			return true;
		}
	}

	return false;
}

void Octant::KillBranches(void) 
{
	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->KillBranches();
		delete children[i];
		children[i] = nullptr;
	}
	numOfChildren = 0;
}

void Octant::DisplayLeafs(vector3 color) 
{
	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->DisplayLeafs(color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(octantSize)), color, RENDER_WIRE);
}

void Octant::ClearEntityList(void) 
{
	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->ClearEntityList();
	}
	entityList.clear();
}

void Octant::ConstructTree(int maximumLevel) 
{
	if (octantLevel != 0) 
	{
		return;
	}

	maxLevel = maximumLevel;
	octantCount = 1;

	entityList.clear();

	KillBranches();

	KillTheChildren();
	
	if (ContainsMoreThan(idealEntityCount)) 
	{
		Subdivide();
	}

	AssignIDToEntity();

	ConstructList();
	
}


void Octant::KillTheChildren() 
{
	for (int i = 0; i < 8; i++)
	{
		delete children[i];
		children[i] = nullptr;
	}
}

void Octant::AssignIDToEntity(void) 
{
	//This will run until a leaf node is found
	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->AssignIDToEntity();
	}

	if (numOfChildren == 0) 
	{
		int entityNum = m_pEntityMngr->GetEntityCount();

		for (int i = 0; i < entityNum; i++) 
		{
			if (IsColliding(i)) 
			{
				entityList.push_back(i);
				m_pEntityMngr->AddDimension(i, octantID);
			}
		}
	}
}


void Octant::ConstructList(void) 
{
	for (int i = 0; i < numOfChildren; i++) 
	{
		children[i]->ConstructList();
	}

	if (entityList.size() > 0) 
	{
		rootOctant->children.push_back(this);
	}
}
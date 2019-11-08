#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		MyEntity* pEntity = m_entityList[i];
		SafeDelete(pEntity);
	}
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size(); i++) 
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			return i;
		}
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	// return nullptr if list is empty
	if (m_entityList.size() == 0)
		return nullptr;

	// avoids array oob errors
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	
	//Check if entity actually exists
	if (temp)
	{
		return temp->GetModel();
	}
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	// return nullptr if list is empty
	if (m_entityList.size() == 0)
		return nullptr;

	// avoids array oob errors
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//Check if entity actually exists
	if (temp)
	{
		return temp->GetRigidBody();
	}
	return nullptr;
	
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	// return identity if list is empty
	if (m_entityList.size() == 0)
		return IDENTITY_M4;

	// avoids array oob errors
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//Check if entity actually exists
	if (temp)
	{
		return temp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//Check if entity actually exists
	if (temp)
	{
		temp->SetModelMatrix(a_m4ToWorld);
	}
	
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	// return  if list is empty
	if (m_entityList.size() == 0)
		return;

	// avoids array oob errors
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	//checking collisions
	for (int i = 0; i < m_uEntityCount - 1; i++)
	{
		for (int j = i + 1; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	
	MyEntity* temp = new MyEntity(a_sFileName, a_sUniqueID);
	
	//Only adds entity if successfully initialized
	if (temp->IsInitialized())
	{
		m_entityList.push_back(temp);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//if the list is empty return
	if (m_entityList.size() == 0)
		return;

	// out of bounds safety
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	// if the entity is not the very last we swap it for the last one
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
	}

	//and then pop it
	MyEntity* temp = m_entityList[m_uEntityCount - 1];
	SafeDelete(temp);
	m_entityList.pop_back();
	m_uEntityCount--;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	RemoveEntity(GetEntityIndex(a_sUniqueID));
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//if the list is empty return nothing
	if (m_entityList.size() == 0)
		return "";

	//index oob error protection
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;

	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//if the list is empty return nothing
	if (m_entityList.size() == 0)
		return nullptr;

	//index oob error protection
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;

	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//Return to avoid index oob errors
	if (a_uIndex >= m_entityList.size()) 
	{
		return;
	}

	m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//Check if entity actually exists
	if (temp)
	{
		temp->AddToRenderList(a_bRigidBody);
	}
}

#ifndef _MYOCTANTCLASS_H_
#define _MYOCTANTCLASS_H_

#include "MyEntityManager.h"

using namespace std;

namespace Simplex
{
	class MyOctant 
	{
		static int octantCount; //stores # of octants
		static int maxLevel; //stores max level an octant can go to
		static int idealEntityCount; //# of entities this octant will contain

		int octantID = 0; //current ID of the octant
		int octantLevel = 0; //current level of the octant
		int numOfChildren = 0; //either zero or eight

		float octantSize; //size of the octant

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;

		vector3 center = vector3(0.0f, 0.0f, 0.0f); //center point of the octant
		vector3 min = vector3(0.0f, 0.0f, 0.0f); //minimum point of the octant
		vector3 max = vector3(0.0f, 0.0f, 0.0f); //maximum point of the octant

		MyOctant* parent = nullptr; //the parent octant of this octant, if there is one
		vector<MyOctant*> children; //stores the children of this octant, if they exist

		vector<int> entityList; //list of entities under the octant

		MyOctant* rootOctant = nullptr; 
		vector<MyOctant*> nodesWithChildren; //only applies to the root octant

	public:

		MyOctant(int maxLevel = 2, int idealEntityCount = 5);

		MyOctant(vector3 center, float size);

		MyOctant(MyOctant const& other);

		MyOctant& operator=(MyOctant const& other);

		~MyOctant(void);

		void Swap(MyOctant& other);

		float GetSize(void);

		vector3 GetCenterGlobal(void);

		vector3 GetMinGlobal(void);

		vector3 GetMaxGlobal(void);

		bool IsColliding(int index);

		void Display(int index, vector3 color = C_YELLOW);

		void Display(vector3 color = C_YELLOW);

		void DisplayLeafs(vector3 color = C_YELLOW);

		void ClearEntityList(void);

		void Subdivide(void);

		MyOctant* GetChild(int child);

		MyOctant* GetParent(void);

		bool IsLeaf(void);

		bool ContainsMoreThan(int entities);

		void KillBranches(void);

		void ConstructTree(int maxLevel = 3);

		void AssignIDToEntity(void);

		int GetOctantCount(void);

		void KillTheChildren(void);

	private:
		
		void Release(void);

		void Init(void);

		void ConstructList(void);

	};

}
#endif
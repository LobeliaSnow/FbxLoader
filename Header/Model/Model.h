#pragma once
namespace FL {
	class Model {
		friend class Mesh;
	private:
		/**@brief fbxÉVÅ[Éì*/
		fbxsdk::FbxScene* scene;
		std::vector<Mesh*> meshes;
		std::vector<Material*> materials;
	private:
		void CalcNormals(fbxsdk::FbxMesh* mesh);
		void TakeMeshes();
		void TakeMatireals();
	public:
		Model(const char* file_path);
		~Model();
		int GetMeshCount();
		Mesh* GetMesh(size_t index);
		int GetMaterialCount();
		Material* GetMaterial(int i);
		Material* GetMaterial(std::string name);
	};
}


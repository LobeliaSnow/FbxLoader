#include "../Common.h"
#include <fbxsdk.h>

#include "../System/System.h"
#include "../AnimationTake/AnimationTake.h"
#include "../Cluster/Cluster.h"
#include "../Skin/Skin.h"
#include "../Mesh/Mesh.h"
#include "../Material/Material.h"
#include "../Model/Model.h"

namespace FL {
	Model::Model(const char* file_path) {
		fbxsdk::FbxImporter* importer = FbxImporter::Create(System::GetInstance()->manager, "Importer");
		scene = fbxsdk::FbxScene::Create(System::GetInstance()->manager, "Scene");
		//Fbxファイルを開く
		if (!importer->Initialize(file_path))	STRICT_THROW("Fbxファイルが開けませんでした");
		//シーンを取り出す
		if (!importer->Import(scene))STRICT_THROW("シーンの取り出しに失敗");
		importer->Destroy();
		System::GetInstance()->Triangulate(scene);
		System::GetInstance()->SplitMesh(scene);
		TakeMeshes();
		TakeMatireals();
	}
	Model::~Model() {
		int meshCount = GetMeshCount();
		for (size_t i = 0; i < meshCount; i++) {
			SafeDelete(meshes[i]);
		}
	}
	void Model::CalcNormals(fbxsdk::FbxMesh* mesh) {
		System::GetInstance()->converter->EmulateNormalsByPolygonVertex(mesh);
	}
	void Model::TakeMeshes() {
		int meshCount = GetMeshCount();
		meshes.resize(static_cast<size_t>(meshCount));
		for (int i = 0; i < meshCount; i++) {
			fbxsdk::FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);
			CalcNormals(mesh);
			meshes[i] = new Mesh(mesh);
		}
	}
	void Model::TakeMatireals() {
		int materialCount = GetMaterialCount();
		materials.resize(materialCount);
		for (int i = 0; i < materialCount; i++) {
			fbxsdk::FbxSurfaceMaterial* material = scene->GetMaterial(i);
			materials[i] = new Material(material);
		}
	}
	int Model::GetMeshCount() {
		return scene->GetSrcObjectCount<FbxMesh>();
	}
	Mesh* Model::GetMesh(size_t index) { return (GetMeshCount() <= index ? nullptr : meshes[index]); }
	std::vector<Mesh*>& Model::GetMeshes() { return meshes; }
	int Model::GetMaterialCount() { return scene->GetMaterialCount(); }
	Material* Model::GetMaterial(int i) { return materials[i]; }
	Material* Model::GetMaterial(std::string name) {
		for each(auto& material in materials) {
			if (material->GetName() == name)return material;
		}
		return nullptr;
	}
	std::vector<Material*>& Model::GetMaterials() { return materials; }

}
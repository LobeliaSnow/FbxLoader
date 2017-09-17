#pragma once
namespace FL {
	__forceinline Mesh::Mesh(fbxsdk::FbxMesh* mesh) :mesh(mesh), vertexCount(0), normalCount(0), uvCount(0), indexCount(0) {
		TakeVertices();
		TakeNormals();
		TakeUVs();
		TakeIndixes();
		TakeSkins();
		TakeMaterialName();
	}

	__forceinline Mesh::~Mesh() {
		for (int i = 0; i < skinCount; i++) {
			delete skins[i];
		}
	}
	__forceinline void Mesh::TakeVertices() {
		if (vertices.empty()) {
			//頂点
			fbxsdk::FbxVector4* vertexPoint = mesh->GetControlPoints();
			vertexCount = mesh->GetControlPointsCount();
			vertices.resize(static_cast<size_t>(vertexCount));
			for (int i = 0; i < vertexCount; i++) {
				for (int j = 0; j < 3; j++) {
					vertices[i].v[j] = static_cast<float>(vertexPoint[i][j]);
				}
			}
		}
	}
	__forceinline void Mesh::TakeNormals() {
		if (normals.empty()) {
			fbxsdk::FbxArray<FbxVector4> data;
			mesh->GetPolygonVertexNormals(data);
			normalCount = data.Size();
			normals.resize(static_cast<size_t>(normalCount));
			for (int i = 0; i < normalCount; i++) {
				for (int j = 0; j < 3; j++) {
					normals[i].v[j] = static_cast<float>(data[i][j]);
				}
			}
		}
	}
	__forceinline void Mesh::TakeUVs() {
		if (uvs.empty()) {
			//uv
			fbxsdk::FbxStringList uvNames;
			fbxsdk::FbxArray<FbxVector2> uvsets;
			mesh->GetUVSetNames(uvNames);
			mesh->GetPolygonVertexUVs(uvNames.GetStringAt(0), uvsets);
			uvCount = uvsets.Size();
			uvs.resize(uvCount);
			for (int i = 0; i < uvCount; i++) {
				uvs[i].x = static_cast<float>(uvsets[i][0]);
				uvs[i].y = static_cast<float>(1.0f - uvsets[i][1]);
			}
		}
	}
	__forceinline void Mesh::TakeIndixes() {
		if (indices.empty()) {
			int* indexBuffer = mesh->GetPolygonVertices();
			indexCount = mesh->GetPolygonCount() * 3;
			indices.resize(indexCount);
			for (int i = 0; i < indexCount; i++) {
				indices[i] = indexBuffer[i];
			}
		}
	}
	__forceinline void Mesh::TakeSkins() {
		int count = mesh->GetDeformerCount();
		for (int i = 0; i < count; i++) {
			fbxsdk::FbxSkin* skin = static_cast<fbxsdk::FbxSkin*>(mesh->GetDeformer(i, fbxsdk::FbxDeformer::eSkin));
			if (!skin)continue;
			skins.push_back(new Skin(skin));
			skinCount++;
		}
	}
	__forceinline void Mesh::TakeMaterialName() {
		fbxsdk::FbxNode* node = mesh->GetNode();
		int materialCount = node->GetMaterialCount();
		if (materialCount <= 0)STRICT_THROW("マテリアルが存在しない可能性があります");
		//if (materialCount > 1)STRICT_THROW("現在1Mesh辺りにつき複数のマテリアル割り当ては対応されていません\nシステム管理者に問い合わせ下さい");
		//マテリアルごとにメッシュを分割しているために1メッシュあたり1マテリアルとなる
		//for (int i = 0; i < materialCount; i++) {
		fbxsdk::FbxSurfaceMaterial* material = node->GetMaterial(materialCount > 1 ? 1 : 0);
		materialName = material->GetName();
		//}
	}

	__forceinline int Mesh::GetVertexCount() { return vertexCount; }
	__forceinline int Mesh::GetNormalCount() { return normalCount; }
	__forceinline int Mesh::GetUVCount() { return uvCount; }
	__forceinline int Mesh::GetIndexCount() { return indexCount; }
	__forceinline int Mesh::GetSkinCount() { return skinCount; }
	__forceinline const Vector3& Mesh::GetVertex(int index) { return vertices[index]; }
	__forceinline const Vector3& Mesh::GetNormal(int index) { return normals[index]; }
	__forceinline const Vector2& Mesh::GetUV(int index) { return uvs[index]; }
	__forceinline int Mesh::GetIndexBuffer(int index) { return indices[index]; }
	__forceinline Skin* Mesh::GetSkin(int index) { return skins[index]; }
	__forceinline const std::string& Mesh::GetMaterialName() { return materialName; }

}

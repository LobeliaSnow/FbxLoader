#pragma once
namespace FL {
	class Mesh {
	private:
		fbxsdk::FbxMesh* mesh;
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<int> indices;
		std::vector<Vector2> uvs;
		std::string materialName;
		int index;
		int vertexCount;
		int normalCount;
		int uvCount;
		int indexCount;
		int skinCount;
		std::vector<Skin*> skins;
	public:
		Mesh(fbxsdk::FbxMesh* mesh);
		~Mesh();
	private:
		void TakeVertices();
		void TakeNormals();
		void TakeUVs();
		void TakeIndixes();
		void TakeSkins();
		void TakeMaterialName();
	public:
		int GetVertexCount();
		int GetNormalCount();
		int GetUVCount();
		int GetIndexCount();
		int GetSkinCount();
		const Vector3& GetVertex(int index);
		const Vector3& GetNormal(int index);
		const Vector2& GetUV(int index);
		int GetIndexBuffer(int index);
		Skin* GetSkin(int index);
		const std::string& GetMaterialName();
	};
}

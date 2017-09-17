#pragma once
namespace FL {
	class Material {
	private:
		fbxsdk::FbxSurfaceMaterial* material;
		std::string name;
		std::vector<std::string> textures;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;

	private:
		void TakeTextures();
		void TakeLayeredTextures(fbxsdk::FbxProperty& property,int layered_count);
		void TakeNoLayeredTextures(fbxsdk::FbxProperty& property);
		void TakeColor();
	public:
		Material(fbxsdk::FbxSurfaceMaterial* material);
		~Material();
		const std::string& GetName();
		int GetTextureCount();
		const std::string& GetTexture(int i);
		const Vector3& GetAmbient();
		const Vector3& GetDiffuse();
		const Vector3& GetSpecular();

	};
}

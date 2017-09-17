#pragma once
namespace FL {
	__forceinline Material::Material(fbxsdk::FbxSurfaceMaterial* material) :material(material) {
		name = material->GetName();
		TakeTextures();
	}
	__forceinline Material::~Material() = default;

	__forceinline void Material::TakeTextures() {
		fbxsdk::FbxProperty property = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
		//int layeredTextureCount = property.GetSrcObjectCount();
		//if (layeredTextureCount > 0) {
		//	TakeLayeredTextures(property, layeredTextureCount);
		//}
		//else {
		TakeNoLayeredTextures(property);
		//}
	}
	__forceinline void Material::TakeLayeredTextures(fbxsdk::FbxProperty& property, int layered_count) {
		for (int i = 0; i < layered_count; i++) {
			fbxsdk::FbxLayeredTexture* layeredTexture = property.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);
			int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();
			textures.resize(textureCount);
			for (int j = 0; j < textureCount; j++) {
				fbxsdk::FbxFileTexture* texture = property.GetSrcObject<FbxFileTexture>(j);
				if (!texture)continue;
				std::string textureName = texture->GetRelativeFileName();
				textures[i] = textureName.substr(textureName.rfind('\\') + 1, textureName.length());
			}
		}
	}
	__forceinline void Material::TakeNoLayeredTextures(fbxsdk::FbxProperty& property) {
		int textureCount = property.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
		textures.resize(textureCount);
		if (textureCount > 0) {
			for (int i = 0; i < textureCount; i++) {
				fbxsdk::FbxFileTexture* texture = property.GetSrcObject<FbxFileTexture>(i);
				std::string textureName = texture->GetRelativeFileName();
				textures[i] = textureName.substr(textureName.rfind('\\') + 1, textureName.length());
			}
		}
		else textures.push_back("");
	}
	__forceinline const std::string& Material::GetName() { return name; }
	__forceinline int Material::GetTextureCount() { return static_cast<int>(textures.size()); }
	__forceinline const std::string& Material::GetTexture(int i) { return textures[i]; }
	__forceinline void Material::TakeColor() {
		fbxsdk::FbxSurfacePhong* phong = phong = (fbxsdk::FbxSurfacePhong*)material;
		if (!phong) { ambient = Vector3(); diffuse = Vector3(); specular = Vector3();	return; }
		bool isAmbient = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sAmbient).IsValid();
		bool isDiffuse = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse).IsValid();
		bool isSpecular = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sSpecular).IsValid();
		for (int i = 0; i < 3; i++) {
			if (isAmbient)ambient.v[i] = static_cast<float>(phong->Ambient.Get()[i]);
			else ambient = Vector3();
			if (isDiffuse)diffuse.v[i] = static_cast<float>(phong->Diffuse.Get()[i]);
			else diffuse = Vector3();
			if (isSpecular)specular.v[i] = static_cast<float>(phong->Specular.Get()[i]);
			else specular = Vector3();
		}
	}
	__forceinline const Vector3& Material::GetAmbient() { return ambient; }
	__forceinline const Vector3& Material::GetDiffuse() { return diffuse; }
	__forceinline const Vector3& Material::GetSpecular() { return specular; }
}
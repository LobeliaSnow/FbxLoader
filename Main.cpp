#include "FbxLoader.h"

//’´’x‚¢B‰ü‘P‚¹‚æB
int main() {
	FL::System::GetInstance()->Initialize();

	FL::Model* model = nullptr;
	model = new FL::Model("Blue Falcon.FBX");

	int meshCount = model->GetMeshCount();
	int materialCount = model->GetMaterialCount();
	for (int index = 0; index < materialCount; index++) {
		FL::Material* material = model->GetMaterial(index);
		auto test = material->GetTextures();
		Sleep(0);
	}
	for (int index = 0; index < meshCount; index++) {
		FL::Mesh* mesh = model->GetMesh(index);
		auto test = mesh->GetMaterialName();
		Sleep(0);
	}

	FL::System::GetInstance()->Finalize();

	return 0;
}
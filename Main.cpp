#include "FbxLoader.h"

//’´’x‚¢B‰ü‘P‚¹‚æB
int main() {
	FL::System::GetInstance()->Initialize();

	FL::Model* model = nullptr;
	model = new FL::Model("danbo_atk.fbx");
	//model = new FL::Model("Iris_Costume.fbx");
	
	int meshCount = model->GetMeshCount();
	int materialCount = model->GetMaterialCount();
	for (int index = 0; index < meshCount; index++) {
		FL::Mesh* mesh = model->GetMesh(index);
		auto test = mesh->GetMaterialName();
		printf_s("%s\n", test.c_str());
	}
	for (int index = 0; index < materialCount; index++) {
		FL::Material* material = model->GetMaterial(index);
		auto test = material->GetTextures();
		for (int i = 0; i < test.size(); i++) {
			printf_s("%s\n", test[i].c_str());
		}
	}

	FL::System::GetInstance()->Finalize();
	rewind(stdin);
	getchar();

	return 0;
}
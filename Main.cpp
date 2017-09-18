#include "FbxLoader.h"

//’´’x‚¢B‰ü‘P‚¹‚æB
int main() {
	FL::System::GetInstance()->Initialize();

	FL::Model* model = nullptr;
	model = new FL::Model("humanoid.fbx");

	int meshCount = model->GetMeshCount();
	for (int index = 0; index < meshCount; index++) {
		FL::Mesh* mesh = model->GetMesh(index);
		mesh->GetVertexCount();
	}

	FL::System::GetInstance()->Finalize();

	return 0;
}
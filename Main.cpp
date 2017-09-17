#include "FbxLoader.h"
//�Ƃ�ܓK����
//SMD����Ď��ۂɏ�񂪎��Ă��邩�m���߂悤�B
struct Vertex {
	/**@brief �ʒu*/
	Vector4 pos = Vector4(1, 1, 1, 1);
	/**@brief ���_�F*/
	Vector4 color = Vector4(1, 1, 1, 1);
	/**@brief �@��*/
	Vector4 normal = Vector4(1, 1, 1, 1);
	/**@brief UV*/
	Vector2 tex = Vector2();
};

struct Material {
	/**@brief �e�N�X�`���� ���256�ɂȂ�悤�Ɏd�l�ύX*/
	char texname[20];
	/**@brief �����F*/
	Vector3 diffuseColor;
	/**@brief ����F*/
	Vector3 specularColor;
	/**@brief ���F*/
	Vector3 ambientColor;
};

bool CreateSmd(const char* filename, const std::vector<Vertex>& vertex, UINT vertex_sum, const std::vector<Material>& material, const std::vector<int>& delimiter) {
	FILE* fp = nullptr;
	TRY{
		if (fopen_s(&fp, filename, "wb"))		STRICT_THROW("�������݃t�@�C�����J���܂���");
		fwrite(&vertex_sum, sizeof(UINT), 1, fp);
		fwrite(vertex.data(), sizeof(Vertex), vertex_sum, fp);
		UINT delimiterSum = static_cast<UINT>(delimiter.size());
		fwrite(&delimiterSum, sizeof(UINT), 1, fp);
		for each(auto it in material) {
			fwrite(&it, sizeof(Material), 1, fp);
		}
		for each(auto it in delimiter) {
			fwrite(&it, sizeof(int), 1, fp);
		}
		fclose(fp);
	}CATCH(FL::Exception& e) {
		e.BoxMessage();
		fclose(fp);
		return false;
	}
	return true;
}

//���x���B���P����B
int main() {
	FL::System::GetInstance()->Initialize();

	FL::Model* model = nullptr;
	model = new FL::Model("humanoid.fbx");
	//model = new FL::Model("test_stage_3.fbx");
	//model = new FL::Model("sigotoninn.fbx");

	int vertexSum = -1;
	std::vector<Vertex> vertices;
	std::vector<Material> materials;
	std::vector<int> delimiter;
	std::vector<std::string> tn;
	int materialCount = model->GetMaterialCount();
	for (int i = 0; i < materialCount; i++) {
		tn.push_back(model->GetMaterial(i)->GetName());
	}
	for (int i = 0; i < model->GetMeshCount(); i++) {
		FL::Mesh* mesh = model->GetMesh(i);
		int indexCount = mesh->GetIndexCount();
		vertexSum += indexCount;
		delimiter.push_back(indexCount);
		for (int j = 0; j < indexCount; j++) {
			int index = mesh->GetIndexBuffer(j);
			Vertex v = {};
			for (int k = 0; k < 3; k++) {
				v.pos.v[k] = mesh->GetVertex(index).v[k];
				v.normal.v[k] = mesh->GetNormal(index).v[k];
			}
			v.color = Vector4(1, 1, 1, 1);
			if (mesh->GetUVCount() > i) {
				for (int k = 0; k < 2; k++) {
					v.tex.v[k] = mesh->GetUV(j).v[k];
				}
			}
			vertices.push_back(v);
		}
		Material mat;
		strcpy_s(mat.texname, model->GetMaterial(mesh->GetMaterialName())->GetTexture(0).c_str());
		for (int j = 0; j < 3; j++) {
			mat.ambientColor.v[j] = model->GetMaterial(mesh->GetMaterialName())->GetAmbient().v[j];
			mat.diffuseColor.v[j] = model->GetMaterial(mesh->GetMaterialName())->GetDiffuse().v[j];
			mat.specularColor.v[j] = model->GetMaterial(mesh->GetMaterialName())->GetSpecular().v[j];
		}
		materials.push_back(mat);
	}
	CreateSmd("A.smd", vertices, vertexSum, materials, delimiter);
	FL::System::GetInstance()->Finalize();

	return 0;
}
#include "../Common.h"
#include <fbxsdk.h>
#include "../System/System.h"

namespace FL {
	void System::Initialize() {
		manager = FbxManager::Create();
		converter = new fbxsdk::FbxGeometryConverter(manager);
	}

	void System::SetSampleFramePerCount(int sample_frame_per_count) { sampleFramePerCount = sample_frame_per_count; }

	void System::Triangulate(fbxsdk::FbxScene* scene) {
		converter->RemoveBadPolygonsFromMeshes(scene);
		//�S�ĎO�p��
		if (!converter->Triangulate(scene, true))	STRICT_THROW("�O�p���Ɏ��s");
	}

	void System::SplitMesh(fbxsdk::FbxScene* scene) {
		//�}�e���A�����ƂɃ��b�V���𕪗�
		if (!converter->SplitMeshesPerMaterial(scene, true))STRICT_THROW("���b�V���̕����Ɏ��s");
	}

	void System::Finalize() {
		delete converter;
		manager->Destroy();
	}

}

#pragma once
namespace FL {
	__forceinline void System::Initialize() {
		manager = FbxManager::Create();
		converter = new fbxsdk::FbxGeometryConverter(manager);
	}

	__forceinline void System::SetSampleFramePerCount(int sample_frame_per_count) { sampleFramePerCount = sample_frame_per_count; }

	__forceinline void System::Triangulate(fbxsdk::FbxScene* scene) {
		converter->RemoveBadPolygonsFromMeshes(scene);
		//�S�ĎO�p��
		if (!converter->Triangulate(scene, true))	STRICT_THROW("�O�p���Ɏ��s");
	}

	__forceinline void System::SplitMesh(fbxsdk::FbxScene* scene) {
		//�}�e���A�����ƂɃ��b�V���𕪗�
		if (!converter->SplitMeshesPerMaterial(scene, true))STRICT_THROW("���b�V���̕����Ɏ��s");
	}

	__forceinline void System::Finalize() {
		delete converter;
		manager->Destroy();
	}

}

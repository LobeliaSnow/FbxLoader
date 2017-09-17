#pragma once
namespace FL {
	__forceinline void System::Initialize() {
		manager = FbxManager::Create();
		converter = new fbxsdk::FbxGeometryConverter(manager);
	}

	__forceinline void System::SetSampleFramePerCount(int sample_frame_per_count) { sampleFramePerCount = sample_frame_per_count; }

	__forceinline void System::Triangulate(fbxsdk::FbxScene* scene) {
		converter->RemoveBadPolygonsFromMeshes(scene);
		//全て三角化
		if (!converter->Triangulate(scene, true))	STRICT_THROW("三角化に失敗");
	}

	__forceinline void System::SplitMesh(fbxsdk::FbxScene* scene) {
		//マテリアルごとにメッシュを分離
		if (!converter->SplitMeshesPerMaterial(scene, true))STRICT_THROW("メッシュの分離に失敗");
	}

	__forceinline void System::Finalize() {
		delete converter;
		manager->Destroy();
	}

}

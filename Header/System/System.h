#pragma once
namespace FL {
	class System :public Singleton<System> {
		friend class Singleton<System>;
		friend class Model;
		friend class AnimationTake;
	private:
		/**@brief fbxマネージャー*/
		fbxsdk::FbxManager* manager = nullptr;
		fbxsdk::FbxGeometryConverter* converter;
		int sampleFramePerCount = 60;
	private:
		System() = default;
		~System() = default;
	public:
		System(const System&) = delete;
		System(System&&) = delete;
	private:
		void Triangulate(fbxsdk::FbxScene* scene);
		void SplitMesh(fbxsdk::FbxScene* scene);

	public:
		void Initialize();
		void SetSampleFramePerCount(int sample_frame_per_count);
		void Finalize();
	};
}


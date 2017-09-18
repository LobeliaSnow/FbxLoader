#pragma once
namespace FL {
	class AnimationTake {
	private:
		class Cluster* cluster;
		fbxsdk::FbxTakeInfo* takeInfo;
		std::string takeName;
		std::vector<Matrix> matrices;
	private:
		void TakeAnimationMatices(fbxsdk::FbxScene* scene);
	public:
		AnimationTake(Cluster* cluster, int index, fbxsdk::FbxTakeInfo* take_Info);
		~AnimationTake();
		const std::string& GetTakeName();
		Matrix& GetCurrentPoseMatrix(int index);
		std::vector<Matrix>& GetMatrices();
	};
}

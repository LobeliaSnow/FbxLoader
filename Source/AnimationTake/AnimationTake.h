#pragma once
namespace FL {
	class AnimationTake {
	private:
		class Cluster* cluster;
		fbxsdk::FbxTakeInfo* takeInfo;
		std::string takeName;
		std::vector<Matrix> matrices;
		std::vector<Matrix> referenceMatrices;
	private:
		void TakeAnimationMatices(fbxsdk::FbxScene* scene);
	public:
		AnimationTake(Cluster* cluster, int index, fbxsdk::FbxTakeInfo* take_Info);
		~AnimationTake();
		
		const std::string& GetTakeName();
		Matrix GetCurrentPoseMatrix(int index);
		Matrix GetReferenceMatrix(int index);
		int GetMatrixSize();
		std::vector<Matrix>& GetMatrices();

	};
}

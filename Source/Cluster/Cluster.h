#pragma once
namespace FL {
	class Cluster {
		friend class AnimationTake;
	private:
		fbxsdk::FbxCluster* cluster;
		int indexCount;
		int animationCount;
		std::vector<int> indices;
		std::vector<float> weights;
		Matrix initMatrix;
		std::vector<AnimationTake*> animationTakes;
	private:
		void TakeIndicesAndWeights();
		void TakeInitMatrix();
		void TakeAnimationTakes();
	public:
		Cluster(fbxsdk::FbxCluster* cluster);
		~Cluster();
		int GetIndexCount();
		int GetImpactIndex(int index);
		std::vector<int>& GetImpactIndices();
		float GetWeight(int index);
		std::vector<float> GetWeights();
		Matrix GetInitPoseMatrix();
		int GetAnimationCount();
		AnimationTake* GetAnimationTake(int index);
		std::vector<AnimationTake*>& GetAnimationTakes();
	};
}


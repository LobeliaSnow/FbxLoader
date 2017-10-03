#pragma once
namespace FL {
	class Skin {
	private:
		fbxsdk::FbxSkin* skin;
		int clusterCount;
		std::vector<Cluster*> clusters;
	public:
		Skin(fbxsdk::FbxSkin* skin, fbxsdk::FbxMesh* mesh);
		~Skin();
		int GetClusterCount();
		Cluster* GetCluster(int index);
		std::vector<Cluster*>& GetClusters();
	};
}

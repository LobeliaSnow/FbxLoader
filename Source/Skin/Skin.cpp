#include "../Common.h"
#include "fbxsdk.h"
#include "../AnimationTake/AnimationTake.h"
#include "../Cluster/Cluster.h"
#include "Skin.h"

namespace FL {
	Skin::Skin(fbxsdk::FbxSkin* skin) :skin(skin) {
		clusterCount = skin->GetClusterCount();
		for (int i = 0; i < clusterCount; i++) {
			clusters.push_back(new Cluster(skin->GetCluster(i)));
		}
	}
	Skin::~Skin() {
		for (int i = 0; i < clusterCount; i++) {
			SafeDelete(clusters[i]);
		}
	}
	int Skin::GetClusterCount() { return clusterCount; }
	Cluster* Skin::GetCluster(int index) { return clusters[index]; }


}
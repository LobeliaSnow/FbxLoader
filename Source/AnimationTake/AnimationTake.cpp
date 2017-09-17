#pragma once
#include "../Common.h"
#include <fbxsdk.h>

#include "../System/System.h"
#include "../AnimationTake/AnimationTake.h"
#include "../Cluster/Cluster.h"

namespace FL {
	AnimationTake::AnimationTake(Cluster* cluster, int index, fbxsdk::FbxTakeInfo* take_Info) :cluster(cluster), takeInfo(take_Info), takeName(take_Info->mName) {
		fbxsdk::FbxScene* scene = cluster->cluster->GetScene();
		//対象のアニメーションをセット
		scene->SetCurrentAnimationStack(scene->GetSrcObject<FbxAnimStack>(index));
		TakeAnimationMatices(scene);
	}
	AnimationTake::~AnimationTake() {

	}
	void AnimationTake::TakeAnimationMatices(fbxsdk::FbxScene* scene) {
		fbxsdk::FbxGlobalSettings &globalSetting = scene->GetGlobalSettings();
		fbxsdk::FbxTime begin = takeInfo->mLocalTimeSpan.GetStart();
		fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
		fbxsdk::FbxTime period;
		period.SetTime(0, 0, 0, 1, 0, globalSetting.GetTimeMode());
		int beginFrame = (int)(begin.Get() / period.Get());
		int endFrame = (int)(end.Get() / period.Get());
		int frameCount = endFrame - beginFrame;
		int totalTime = end.GetMilliSeconds() - begin.GetMilliSeconds();
		float bias = System::GetInstance()->sampleFramePerCount / fbxsdk::FbxTime::GetFrameRate(globalSetting.GetTimeMode());
		int loopCount = frameCount*bias;
		matrices.resize(static_cast<size_t>(loopCount));
		for (int i = 0; i < loopCount; i++) {
			fbxsdk::FbxMatrix matrix;
			fbxsdk::FbxTime time = begin + period.Get() * (static_cast<float>(i) / 2.0f);
			matrix = cluster->cluster->GetLink()->EvaluateGlobalTransform(time);
			for (int row = 0; row < 4; row++) {
				for (int column = 0; column < 4; column++) {
					matrices[i].mat[row][column] = static_cast<float>(matrix.GetRow(row)[column]);
				}
			}
		}
	}

}
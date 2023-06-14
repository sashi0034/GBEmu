﻿#include "stdafx.h"
#include "HWFrame.h"

namespace GBEmu::HW::HWFrame
{
	void EmulateFrame(HWEnv& env)
	{
		while (true)
		{
			// CPU実行
			const auto cpuCycle = env.GetCPU().StepOperation(env);

			// CPUで進んだサイクル数の分を他のユニットも更新
			for (int i=0; i<cpuCycle.Count; ++i)
			{
				// タイマ更新
				env.GetTimer().StepCycle(env);
			}
			for (int i=0; i<cpuCycle.Count; ++i)
			{
				// PPU更新
				const auto ppuResult = env.GetPPU().StepCycle(env);
				if (ppuResult.IsDotCycleCompleted) return; // フレーム終了
			}
		}
	}
}

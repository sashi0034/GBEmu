﻿#include "stdafx.h"
#include "HWFrame.h"

namespace GBEmu::HW::HWFrame
{
	void EmulateFrame(HWEnv& env)
	{
		int passedCycle = 0;
		env.GetJoypad().UpdateFrame(env.GetMemory());

		env.Debugger().UpdateFrame(env);
		if (env.Debugger().IsDebugSuspend()) return;

		while (true)
		{
			env.Debugger().UpdateCycle(env);

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
				if (ppuResult.IsEnteredVBlank) return; // フレーム終了
			}

			passedCycle += cpuCycle.Count;
			constexpr int ppuCycle = 70224;
			if (passedCycle > ppuCycle) return;
		}
	}
}

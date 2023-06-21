#include "stdafx.h"
#include "HWFrame.h"

namespace GBEmu::HW::HWFrame
{
	void EmulateFrame(HWEnv& env)
	{
		env.Debugger().UpdateFrame(env);
		if (env.Debugger().IsDebugSuspend()) return;

		int passedCycle = 0;

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

			auto&& lcd = env.GetMemory().GetLCD();
			for (int i=0; i<(lcd.IsLCDDisplayEnable() ? cpuCycle.Count : 0); ++i)
			{
				// PPU更新
				const auto ppuResult = env.GetPPU().StepCycle(env);
				if (ppuResult.IsEnteredVBlank) return; // フレーム終了
			}

			// フラグ更新
			lcd.UpdateLYCoincidenceFlag();

			// デバッガ更新
			env.Debugger().UpdateCycle(env, cpuCycle);

			passedCycle += cpuCycle.Count;
			if (passedCycle > HWParam::PPUCyclePeriod_70224) return;
		}
	}
}

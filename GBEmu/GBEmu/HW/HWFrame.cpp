#include "stdafx.h"
#include "HWFrame.h"

namespace GBEmu::HW
{
	static void emulateFrame(HWEnv& env)
	{
		env.GetAPU().UpdateFrame(env);

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

			for (int i=0; i<cpuCycle.Count; ++i)
			{
				// APU更新
				env.GetAPU().StepCycle(env);
			}

			auto&& lcd = env.GetPPU().GetLCD();
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

	void HWFrame::ControlFrame(HWEnv& env)
	{
		const double actualDeltaTime = Scene::DeltaTime();
		constexpr double virtualDeltaTime = 1.0 / HWParam::FPS;

		m_fragmentTime += actualDeltaTime;

		// 60FPS制御
		while (m_fragmentTime >= virtualDeltaTime)
		{
			m_fragmentTime -= virtualDeltaTime;
			emulateFrame(env);
		}
	}
}

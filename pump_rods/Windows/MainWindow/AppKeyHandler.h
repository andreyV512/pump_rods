#pragma once

namespace AppKeyHandler
{
	void Init();
	void Run();
	void Continue();
	void Stop();
	void RunContine();

	void ExitTubeMode();
	void ReturnTubeMode();

	void KeyPressed(unsigned );

	void DisableAllBtn();
}
#pragma once
#ifdef GUI
#include <functional>

struct GLFWwindow;

namespace WrapGui
{
	struct WindowDetails
	{
		std::string title;
		unsigned width;
		unsigned length;
	};

	class GuiApp
	{
	private:
		std::function<void()> m_Function;
		GLFWwindow* m_MainWindow = nullptr;
		bool m_Run;

	private:
		GuiApp();
		int Internal_Init();
		int Internal_CleanUp();

	public:
		static GuiApp& Get();
		~GuiApp();

		void HideWindow();

		GLFWwindow* GetWindowHandle() const { return m_MainWindow; }
		bool* GetRunStatePtr() { return &m_Run; }

		void SetMain(const std::function<void()>& function);
		void Run();
		void Close() { m_Run = false; }

	};
}
#endif
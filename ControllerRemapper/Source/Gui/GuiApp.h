#pragma once
#ifdef GUI
#include <functional>

struct GLFWwindow;

namespace WrapGui
{
	struct WindowDetails
	{
		std::string title = "WrapGui";
		unsigned width = 1280;
		unsigned height = 720;
	};

	class GuiApp
	{
	private:
		std::function<void()> m_Function;
		GLFWwindow* m_MainWindow = nullptr;
		WindowDetails m_WindowDetails;
		bool m_Run;
		static GuiApp* m_Instance;

	private:
		GuiApp(const WindowDetails& windowDetails);
		int Internal_Init();
		int Internal_CleanUp();

	public:
		static GuiApp& Get();
		static void InitGui(const WindowDetails& windowDetails = WindowDetails());
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
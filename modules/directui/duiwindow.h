#pragma once
// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "UIlib.h"

namespace DuiLib{
#ifndef _DWMAPI_H_
	typedef struct DWM_BLURBEHIND{
		DWORD dwFlags;
		BOOL fEnable;
		HRGN hRgnBlur;
		BOOL fTransitionOnMaximized;
	} DWM_BLURBEHIND;

	typedef struct tagDWL_MARGINS {
		int cxLeftWidth;
		int cxRightWidth;
		int cyTopHeight;
		int cyBottomHeight;
	} DWM_MARGINS, *PDWM_MARGINS;

	// Window attributes
	enum DWMWINDOWATTRIBUTE{
		DWMWA_NCRENDERING_ENABLED = 1,      // [get] Is non-client rendering enabled/disabled
		DWMWA_NCRENDERING_POLICY,           // [set] Non-client rendering policy
		DWMWA_TRANSITIONS_FORCEDISABLED,    // [set] Potentially enable/forcibly disable transitions
		DWMWA_ALLOW_NCPAINT,                // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
		DWMWA_CAPTION_BUTTON_BOUNDS,        // [get] Bounds of the caption button area in window-relative space.
		DWMWA_NONCLIENT_RTL_LAYOUT,         // [set] Is non-client content RTL mirrored
		DWMWA_FORCE_ICONIC_REPRESENTATION,  // [set] Force this window to display iconic thumbnails.
		DWMWA_FLIP3D_POLICY,                // [set] Designates how Flip3D will treat the window.
		DWMWA_EXTENDED_FRAME_BOUNDS,        // [get] Gets the extended frame bounds rectangle in screen space
		DWMWA_HAS_ICONIC_BITMAP,            // [set] Indicates an available bitmap when there is no better thumbnail representation.
		DWMWA_DISALLOW_PEEK,                // [set] Don't invoke Peek on the window.
		DWMWA_EXCLUDED_FROM_PEEK,           // [set] LivePreview exclusion information
		DWMWA_CLOAK,                        // [set] Cloak or uncloak the window
		DWMWA_CLOAKED,                      // [get] Gets the cloaked state of the window
		DWMWA_FREEZE_REPRESENTATION,        // [set] Force this window to freeze the thumbnail without live update
		DWMWA_LAST
	};

	// Non-client rendering policy attribute values
	enum DWMNCRENDERINGPOLICY{
		DWMNCRP_USEWINDOWSTYLE, // Enable/disable non-client rendering based on window style
		DWMNCRP_DISABLED,       // Disabled non-client rendering; window style is ignored
		DWMNCRP_ENABLED,        // Enabled non-client rendering; window style is ignored
		DWMNCRP_LAST
	};

	// Values designating how Flip3D treats a given window.
	enum DWMFLIP3DWINDOWPOLICY{
		DWMFLIP3D_DEFAULT,      // Hide or include the window in Flip3D based on window style and visibility.
		DWMFLIP3D_EXCLUDEBELOW, // Display the window under Flip3D and disabled.
		DWMFLIP3D_EXCLUDEABOVE, // Display the window above Flip3D and enabled.
		DWMFLIP3D_LAST
	};

#define DWM_BB_ENABLE                 0x00000001
#define DWM_BB_BLURREGION             0x00000002
#define DWM_BB_TRANSITIONONMAXIMIZED  0x00000004
#define DWM_EC_DISABLECOMPOSITION     0x00000000
#define DWM_EC_ENABLECOMPOSITION      0x00000001
#endif // _DWMAPI_H_

	class CDwm{
	public:
		typedef HRESULT(WINAPI *FNDWMENABLECOMPOSITION)(UINT);
		typedef HRESULT(WINAPI *FNDWNISCOMPOSITIONENABLED)(LPBOOL);
		typedef HRESULT(WINAPI *FNENABLEBLURBEHINDWINDOW)(HWND, CONST DWM_BLURBEHIND*);
		typedef HRESULT(WINAPI *FNDWMEXTENDFRAMEINTOCLIENTAREA)(HWND, CONST DWM_MARGINS*);
		typedef HRESULT(WINAPI *FNDWMSETWINDOWATTRIBUTE)(HWND, DWORD, LPCVOID pvAttribute, DWORD);
		FNDWMENABLECOMPOSITION fnDwmEnableComposition;
		FNDWNISCOMPOSITIONENABLED fnDwmIsCompositionEnabled;
		FNENABLEBLURBEHINDWINDOW fnDwmEnableBlurBehindWindow;
		FNDWMEXTENDFRAMEINTOCLIENTAREA fnDwmExtendFrameIntoClientArea;
		FNDWMSETWINDOWATTRIBUTE fnDwmSetWindowAttribute;
		CDwm::CDwm();
		BOOL IsCompositionEnabled() const;
		BOOL EnableComposition(UINT fEnable);
		BOOL EnableBlurBehindWindow(HWND hWnd);
		BOOL EnableBlurBehindWindow(HWND hWnd, CONST DWM_BLURBEHIND& bb);
		BOOL ExtendFrameIntoClientArea(HWND hWnd, CONST DWM_MARGINS& Margins);
		BOOL SetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
	};

#ifndef WM_DPICHANGED
#   define WM_DPICHANGED       0x02E0
#endif

#ifndef _SHELLSCALINGAPI_H_
	typedef enum _PROCESS_DPI_AWARENESS {
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;
	typedef enum _MONITOR_DPI_TYPE {
		MDT_EFFECTIVE_DPI = 0,
		MDT_ANGULAR_DPI = 1,
		MDT_RAW_DPI = 2,
		MDT_DEFAULT = MDT_EFFECTIVE_DPI
	} Monitor_DPI_Type;
#endif // _SHELLSCALINGAPI_H_

	class CDPI{
	public:
		typedef BOOL(WINAPI *FNSETPROCESSDPIAWARE)(VOID);
		typedef HRESULT(WINAPI *FNSETPROCESSDPIAWARENESS)(PROCESS_DPI_AWARENESS);
		typedef HRESULT(WINAPI *FNGETDPIFORMONITOR)(HMONITOR, Monitor_DPI_Type, UINT*, UINT*);
		FNSETPROCESSDPIAWARE fnSetProcessDPIAware; // vista,win7
		FNSETPROCESSDPIAWARENESS fnSetProcessDpiAwareness; // win8+
		FNGETDPIFORMONITOR fnGetDpiForMonitor; //
		CDPI();
		PROCESS_DPI_AWARENESS GetAwareness();
		int  Scale(int x);
		UINT GetScale();
		void ScaleRect(__inout RECT *pRect);
		void ScalePoint(__inout POINT *pPoint);
		void OnDPIChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
	private:
		UINT m_nScaleFactor;
		UINT m_nScaleFactorSDA;
		PROCESS_DPI_AWARENESS m_Awareness;
		BOOL SetAwareness(PROCESS_DPI_AWARENESS value);
		void SetScale(__in UINT iDPI);
	};

	class CDuiWidow : public WindowImplBase, public CDwm, public CDPI{
	public:
		CDuiWidow() {}
		LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
		UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
		void OnFinalMessage(HWND hWnd) { delete this; };
		virtual CDuiString GetSkinFile() override;
		virtual CDuiString GetSkinFolder() override;
		void Notify(TNotifyUI& msg);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void CreateWithXML(HWND hParent,LPCTSTR lpszXml);
	private:
		CDuiString xml_;
		bool isframe_ = false;
	};
}




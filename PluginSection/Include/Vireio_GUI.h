/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_GUI.h>
and Class <Vireio_GUI>:
Copyright (C) 2015 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <vector>

#define MAX_CONTROLS_PER_PAGE 2048

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_INT(a) { wchar_t buf[128]; wsprintf(buf, L"%i", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

/**
* Vireio control types enumeration.
* All types of possible GUI controls are listed here.
***/
enum Vireio_Control_Type
{
	StaticListBox,
	ListBox,
	SpinControl,
	EditLine,
	Slider,
	CheckBox,
};

/**
* Vireio control action enumeration.
* Type of action the active control is bound to.
***/
enum Vireio_Control_Action
{
	None,
	ScrollBar,
};

/**
* Vireio GUI event as returned to the calling class.
***/
enum Vireio_GUI_Event_Type
{
	NoEvent,
	ChangedToNext,
	ChangedToPrevious,
	ChangedToValue,
	ChangedToText,
	Pressed
};

/**
* Vireio GUI event as returned to the calling class.
***/
struct Vireio_GUI_Event
{
	Vireio_GUI_Event_Type eType;
	UINT dwIndexOfPage;
	UINT dwIndexOfControl;
	UINT dwIndexOfEntry;
	UINT dwIndexOfSubentry;
	union
	{
		UINT dwNewValueUINT;
		INT nNewValueINT;
		FLOAT fNewValueFLOAT;
		std::wstring* pszNewValueWSTRING;
	};
};

/**
* Vireio static list box control structure.
* Stores all data necessary for a static list box control.
***/
struct Vireio_StaticListBox_Data
{
	bool m_bSelectable;                       /**< True if this list box is used as a selectable menu. ***/
	INT m_dwCurrentSelection;                 /**< Current selection, -1 if nothing is selected. ***/
	std::vector<std::wstring>* m_paszEntries; /**< Pointer to Vector of entries in the current list. ***/
};

/**
* Vireio list box control structure.
* Stores all data necessary for a list box control.
***/
struct Vireio_ListBox_Data : public Vireio_StaticListBox_Data
{
	float m_fScrollPosY;                     /**< The position of the scroll bar. ***/
};

/**
* Vireio control structure.
* All data for a single GUI control is stored here.
***/
struct Vireio_Control
{
	POINT m_sPosition;                   /**< The position of the control in pixel space. ***/
	SIZE m_sSize;                        /**< The size of the control in pixel space. ***/
	Vireio_Control_Type m_eControlType;  /**< The type of the control. ***/
	union                                /**< Control type-specific data. ***/
	{
		Vireio_StaticListBox_Data m_sStaticListBox;
		Vireio_ListBox_Data m_sListBox;
	};
};

/**
* Vireio page structure.
* All data for a single page is stored here.
***/
struct Vireio_Page
{
	std::vector<Vireio_Control> m_asControls; /**< All stored controls for that page. ***/
};

/**
* Vireio general Graphical User Interface class.
* This class provides and handles a windows bitmap
* as a configurable User Interface.
***/
class Vireio_GUI
{
public:
	Vireio_GUI(SIZE sSize, LPCWSTR szFont, BOOL bItalic, DWORD dwFontSize, COLORREF dwColorFront, COLORREF dwColorBack);
	~Vireio_GUI();

	HBITMAP          GetGUI();
	void             DrawStaticListBox(HDC hdc, Vireio_Control& sControl);
	void             DrawListBox(HDC hdc, Vireio_Control& sControl);
	UINT             AddPage() { Vireio_Page sPage; ZeroMemory(&sPage, sizeof(sPage)); m_asPages.push_back(sPage); return (UINT)m_asPages.size() - 1; } /**< Adds a new page. ***/
	UINT             AddControl(UINT dwPage, Vireio_Control& sControl);
	void             AddEntry(UINT dwControl, LPCWSTR szString);
	Vireio_GUI_Event WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	/**
	* All stored pages for that GUI.
	***/
	std::vector<Vireio_Page> m_asPages;
	/**
	* Current page index
	***/
	UINT m_dwCurrentPage;
	/**
	* The constant size of this GUI, in pixel space.
	***/
	SIZE m_sGUISize;
	/**
	* The font used.
	***/
	HFONT m_hFont;
	/**
	* The size of the font used.
	***/
	DWORD m_dwFontSize;
	/**
	* The color of the front font.
	***/
	COLORREF m_dwColorFront;
	/**
	* The color of the background.
	***/
	COLORREF m_dwColorBack;
	/**
	* The name of the used font.
	***/
	std::wstring m_szFontName;
	/**
	* The control bitmap.
	***/
	HBITMAP m_hBitmapControl;
	/**
	* The active control index.
	***/
	UINT m_dwActiveControl;
	/**
	* The current control action.
	***/
	Vireio_Control_Action m_eActiveControlAction;
	/**
	* The control update bool.
	***/
	bool m_bControlUpdate;
	/**
	* True if the mouse is currently bound to a control.
	* (in case left mouse button is down)
	***/
	bool m_bMouseBoundToControl;
	/**
	* Mouse coords.
	***/
	POINT m_sMouseCoords;
};